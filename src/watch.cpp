#include <watch.hpp>
#include <dir_reader.hpp>
#include <iostream>
#include <sys/inotify.h>
#include <log/log.hpp>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <child_process.hpp>
#include <utils/utils.hpp>
#include <thread>
#include <wait.h>
#include <sys/resource.h>

#define MAX_EVENTS      1024
#define MAX_FNAME       NAME_MAX
#define EV_LEN          (sizeof(struct inotify_event))
#define BUF_LEN         ( MAX_EVENTS * ( EV_LEN + MAX_FNAME) )

int inotify_fd;
int child_pid = -1;

#ifdef DEBUGMODE
    Log_class logger("debug-log.txt");
#else
    Log_class logger("/tmp/watchman-log.txt");
#endif

void sigint_handler(int sig) {
    std::cout<<"Terminating..."<<std::endl;
    close(inotify_fd);
    exit(0);
}

void child_monitor() {
    int child_status;
    rusage _;
    if(wait4(child_pid, &child_status, WSTOPPED, &_) == -1) {
        return;
    }

    int child_sig = WTERMSIG(child_status);
    int child_rc = WEXITSTATUS(child_status);

    if(child_sig == SIGUSR1) {
        logger.write(LOG_LEVEL::FATAL, "Failed to run given command, check if your comand is correct", true);
    }

    if(child_rc != 0) {
        logger.write(LOG_LEVEL::INFO, "Command exited with errors", true);
    }
    else {
        logger.write(LOG_LEVEL::INFO, "Command exited without any errors", true);
    }
    std::cout<<"Return Code: "<<child_rc<<std::endl;
}

void restart_child(watchman_config *config) {
    if (child_pid != -1) {
        kill(child_pid, SIGKILL);
    }

    child_pid = fork();
    
    if(child_pid < 0) {
        logger.write(LOG_LEVEL::FATAL, "Failed to fork! I cannot continue without this thingy", true);
    }
    else if(child_pid == 0) {
        child_process(config);
    }
    else {
        std::thread t(child_monitor);
        t.detach();
    }
}

void display_event(inotify_event *i) {
    std::string result_message;

    if (i->mask & IN_ISDIR) {
        result_message += "Directory: ";
    }
    else {
        result_message += "File: ";
    }

    result_message += i->name;

    if (i->mask & IN_MODIFY) {
        result_message += " was modified.";
    }
    else if(i->mask & IN_CREATE) {
        result_message += " was created.";
    }
    else if(i->mask & IN_DELETE) {
        result_message += " was removed.";
    }
    std::cout<<result_message<<std::endl;
}

int watch(watchman_config *config) {
    signal(SIGINT, sigint_handler);
    std::filesystem::path p = std::filesystem::current_path();
    char buf[BUF_LEN];
    ssize_t rd_s;
    inotify_fd = inotify_init();

    if (inotify_fd < 0) {
        logger.write(LOG_LEVEL::FATAL, "Failed to initialize inotify for watching files!", true);
    }

    for (auto f: get_files(p, config->excluded, config->recursive)) {
        inotify_add_watch(inotify_fd, f.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
    }

    logger.write(LOG_LEVEL::INFO, "Running given command for the first time...", true);
    restart_child(config);

    for(;;) {
        rd_s = read(inotify_fd, buf, BUF_LEN);

        char *s;
        int changed_ones = 0;
        for(s = buf; s < buf + rd_s;) {
            struct inotify_event *ie = (struct inotify_event *)s;

            if (!has_value(config->excluded, std::string() + ie->name)) {
                if (std::filesystem::path((char*)ie->name).has_extension()) {
                    if(!has_value(config->extensions, std::filesystem::path((char*)ie->name).extension().string())) {
                        continue;
                    }
                }
                if (config->verbose)
                    display_event(ie);
                changed_ones += 1;
            }

            s += EV_LEN + ie->len;
        }

        if (changed_ones > 0) {
            logger.write(LOG_LEVEL::INFO, "File changes detected, restarting...", true);
            restart_child(config);
        }
    }

    return 0;
}
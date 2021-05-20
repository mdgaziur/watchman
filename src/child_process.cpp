#include <child_process.hpp>
#include <unistd.h>
#include <signal.h>

void child_process(watchman_config *config) {
    char *writable_cmd = new char[config->command.size() + 1];
    std::copy(config->command.begin(), config->command.end(), writable_cmd);
    writable_cmd[config->command.size()] = '\0';
    char *args[] = { (char*)"/bin/sh", (char*)"-c", writable_cmd, NULL };

    execve("/bin/sh", &args[0], __environ);
    raise(SIGUSR1);
}
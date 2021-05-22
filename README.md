# Watchman
### Simple file watcher program that executes command when change is detected

## Why?
I was just exploring all the syscalls and features of Linux and I found Inotify. It watches for file changes and saves the event data that can be converted to a c struct
in a fd. So I was curious and created this.

## How to use?
Create watchman.config.json in the directory where the program will watch for. You can use the schema in the schema folder for VSCode. The watchman.config.json in 
this program's source codes root directory can be used as a template.

## Is it platform independent?
I will only work on Linux afaik. I'm not planning to add support for any other platform.

## Contributing
Any bugs, features? Make issue and PRs.

#pragma once
#include <vector>
#include <string>

struct watchman_config
{
    bool recursive = false;
    std::vector<std::string> extensions;
    std::vector<std::string> filenames;
    std::vector<std::string> excluded_dirs;
    std::string command;
    bool show_changed_file_name = false;
    bool verbose = false;
};
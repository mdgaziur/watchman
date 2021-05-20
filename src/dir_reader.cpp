#include <dir_reader.hpp>
#include <iostream>
#include <utils/utils.hpp>

std::vector<std::string> get_files(std::filesystem::path current_path, std::vector<std::string> excluded_dirs, bool recursive) {
    std::vector<std::string> result;
    result.push_back(current_path.string());

    std::filesystem::directory_iterator dr(current_path);

    for(auto p: dr) {
        if (p.is_directory()
            && !has_value(excluded_dirs, p.path().lexically_relative(current_path))
            && recursive) {
                for (auto d: get_files(p.path(), excluded_dirs, recursive)) {
                    result.push_back(d);
                }
            }
    }

    return result;
}
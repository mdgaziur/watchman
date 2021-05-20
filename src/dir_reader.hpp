#pragma once
#include <string>
#include <vector>
#include <filesystem>

std::vector<std::string> get_files(std::filesystem::path, std::vector<std::string>, bool = false);
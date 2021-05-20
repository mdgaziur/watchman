#include <utils/utils.hpp>

bool has_value(std::vector<std::string> vec, std::string query) {
    for (auto s: vec) {
        if (s == query) {
            return true;
        }
    }
    return false;
}
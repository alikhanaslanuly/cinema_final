#pragma once
#include <fstream>
#include <string>
#include <vector>

// Finds a resource (shaders/..., assets/...) no matter where the exe is started from:
// cwd, up to 3 levels above it, finally the source root baked in by CMake.
namespace paths {
inline bool fileExists(const std::string& p) {
    std::ifstream f(p, std::ios::binary);
    return f.good();
}
inline std::string resolve(const std::string& rel) {
    std::vector<std::string> c = {rel, "../" + rel, "../../" + rel, "../../../" + rel};
#ifdef CINEMA_ROOT_DIR
    c.push_back(std::string(CINEMA_ROOT_DIR) + "/" + rel);
#endif
    for (const auto& p : c)
        if (fileExists(p)) return p;
    return rel;  // caller prints the error with this path
}
}  // namespace paths

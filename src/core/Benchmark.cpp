#include "core/Benchmark.h"

#include <cstdio>
#include <fstream>

#include "core/ShadowMap.h"

namespace {
struct Cfg { bool shadows; int res; };
const Cfg kCfg[4] = {{false, 512}, {false, 2048}, {true, 512}, {true, 2048}};
}  // namespace

void Benchmark::start(bool shadows, int res) {
    m_active = true;
    m_idx = 0;
    m_time = 0.0f;
    m_frames = 0;
    m_origShadows = shadows;
    m_origRes = res;
    m_rows.clear();
    std::printf("[Benchmark] started (4 configurations x %.0f s). Do not touch the window.\n", kWarmup + kMeasure);
}

void Benchmark::apply(bool& shadows, ShadowMap& sm) const {
    shadows = kCfg[m_idx].shadows;
    if (sm.resolution() != kCfg[m_idx].res) sm.resize(kCfg[m_idx].res);
}

bool Benchmark::update(float dt, bool& shadows, ShadowMap& sm) {
    if (!m_active) return false;
    if (m_time == 0.0f && m_frames == 0) apply(shadows, sm);
    m_time += dt;
    if (m_time < kWarmup) return false;
    ++m_frames;
    if (m_time < kWarmup + kMeasure) return false;

    double fps = m_frames / static_cast<double>(m_time - kWarmup);
    m_rows.push_back({kCfg[m_idx].shadows, kCfg[m_idx].res, fps});
    std::printf("[Benchmark] shadows=%s map=%d -> %.1f FPS\n", kCfg[m_idx].shadows ? "ON " : "OFF", kCfg[m_idx].res, fps);
    ++m_idx;
    m_time = 0.0f;
    m_frames = 0;
    if (m_idx >= 4) {
        m_active = false;
        shadows = m_origShadows;
        if (sm.resolution() != m_origRes) sm.resize(m_origRes);
        return true;
    }
    return false;
}

void Benchmark::abort(bool& shadows, ShadowMap& sm) {
    if (!m_active) return;
    m_active = false;
    shadows = m_origShadows;
    if (sm.resolution() != m_origRes) sm.resize(m_origRes);
    std::printf("[Benchmark] aborted\n");
}

std::string Benchmark::status() const {
    if (!m_active) return "";
    return "BENCHMARK " + std::to_string(m_idx + 1) + "/4";
}

std::string Benchmark::reportMarkdown(const std::string& gpu) const {
    std::string s = "# FPS measurements\n\nGPU: " + gpu + "\n\n| Shadows | Shadow map | FPS |\n|---|---|---|\n";
    for (const auto& r : m_rows) {
        char line[128];
        std::snprintf(line, sizeof(line), "| %s | %s | %.1f |\n", r.shadows ? "ON" : "OFF",
                      r.shadows ? std::to_string(r.res).c_str() : (std::to_string(r.res) + " (unused)").c_str(), r.fps);
        s += line;
    }
    return s;
}

bool Benchmark::writeReport(const std::string& path, const std::string& gpu) const {
    std::ofstream f(path);
    if (!f) return false;
    f << reportMarkdown(gpu);
    return true;
}

#pragma once
#include <string>
#include <vector>

class ShadowMap;

// F5: measures FPS in 4 configurations (shadows off/on x 512/2048) for the report table.
// Each configuration: 1 s warm-up + 5 s measurement, vsync is switched off by the caller.
class Benchmark {
public:
    struct Row {
        bool shadows;
        int res;
        double fps;
    };
    void start(bool currentShadows, int currentRes);
    bool active() const { return m_active; }
    // returns true once, when the whole run has finished (settings are restored)
    bool update(float dt, bool& shadows, ShadowMap& sm);
    void abort(bool& shadows, ShadowMap& sm);
    std::string status() const;
    std::string reportMarkdown(const std::string& gpu) const;
    bool writeReport(const std::string& path, const std::string& gpu) const;

private:
    void apply(bool& shadows, ShadowMap& sm) const;
    bool m_active = false;
    size_t m_idx = 0;
    float m_time = 0.0f;
    int m_frames = 0;
    bool m_origShadows = true;
    int m_origRes = 2048;
    std::vector<Row> m_rows;
    static constexpr float kWarmup = 1.0f, kMeasure = 5.0f;
};

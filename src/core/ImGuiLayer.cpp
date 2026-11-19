#include "core/ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdio>
#include <string>

#include "core/Paths.h"

namespace {
const char* findFont() {
    static std::string found;
    const char* cands[] = {
        "C:/Windows/Fonts/segoeui.ttf", "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", "/System/Library/Fonts/Supplemental/Arial.ttf"};
    std::string own = paths::resolve("assets/fonts/DejaVuSans.ttf");  // optional bundled font (add licence to LICENSES.md)
    if (paths::fileExists(own)) { found = own; return found.c_str(); }
    for (const char* c : cands)
        if (paths::fileExists(c)) { found = c; return found.c_str(); }
    return nullptr;
}
}  // namespace

bool ImGuiLayer::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;  // do not litter the repo with imgui.ini
    if (const char* font = findFont()) {
        // Cyrillic glyphs are needed for "Купить билет", team names etc.
        io.Fonts->AddFontFromFileTTF(font, 20.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    } else {
        std::fprintf(stderr, "[ImGui] no TTF font found, Cyrillic text will show as '?'. Put DejaVuSans.ttf into assets/fonts/.\n");
    }
    ImGui::StyleColorsDark();
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) return false;  // true = chain our GLFW callbacks
    if (!ImGui_ImplOpenGL3_Init("#version 330")) return false;
    m_ready = true;
    return true;
}

void ImGuiLayer::shutdown() {
    if (!m_ready) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    m_ready = false;
}

void ImGuiLayer::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiLayer::wantsMouse() const { return ImGui::GetIO().WantCaptureMouse; }

// cinema3d — Cinema Seat Preview: Real-Time Shadow Mapping
// main.cpp only wires the modules together (owner: Alikhan).
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

#include "core/Benchmark.h"
#include "core/Camera.h"
#include "core/GLUtil.h"
#include "core/ImGuiLayer.h"
#include "core/Input.h"
#include "core/Light.h"
#include "core/ShadowMap.h"
#include "core/Shader.h"
#include "core/Window.h"
#include "game/Game.h"
#include "scene/Hall.h"
#include "scene/Screen.h"
#include "scene/SeatGrid.h"
#include "ui/UI.h"

namespace {
const char* kTeam = "Алихан, Куаныш, Арман";
const char* kTopic = "Cinema Seat Preview: Real-Time Shadow Mapping";

const char* stageName(AppState s) {
    switch (s) {
        case AppState::Menu: return "MENU";
        case AppState::Hall: return "HALL";
        case AppState::Preview: return "PREVIEW";
        case AppState::Confirm: return "CONFIRM";
        case AppState::Purchased: return "PURCHASED";
    }
    return "?";
}
}  // namespace

int main() {
    // ---- window, input, ImGui -------------------------------------------------------------
    Window window;
    if (!window.init(1280, 720, std::string(kTopic) + " | " + kTeam)) return 1;
    Input input;
    input.attach(window.handle());  // before ImGui: ImGui chains our callbacks
    ImGuiLayer gui;
    if (!gui.init(window.handle())) {
        std::fprintf(stderr, "[main] ImGui init failed\n");
        return 1;
    }

    // ---- shaders and shadow map -----------------------------------------------------------
    Shader depthShader, sceneShader;
    bool shadersOk = depthShader.load("shaders/core/depth.vert", "shaders/core/depth.frag");
    shadersOk = sceneShader.load("shaders/core/blinn_phong.vert", "shaders/core/blinn_phong.frag") && shadersOk;
    if (!shadersOk) {
        std::fprintf(stderr, "[main] shader errors above; cannot continue\n");
        gui.shutdown();
        return 1;
    }
    ShadowMap shadow;
    shadow.init(ShadowMap::kHigh);

    // ---- scene (Kuanysh) ------------------------------------------------------------------
    Hall hall;
    hall.init();
    Screen screen;
    screen.init();
    SeatGrid seats;
    seats.init();
    glm::vec3 hallMin, hallMax;
    hall.bounds(hallMin, hallMax);
    const glm::vec3 hallCenter = (hallMin + hallMax) * 0.5f;

    // ---- core objects ---------------------------------------------------------------------
    Camera camera;
    camera.setWalkBounds(hallMin, hallMax);
    camera.setFloorHeightFn([&hall](float x, float z) { return hall.floorHeight(x, z); });
    camera.setOrbit(hallCenter, 17.0f, 35.0f, 25.0f);
    Light light;
    light.configure(hallMin, hallMax);
    Benchmark bench;
    Game game;

    // ---- runtime state --------------------------------------------------------------------
    bool shadowsOn = true, paused = false, helpVisible = false;
    float animTime = 0.0f, biasTexels = 1.0f;
    int pcfRadius = 1;
    AppState prevState = game.state();
    glm::vec3 savedEye(0.0f), savedLook(0.0f);  // walking pose, restored on "Back"
    const std::string gpuName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    double lastTime = glfwGetTime(), fpsTimer = 0.0;
    int fpsFrames = 0;
    float fps = 0.0f;

    auto drawScene = [&](Shader& s) {
        hall.draw(s);
        seats.draw(s);
        screen.draw(s, animTime);
    };

    while (!window.shouldClose()) {
        input.newFrame();
        Window::pollEvents();
        window.updateSize();

        double now = glfwGetTime();
        float dt = static_cast<float>(std::min(now - lastTime, 0.1));
        lastTime = now;
        fpsTimer += dt;
        ++fpsFrames;
        if (fpsTimer >= 0.5) {
            fps = static_cast<float>(fpsFrames / fpsTimer);
            fpsTimer = 0.0;
            fpsFrames = 0;
            char buf[256];
            std::snprintf(buf, sizeof(buf), "%s | %s | %s | FPS %.0f | Shadows %s %d%s", kTopic, kTeam, stageName(game.state()), fps,
                          shadowsOn ? "ON" : "OFF", shadow.resolution(), paused ? " | PAUSED" : "");
            window.setTitle(buf);
        }

        // ---- global keys ------------------------------------------------------------------
        if (input.pressed(GLFW_KEY_F1) || input.pressed(GLFW_KEY_H)) helpVisible = !helpVisible;
        if (input.pressed(GLFW_KEY_P)) paused = !paused;
        if (input.pressed(GLFW_KEY_T) && !bench.active()) shadowsOn = !shadowsOn;
        if (input.pressed(GLFW_KEY_Q) && !bench.active()) shadow.toggleQuality();
        if (input.pressed(GLFW_KEY_LEFT_BRACKET)) biasTexels = std::max(0.0f, biasTexels - 0.25f);
        if (input.pressed(GLFW_KEY_RIGHT_BRACKET)) biasTexels = std::min(8.0f, biasTexels + 0.25f);
        if (input.pressed(GLFW_KEY_K)) pcfRadius = (pcfRadius + 1) % 3;  // 0:1x1  1:3x3  2:5x5
        if (input.pressed(GLFW_KEY_C)) shadow.cullFrontFaces = !shadow.cullFrontFaces;
        if (input.pressed(GLFW_KEY_V) && !bench.active()) window.setVsync(!window.vsync());
        if (input.pressed(GLFW_KEY_F6)) {
            depthShader.reload();
            sceneShader.reload();
        }
        if (input.pressed(GLFW_KEY_F5) && !bench.active()) {
            window.setVsync(false);
            bench.start(shadowsOn, shadow.resolution());
        }
        if (input.pressed(GLFW_KEY_O) && game.state() == AppState::Hall && !camera.transitioning())
            camera.toggleOrbit(hallCenter, 17.0f);
        if (input.pressed(GLFW_KEY_ESCAPE)) {
            if (bench.active()) {
                bench.abort(shadowsOn, shadow);
                window.setVsync(true);
            } else if (helpVisible) {
                helpVisible = false;
            } else if (camera.mode() == Camera::Mode::Orbit && game.state() == AppState::Hall) {
                camera.toggleOrbit(hallCenter, 17.0f);
            } else if (!game.back(seats)) {
                window.requestClose();
            }
        }
        if (bench.update(dt, shadowsOn, shadow)) {
            bench.writeReport("fps_results.md", gpuName);
            std::printf("%s\n[Benchmark] saved to fps_results.md\n", bench.reportMarkdown(gpuName).c_str());
            window.setVsync(true);
        }

        // ---- per-frame updates --------------------------------------------------------------
        if (!paused) animTime += dt;
        light.update(dt, input);

        AppState st = game.state();
        const bool walking = st == AppState::Hall && camera.mode() == Camera::Mode::FirstPerson;
        input.setCursorCaptured(walking);
        camera.update(dt, input, st == AppState::Hall, st == AppState::Menu && !paused);

        // HALL: click the seat under the crosshair
        if (walking && !camera.transitioning() && !gui.wantsMouse() && input.mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
            int id = seats.pick(camera.centerRay());
            if (id >= 0 && !seats.seats()[id].taken) {
                savedEye = camera.position();
                savedLook = savedEye + camera.front();
                game.seatPicked(id, seats);
                camera.setFirstPerson(seats.seats()[id].eyePos, screen.center());  // glide to the spectator's eyes
            }
        }
        if (game.state() == AppState::Preview && !camera.transitioning()) game.cameraArrived();

        // ---- UI (ImGui screens) -------------------------------------------------------------
        gui.beginFrame();
        ui::Hud hud{kTeam, kTopic, fps, shadowsOn, shadow.resolution(), paused, helpVisible,
                    camera.mode() == Camera::Mode::Orbit, window.vsync(), biasTexels, pcfRadius * 2 + 1, ""};
        const std::string benchText = bench.status();
        hud.benchmark = benchText.c_str();
        UiAction action = ui::draw(game, seats, hud);
        if (action != UiAction::None) game.apply(action, seats);

        // ---- state transitions -> camera ----------------------------------------------------
        AppState cur = game.state();
        if (cur != prevState) {
            if (cur == AppState::Menu) {
                camera.setOrbit(hallCenter, 17.0f, 35.0f, 25.0f);
            } else if (cur == AppState::Hall) {
                if (prevState == AppState::Menu) {
                    glm::vec3 eye, look;
                    hall.spawnPose(eye, look);
                    camera.setFirstPerson(eye, look);
                } else {
                    camera.setFirstPerson(savedEye, savedLook);
                }
            }
            prevState = cur;
        }

        // ---- render -------------------------------------------------------------------------
        const int fbw = window.fbWidth(), fbh = window.fbHeight();
        if (fbw > 0 && fbh > 0) {
            const bool useShadows = shadowsOn && shadow.ok();
            const glm::mat4 lightSpace = light.lightSpace();

            if (useShadows) {  // pass 1: depth from the light into the FBO
                shadow.begin();
                depthShader.use();
                depthShader.setMat4("uLightSpace", lightSpace);
                drawScene(depthShader);
                shadow.end(fbw, fbh);
            }

            // pass 2: main render with Blinn-Phong + shadow lookup
            glViewport(0, 0, fbw, fbh);
            glClearColor(0.04f, 0.045f, 0.06f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);

            sceneShader.use();
            sceneShader.setMat4("uView", camera.view());
            sceneShader.setMat4("uProj", camera.projection(window.aspect()));
            sceneShader.setVec3("uViewPos", camera.position());
            sceneShader.setVec3("uLightPos", light.position());
            sceneShader.setVec3("uLightDir", light.direction());
            sceneShader.setVec3("uLightColor", light.color);
            sceneShader.setFloat("uCosInner", std::cos(light.innerDeg * 3.14159265f / 180.0f));
            sceneShader.setFloat("uCosOuter", std::cos(light.outerDeg * 3.14159265f / 180.0f));
            sceneShader.setVec3("uAmbientSky", glm::vec3(0.20f, 0.21f, 0.26f));
            sceneShader.setVec3("uAmbientGround", glm::vec3(0.08f, 0.07f, 0.07f));
            sceneShader.setFloat("uShininess", 48.0f);
            sceneShader.setFloat("uSpecStrength", 0.35f);
            sceneShader.setBool("uShadowsOn", useShadows);
            sceneShader.setMat4("uLightSpace", lightSpace);
            sceneShader.setFloat("uLightNear", light.nearPlane);
            sceneShader.setFloat("uLightFar", light.farPlane);
            const float res = static_cast<float>(std::max(1, shadow.resolution()));
            sceneShader.setFloat("uTexelScale", 2.0f * std::tan(light.fovDeg() * 0.5f * 3.14159265f / 180.0f) / res);
            sceneShader.setFloat("uBiasTexels", biasTexels);
            sceneShader.setInt("uPcfRadius", pcfRadius);
            sceneShader.setInt("uTexture", 0);
            sceneShader.setInt("uShadowMap", 1);
            if (useShadows) shadow.bindTexture(1);
            drawScene(sceneShader);
            glCheck("frame");
        }

        gui.render();
        window.swapBuffers();
    }

    gui.shutdown();
    return 0;
}

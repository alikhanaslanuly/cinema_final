# Контракт между модулями (Алихан = core + main.cpp)

`main.cpp` вызывает чужие модули ТОЛЬКО через интерфейсы ниже. Пункты с пометкой **(+)** — то, что
из плана не следовало напрямую, но нужно, чтобы `main.cpp` собрался. Их надо согласить в первый час.

## Куаныш (src/scene, src/picking)
```cpp
// picking/Ray.h
struct Ray { glm::vec3 origin; glm::vec3 dir; };          // dir — единичный

// scene/Mesh.h — раскладка вершин ВСЕХ мешей:
//   location 0 = vec3 position, 1 = vec3 normal, 2 = vec2 uv

// scene/SeatGrid.h  (как в плане)
struct Seat { int id; int row, col; glm::vec3 aabbMin, aabbMax; glm::vec3 eyePos; bool taken; };
class SeatGrid {
  void init();                                  // (+) вызывается после создания GL-контекста
  const std::vector<Seat>& seats() const;       // seats()[id].id == id
  int  pick(const Ray&) const;                  // id или -1
  void setTaken(int id, bool);
  void setSelected(int id);                     // -1 = снять подсветку
  void draw(Shader&) const;
};

// scene/Hall.h   (+)
class Hall {
  void init();
  void draw(Shader&) const;
  float floorHeight(float x, float z) const;    // высота пола (уступы) — камера ходит по ней
  void bounds(glm::vec3& mn, glm::vec3& mx) const;           // внутренний AABB зала (потолок = mx.y)
  void spawnPose(glm::vec3& eye, glm::vec3& lookAt) const;   // где игрок входит в зал
};

// scene/Screen.h (+)
class Screen {
  void init();
  void draw(Shader&, float time) const;         // time замораживается клавишей P
  glm::vec3 center() const;                     // куда смотреть из кресла
};
```
**Uniform-ы, которые `draw()` каждого объекта ОБЯЗАН выставить сам** (шейдер core их не помнит):
`uModel` (mat4), `uColor` (vec3), `uEmissive` (vec3), `uUseTexture` (int; если 1 — текстура на unit 0 в `uTexture`).
Всё остальное (камера, свет, тени, uShininess, uSpecStrength) выставляет `main.cpp`.
Один и тот же `draw()` вызывается и в depth-проходе (там нужен только `uModel`; остальные uniform-ы игнорируются).
Поверхности должны быть замкнутыми боксами с нормалями наружу и обходом CCW (включён back-face culling).

## Арман (src/game, src/ui)
```cpp
// game/AppState.h
enum class AppState { Menu, Hall, Preview, Confirm, Purchased };
enum class UiAction { None, BuyTicket, Select, Buy, Back };                      // (+)

// game/Game.h (+)
class Game {
  AppState state() const;  int selectedSeat() const;  const Ticket* ticket() const;
  void apply(UiAction, SeatGrid&);      // нажата кнопка: переходы, Booking, ticket.txt, setTaken
  void seatPicked(int id, SeatGrid&);   // HALL -> PREVIEW (луч попал в свободное кресло)
  void cameraArrived();                 // PREVIEW -> CONFIRM (камера доехала до места)
  bool back(SeatGrid&);                 // Esc; false, если уже MENU (main закрывает окно)
};

// ui/UI.h (+)  — вызывается между ImGuiLayer::beginFrame() и render()
namespace ui { struct Hud { team, topic, fps, shadowsOn, shadowRes, paused, helpVisible, orbit,
                            vsync, biasTexels, pcfKernel, benchmark };
               UiAction draw(const Game&, const SeatGrid&, const Hud&); }
```
Шрифт с кириллицей ImGui грузит сам `ImGuiLayer` (Segoe UI / Arial на Windows).
Если кладёте свой шрифт — `assets/fonts/DejaVuSans.ttf` + лицензия в `LICENSES.md`.

## Алихан (src/core) — что отдаёт остальным
`Shader` (setInt/Float/Vec3/Mat4), `Texture` (`loadTexture2D("assets/x.png")`, ошибки в консоль),
`Camera::centerRay()`, `Camera::setFirstPerson(eyePos, lookAt)` (плавный переход ~0.9 с).

## Клавиши (в README и F1; к плану добавлены O, [ ], K, C, V, F5, F6)
WASD+мышь, ЛКМ, T, Q, стрелки, P, F1/H, Esc — по плану.
O — orbit-обзор зала; `[` `]` — bias; K — ядро PCF 1×1/3×3/5×5; C — culling depth-прохода;
V — vsync; F5 — автозамер FPS (пишет `fps_results.md`); F6 — перезагрузка шейдеров без перезапуска.

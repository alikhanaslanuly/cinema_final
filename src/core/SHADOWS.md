# Shadow mapping — шпаргалка для отчёта (раздел 3–4) и защиты

## Пайплайн
1. **Pass 1 (depth)** `ShadowMap::begin()` → FBO с одной depth-текстурой (24 бит, NEAREST, border = 1.0).
   `depth.vert` кладёт `gl_Position = uLightSpace * uModel * aPos`, `depth.frag` пустой.
2. **Pass 2 (main)** `blinn_phong.vert/frag` + `shadow.glsl`: Blinn-Phong по прожектору, множитель `(1 - shadow)` на прямой свет.
Выключение клавишей T пропускает pass 1 целиком (так считается FPS «без теней»).

## Формулы (ровно как в `shaders/core/shadow.glsl`)
* Источник — прожектор под потолком; матрица света `P_L · V_L`, `P_L` — перспектива с fov = 2·60° + 6°, near 0.3, far 60.
* Положение в карте: `p = (P_L V_L · x_world).xyz / w · 0.5 + 0.5`.
* Линеаризация глубины (м): `lin(d) = 2·n·f / (f + n − (2d−1)(f − n))`.
* Тест: `shadow = lin(p.z) − bias > lin(texture(map, p.xy).r)`.
* **bias** (в метрах, slope-scaled): `bias = texel · (uBiasTexels + (r + 0.5)·min(tan θ, 5))`,
  `texel = 2·tan(fov/2)/res · lin(p.z)` — размер одного тексела карты на этой дистанции,
  `cos θ = N·L`, `r` — радиус PCF.
* **PCF**: `shadow = (1/(2r+1)²) · Σ_{i,j=−r..r} [ lin(p.z) − bias > lin(map(p.xy + (i,j)/res)) ]`; r = 1 → 3×3.
* Вне усечённой пирамиды света (`p∉[0,1]`) — «не в тени» (прожектор там всё равно не светит: конус 60° внутри fov 126°).

## Что можно «вживую» поменять на защите
| Что | Как |
|---|---|
| bias | клавиши `[` `]` (шаг 0.25 тексела) или `uBiasTexels` в `main.cpp` |
| размер карты | Q (512 ↔ 2048) или `ShadowMap::kLow/kHigh` |
| PCF-ядро | K (1×1 / 3×3 / 5×5) |
| направление/позиция света | стрелки; границы в `Light::configure` |
| culling depth-прохода | C (back ↔ front) |
| шейдер без перезапуска | править `shaders/core/*`, нажать F6 |

## Типичные артефакты
* **Shadow acne** (полосы на освещённых поверхностях) → bias мал: `]` или C.
* **Peter-panning** (тень «отрывается» от кресла) → bias велик: `[`.
* **Блоки/ступеньки** на краях при 512 → Q (2048) или K (5×5).

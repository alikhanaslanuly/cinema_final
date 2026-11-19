# dev_stubs — temporary placeholders (NOT part of the final project)

Minimal implementations of the modules owned by Kuanysh (scene, picking) and Arman (game, ui),
written to the agreed interfaces (see src/core/CONTRACT.md). They let Alikhan build and test
core / shadows / main.cpp before the teammates push their code.

CMake rule: if `src/<module>/` contains at least one `.cpp`, the real module is built and the
stub of that module is ignored. Real headers always win over stub headers (src/ is first in the
include path). When all four modules exist in src/, delete this folder.

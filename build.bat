@echo off
rem One-command build on Windows 10 (Visual Studio 2019/2022 + CMake in PATH)
cmake -S . -B build || exit /b 1
cmake --build build --config Release || exit /b 1
echo.
echo Build OK. Run: build\bin\Release\cinema3d.exe

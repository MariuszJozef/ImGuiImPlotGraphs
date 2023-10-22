# ImPlot interactive graphs

This demo exemplifies how to use ImPlot (in conjunction with ImGui and SDL2 backend) to plot interactive graphs in a C++ program.

For a beginners example of how to use in a C++ project just ImGui without ImPlot, please refer to:
[SDLImGuiTriangle](https://github.com/MariuszJozef/SDLImGuiTriangle.git).

```
git clone --depth 1 https://github.com/MariuszJozef/ImGuiImPlotGraphs.git
cd ImGuiImPlotGraphs
```

```
cmake --preset Make-Gnu-Debug [ -D SDL2_DIR=/path/to/SDL2Config.cmake/ ]
cmake --build --preset Make-Gnu-Debug
cmake --build --preset Make-Gnu-Debug --target run
```

```
cmake --preset Make-Clang-Debug [ -D SDL2_DIR=/path/to/SDL2Config.cmake/ ]
cmake --build --preset Make-Clang-Debug
cmake --build --preset Make-Clang-Debug --target run
```

```
cmake --preset NMake-Msvc-Debug [ -D SDL2_DIR=/path/to/SDL2Config.cmake/ ]
cmake --build --preset Make-Msvc-Debug
cmake --build --preset Make-Msvc-Debug --target run
```

**N.B.**

- On Windows 11 it is preferable to issue the commands from **"Developer Command Prompt for VS 2022"** instead of regular PowerShell or CMD because the former has predefined Visual Studio environment variables which enable compilation with VS compiler.
- If nevertheless using PowerShell/CMD, or on Windows 10, one must first run some appropriate *.bat scripts that define VS environment variables.


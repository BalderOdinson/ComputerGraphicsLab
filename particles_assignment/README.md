# Particles
The assignment was to implement particle effect. Calculation of the particle's position is done on compute shader for better performance. The example shows 100k particles falling from the top of the fountain and 1 million particles moving on the bottom of the fountain.

![Particles animation](https://media.giphy.com/media/XdJTcWIKUPLKEHk5R7/source.gif)

## Build instructions

Sample also requires assets contained in this repository. Application assumes that assets folder is one directory back from the active directory. If that's not the case move assets to desired location.

### Windows

Dependencies
- CMake 3.14+
- Visual Studio 2017 or above

Generate Visual Studio 2017 project:
```
cmake -G"Visual Studio 15 2017 Win64" -H. -Bbuild
```
or Visual Studio 2019 project:
```
cmake -G"Visual Studio 16 2019" -A x64 -H. -Bbuild
```

Build it with Visual Studio or with following CMake command:
```
cmake --build build --config Release --target Particles
```

### Linux

Dependencies
- CMake 3.14+
- C++17 Compiler(tested on gcc 9.1 and clang 9)

Generate project:
```
cmake -G "Unix Makefiles" -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
```

Build the project:
```
cmake --build build --config Release --target Particles -- -j4
```

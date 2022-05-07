# C++ Compiler
Simple C++ like compiler using Bison/Flex

We use C++ 17, Bison >= 3.0 because we use it to generate C++ files and use C++ variants instead of `%union`

# How To Build:
## Dependencies
  * Cmake >= 3.0
  * C++ compiler
  * Bison >= 3.0
  * Flex

## Install Dependencies
  * Ubuntu: 
    ```bash
        sudo apt-get update
        sudo apt-get install build-essential bison flex cmake gcc g++
    ```

  * Windows: 
    * [CMake](https://cmake.org/download/)
    * C++ Compiler: You can use [Visual Studio](https://visualstudio.microsoft.com/downloads/) or [mingw-w64](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download)
    * You can get new version of bison [from here](https://github.com/lexxmark/winflexbison/releases/tag/latest), extract the zip, and add the directory to the `PATH` environment variable
    * if your are using [chocolatey package manager](https://chocolatey.org/install), install bison/flex using
      ```
      choco install winflexbison3
      ```


## Build
  * Ubuntu: 
    ```bash
          mkdir build
          cd build
          cmake .. -DCMAKE_BUILD_TYPE=Release
          cmake --build . --config Release --clean-first
    ```
  You will find executable called `CppCompiler` inside `build` directory

  Generated C++ grammar files inside `build/src/grammar/` directory

------------------------------------------------------------------------------------

  * Windows:
    * Using Visual Studio:
      ```cmd
          mkdir build
          cd build
          cmake .. -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64
          cmake --build . --config Release --clean-first
      ```

      Visual Studio it is the default generator for windows, (i.e you can remove `-G "Visual Studio 17 2022"` and cmake will choose the installed version) but you can select a specific version:

      - "Visual Studio 11 2012"
      - "Visual Studio 14 2015"
      - "Visual Studio 15 2017"
      - "Visual Studio 16 2019"
      - "Visual Studio 17 2022"

      You will find executable called `CppCompiler.exe` inside `build/Release/` directory

      Generated C++ grammar files inside `build/src/grammar/` directory

    * Using Mingw-w64:
        ```cmd
            mkdir build
            cd build
            cmake .. -DCMAKE_BUILD_TYPE=Release -G "Mingw-w64 GCC 8"
            cmake --build . --config Release --clean-first
        ```

      You will find executable called `CppCompiler.exe` inside `build` directory

      Generated C++ grammar files inside `build/src/grammar/` directory

# C++ files

## Structure

``` text
├── CMakeLists.txt
├── cmake
│   ├── CMake modules
│   └── ...
├── doc
│   └── CMakesLists.txt
│   └── Doxyfile
├── include
│   ├── DynArray.h
│   └── ...
├── src
│   └── ...
└── test
    ├── CMakeLists.txt
    ├── test.cpp
    └── ...
```

## Software Requirements

## Building

First, clone this repo:

```shell
git clone --recursive https://github.com/<your github username>/<your repository name>
```

Now ideally use VSCode with [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) which will read from [CMakeUserPresets.json](CMakeUserPresets.json) and give you a nice toolbar on the bottom of the VSCode window to switch between configurations. Alternatively you can build the project manually on the command line:

- Executable

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXECUTABLE=ON ..
cmake --build . --config Release
cd bin
./<your project name>.exe
```

- Unit testing

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target unit_tests
ctest -C Debug -VV
```
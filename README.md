# GraphicLib

> Simple X11 graphics engine with physics.

## Requirements

- A compiler that supports C++20 (e.g. [GCC](https://gcc.gnu.org/), [Clang](https://clang.llvm.org/), etc.)
- [CMake](https://cmake.org/) 3.20 or higher
- [X11](https://www.x.org/wiki/) development libraries

## Getting started

- Clone the repository

```bash
$ git clone https://github.com/Sid110307/GraphicLib.git
$ cd GraphicLib
```

- Configure and build

```bash
$ cmake -S . -B bin
$ cmake --build bin --target all -j4
```

- Run the executable

```bash
$ ./bin/graphicLib
```

## License

[MIT](https://opensource.org/licenses/MIT)

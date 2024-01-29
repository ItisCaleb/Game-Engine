# Some Game

## prerequisite

* g++ (gcc/mingw...)

## install libraries

### Windows
After cloned
```sh
$ ./vcpkg/bootstrap-vcpkg.bat
$ ./vcpkg/vcpkg.exe install sdl2 sdl2-image nlohmann-json sdl2-ttf --triplet=x64-mingw-dynamic --host-triplet=x64-mingw-dynamic
```

### MacOS(Apple Silicon)
```sh
$ ./vcpkg/bootstrap-vcpkg.sh
$ ./vcpkg/vcpkg install sdl2 sdl2-image nlohmann-json sdl2-ttf --triplet=arm64-osx-dynamic --host-triplet=arm64-osx-dynamic
```

## compile

### Windows
```sh
$ mingw32-make.exe run
```

### MacOS(Apple Silicon)
```sh
$ make run
```
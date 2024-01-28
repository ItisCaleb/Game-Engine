# Some Game

## prerequisite

* g++ (gcc/mingw...)

## install librarys

After cloned
```sh
$ ./vcpkg/bootstrap-vcpkg.bat
$ ./vcpkg/vcpkg.exe install sdl2 sdl2-image nlohmann-json sdl2-ttf --triplet=x64-mingw-dynamic --host-triplet=x64-mingw-dynamic
```

## compile

Windows
```sh
$ mingw32-make.exe run
```
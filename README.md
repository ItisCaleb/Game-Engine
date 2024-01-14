# Some Game

## prerequisite

* g++ (gcc/mingw...)

## install librarys

After cloned
```sh
$ git submodule update
$ ./vcpkg/bootstrap-vcpkg.bat
$ ./vcpkg/vcpkg.exe install sdl2 sdl-image
```

## compile

Windows
```sh
$ mingw32-make.exe run
```
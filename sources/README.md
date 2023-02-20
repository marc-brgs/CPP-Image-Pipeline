# CPP Image Pipeline (v0.X)

The following will decribe the procudures to be able to construct, compile and build project with edited sources scripts.

## Dependencies

### MinGW-W64 GCC-8.1.0 : x86_64-posix-seh

*Download link : https://sourceforge.net/projects/mingw-w64/files/*

Reference it to `Path` environnement variable :  
C:\...\mingw64\bin

### CMake

*Download link : https://cmake.org/download/*

Reference it to `Path` environnement variable :  
C:\...\CMake\bin

### OpenCV – 3.4.X

*Download link : https://opencv.org/releases/*

Reference it to `Path` environnement variable :  
C:\...\opencv\build\x64\mingw\bin  
C:\...\opencv\build\x64\mingw\lib

### Special mingw builder for OpenCV

Add mingw folder of the following link to C:\...\opencv\build\x64\mingw\build\x64 (next to vc14 and vc15 builders) :

*https://drive.google.com/file/d/1SflymUxuZESJE0G_T8ZVABC3wNwt1V-j/view*


## Installation check

You do not need to reboot your computer after installation.  
```
g++ --version
cmake --version
```

## Terminal commands

- Construct project  
```
cmake . -G "MinGW Makefiles"
```

- Build  
```
mingw32-make.exe
```

- Execute  
```
.\image_pipeline_app
```
*Note : You cannot construct project structure over a different CMakeCache.txt, remove it first*

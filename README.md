# CPP Image Pipeline (v0.1)

The following will decribe the procedures to be able to construct, compile and build project with edited sources scripts on Windows.

## Dependencies

### MinGW-W64 GCC-8.1.0 : x86_64-posix-seh

*Download link : https://sourceforge.net/projects/mingw-w64/files/*

Reference it to `Path` environnement variable of system as following :  
C:\...\mingw64\bin

### CMake

*Download link : https://cmake.org/download/*

Reference it to `Path` environnement variable of system as following :  
C:\...\CMake\bin

### OpenCV – 3.4.1X

*Download link : https://opencv.org/releases/*

Reference it to `Path` environnement variable of system as following :  
C:\...\opencv\build\x64\mingw\bin  
C:\...\opencv\build\x64\mingw\lib

### Special mingw builder for OpenCV

Add mingw folder of the following link to C:\...\opencv\build\x64\mingw\build\x64 (next to vc14 and vc15 builders) :

*https://drive.google.com/file/d/1SflymUxuZESJE0G_T8ZVABC3wNwt1V-j/view*


## Installation check

You may need to reboot your computer after CMake installation to make it work.  
```
g++ --version
cmake --version
```

## Terminal commands

- Construct project  
```
cmake . -G "MinGW Makefiles"
```

- Build (compile)  
```
mingw32-make.exe
```

- Execute  
```
.\image_pipeline_app
```
*Note : You cannot construct project structure over a different CMakeCache.txt, remove it first*

## Pipeline steps available commands

#### load
```
load <image_path> as <image_variable>
```  
*Example :* `load image.png as img1`

#### load_mask
```
load_mask <image_path> as <image_variable>
```  
*Example :* `load_mask mask.png as mask1`

#### save
```
save <image_variable> as <image_path>
```  
*Example :* `save img1 as result`

#### reset
```
reset <image_variable>
```  
*Example :* `reset img1`

#### brightness
```
brightness <image_variable> <value>
```  
*Example :* `brightness img1 40`

#### contrast
```
contrast <image_variable> <value>
```  
*Example :* `contrast img1 40`

#### saturate
```
saturate <image_variable> <value>
```  
*Example :* `saturate img1 40`

#### threshold
```
threshold <image_variable> <value>
```  
*Example :* `threshold img1 180`

#### invert
```
invert <image_variable>
```  
*Example :* `invert img1`

#### horizontal_flip
```
horizontal_flip <image_variable>
```  
*Example :* `horizontal_flip img1`

#### vertical_flip
```
vertical_flip <image_variable>
```  
*Example :* `vertical_flip img1`

#### rotate_90
```
rotate_90 <image_variable>
```  
*Example :* `rotate_90 img1`

#### rotate
```
rotate <image_variable> <angle> <center_x> <center_y>
```  
*Example :* `rotate img1 45 0 0`

#### crop
```
crop <image_variable> <top> <left> <bottom> <right>
```  
*Example :* `crop img1 100 100 200 200`

#### translate
```
translate <image_variable> <x> <y>
```  
*Example :* `translate img1 40 40`

#### resize
```
resize <image_variable> <width> <height>
```  
*Example :* `resize img1 200 200`

#### blur
```
blur <image_variable> <radius>
```  
*Example :* `blur img1 5`

#### erode
```
erode <image_variable> <radius>
```  
*Example :* `erode img1 5`

#### dilate
```
dilate <image_variable> <radius>
```  
*Example :* `dilate img1 5`

#### add
```
add <image_variable1> <image_variable2>
```  
*Example :* `add img1 img2`

#### diff
```
diff <image_variable1> <image_variable2>
```  
*Example :* `diff img1 img2`

#### product
```
product <image_variable1> <image_variable2>
```  
*Example :* `product img1 img2`

#### screen
```
screen <image_variable1> <image_variable2>
```  
*Example :* `screen img1 img2`

#### product
```
overlay <image_variable1> <image_variable2>
```  
*Example :* `overlay img1 img2`

#### binary_merge
```
binary_merge <image_variable1> <image_variable2> <mask_variable>
```  
*Example :* `binary_merge img1 img2 mask1`

#### weighted_merge
```
weighted_merge <image_variable1> <image_variable2> <mask_variable>
```  
*Example :* `weighted_merge img1 img2 mask1`




## Pipeline steps file complete example

```
load images/antoine-griezmann.png as img1
load images/cat-cry.jpg as img2
load images/harry-potter-akimbo.png as img3
load images/uganda-army.png as img4
load_mask images/mask.png as mask1
load_mask images/mask2.png as mask2


brightness img1 40
save img1 as images/result1.png
brightness img2 -100
invert img2
reset img2
contrast img1 100
saturate img1 -100
horizontal_flip img1
vertical_flip img2
rotate_90 img1
//crop img4 50 300 350 650
invert img2
add img1 img2
//diff img4 img3
product img4 img3
translate img2 100 50

resize img3 200 200
blur img3 5
erode img3 5
dilate img2 5
save img2 as images/result2.png

reset img1
reset img2

screen img2 img1
overlay img1 img2

reset img1
reset img2

binary_merge img3 img2 mask1
brightness mask2 80
weighted_merge img2 img1 mask2
resize img2 200 200
rotate img2 45 100 100
threshold img2 180
```

*Note 1 : // can be used to comments lines but they should be place at the 2 first characters of the line*  
*Note 2 : Line jumps are allowed*

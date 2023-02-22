# canvas-drawer

Implements a simple drawing api
![avatar](/images/my_art_work1.png)

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

## Supported primitives
```
1. Points
```
Usage: need one vertex to be specified 
![avatar](/images/points.png)
```
2. Lines
```
Usage: need two vertices to be specified
![avatar](/images/lines.png)
```
3. Triangles
```
Usage: need three vertices to be specified
![avatar](/images/triangles.png)
```
4. Circles
```
Usage: need the center vertex and the radius to be specified
![avatar](/images/circles.png)

```
5. Rectangles
```
Usage: need two vertices to be specified
![avatar](/images/rectangles.png)

```
6. Stars
```
Usage: need the center vertex and the radius to be specified
![avatar](/images/stars.png)
```
7. Roses
```
Usage: need the center vertex, the radius, and the number of petals to be specified
![avatar](/images/flowers1.png)

## Results

![avatar](/images/my_art_work1.png)

![avatar](/images/bacteria.png)

![avatar](/images/my_art_work2.png)
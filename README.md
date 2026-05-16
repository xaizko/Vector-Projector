# Vector Projector

Vector Projector is a C++ desktop app for visualizing vector operations in 3D.  
It lets you enter two vectors, choose an operation, and see the resulting vector rendered in real time.

## Technologies Used

- **C++23** for application logic
- **raylib** for the window, rendering, 3D camera, and drawing
- **raygui** for the dropdown control in the UI
- **g++** as the compiler
- **Makefile** build workflow

## Features

- Interactive 3D vector visualization on a grid
- Two editable input vectors (`v1` and `v2`)
- Supported operations:
  - Addition
  - Subtraction
  - Dot product (component-wise result display)
  - Cross product
- Live result vector display in both:
  - Numeric form (`<x, y, z>`)
  - 3D rendered vector (purple)
- Color-coded vectors and legend:
  - Red: Vector 1
  - Blue: Vector 2
  - Purple: Result vector
- Axis guides and axis labels (`+X/-X`, `+Y/-Y`, `+Z/-Z`)
- Split UI layout:
  - Left control panel for vector input and operation selection
  - Right viewport for 3D scene
- Camera controls in viewport:
  - WASD to orbit
  - Mouse wheel to zoom

## Requirements

Before building, make sure you have:

- A C++ compiler with C++23 support (for example, `g++`)
- `make`
- `raylib` installed and linkable as `-lraylib`

## Build

From the project root:

```bash
make
```

This builds the executable at:

```bash
bin/vector_projection
```

## Run

```bash
./bin/vector_projection
```

## Clean Build Artifacts

```bash
make clean
```

## Project Structure

```text
src/
  main.cpp
  controller/
    operationEnums.cpp
    VectorCalculator.cpp
    VectorController.cpp
  view/
    ViewController.cpp
bin/
  vector_projection
Makefile
```

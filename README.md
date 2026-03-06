# CapOps Frontend

A Qt-based frontend application for visualizing and interacting with flight data on a grid-based map.

## Table of Contents

- [Features](#features)
- [Build instructions](#build-instructions)
- [Architecture](#architecture)
- [Project Structure](#project-structure)

## Features

- Map overlay with selectable sectors
- Multiple display modes
    - Risk state
    - Weather condition
    - Traffic load
    - None
- Tile-based map rendering



## Build instructions

#### Requirements:
- Qt 6.0 or later
- CMake 3.16 or later
- C++17 compatible compiler


#### Build:

Using CMake:
```
cmake -S . -B build && cmake --build build/
```
#### Run:
```
./build/app
```


## Architecture

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="./docs/AppWindow-dark.png">
  <source media="(prefers-color-scheme: light)" srcset="./docs/AppWindow-light.png">
  <img src="./AppWindow-light.png" alt="uml diagram">
</picture>


## Project Structure
```
include/
├── domain/
│   ├── Coordinates.hpp
│   ├── DisplayMode.hpp
│   └── SectorStates.hpp
│
├── helpers/
│   ├── IMapFetcher.hpp
│   └── MapFetcherCARTO.hpp
│
├── pages/
│   └── MainPage.hpp / .cpp
│
├── panels/
│   ├── StateGridPanel
│   ├── SectorDetailsPanel
│   └── OperatorPanel
│
├── widgets/
│   ├── GridSector
│   └── SegmentedControl
│
└── AppWindow

src/
│
├── ...
│
└── ...
```
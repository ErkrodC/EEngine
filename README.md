![EEngine demo](EEngine.gif)

# EEngine

A personal C++ game engine project focused on learning modern engine architecture and graphics programming.

The project originally began following the **Hazel** engine (from The Cherno’s game engine series) but has since diverged significantly in structure and implementation. The engine now explores modern C++ features, modular architecture, and custom systems built from scratch.

This repository is primarily for **self-education** rather than production use.

---

## Overview

The engine is a modular C++23 codebase implementing a lightweight rendering framework. It includes both **2D and 3D rendering pipelines**, a **custom entity component system**, and organization with **modern C++ modules**.

Core goals of the project:

- renderer design and graphics programming 
- Explore modern C++ language features (C++23)
- Build engine subsystems from scratch
- networking and multiplayer

---

## Key Features

### Modern C++

- **C++ Modules** for compile-time isolation and dependency reduction
- **Concepts** for better type safety and compile-time error detection
- **std::expected** for error handling

### Rendering

- **OpenGL renderer**
- **2D rendering system**
    - sprite batching
    - orthographic camera
- **3D rendering system**
    - mesh rendering
    - perspective camera

### Engine Systems

- **Custom ECS (Entity Component System)**
    - sparse set component storage model
- **Scene system**
- **Input handling**
- **Window and platform layer**

---

## Current Status

This project is **experimental**.

Systems are frequently rewritten as new ideas are explored.
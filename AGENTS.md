# Shooter Project - Quick Start Guide

## Build & Run
```bash
make && ./run
```
- Makefile is authoritative (not CMake)
- SFML 2.5.1+ required at `/home/alexandr/libraries/SFML-2.5.1/lib/`
- C++11 required (`-std=c++11`)

## Architecture

### Entry Point
- `main.cpp` → `Application.run()` → 60 FPS loop (`sf::seconds(1.f/60.f)`)

### Key Classes
- `Application` - main loop coordinator, owns Window/EventHandler/Scene/GameState
- `GameState` - game logic, camera (sf::View), gravity, object updates
- `GameObjects` - container with `buttons`, `borders`, `all`, `background` vectors + `player` + `fabric`
- `CObject` - base class for all game objects (virtual methods: `update()`, `draw()`, `collide*`)
- `Fabric` - factory for creating objects (Bullet, Shooter, Crystal, etc.)

### Object Tree
```
CObject → Movable → Unit → Shooter (player)
CObject → Bullet, Crystal, Heart, Ammunition, Platform
CObject → Ground (background)
CObject → Button, Text (UI)
```

### Map System
- Tiled XML format via TinyXML2 (included in `include/tinyxml/`)
- Level files in `resources/levels/`
- Generates background tiles and interactive objects from XML

## Resources
- Images: `resources/images/*.png` (texture atlases)
- Audio: `resources/audio/*.wav`
- Fonts: `resources/fonts/arial.ttf`
- Levels: `resources/levels/*.tmx` (Tiled) / `*.xml`

## Platform
- Linux Ubuntu 18.04+ (Windows/Mac untested)
- Fullscreen buggy in SFML < 2.5.1 with multi-monitor

## Conventions
- All objects inherit from `CObject`
- New objects go under `src/objects/{backgrounds,interactive,interfaces,units}/`
- UI components in `src/ui/`
- Core systems in `src/core/` (Application, GameState, etc.)

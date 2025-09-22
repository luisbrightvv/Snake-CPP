# 🐍 Snake C++ Edition

Welcome to my neon take on the classic Snake. It is a clean C++17 codebase that keeps the gameplay logic tidy and uses **SFML 3** (falls back to **SFML 2.5** automatically) for windowing, rendering, and input. The project has a full start screen, animated food, rainbow snake body, and an arcade HUD styled with the Press Start 2P font.

## 🎥 Quick Look
<video src="docs/Registrazione%20Snake%20Game.mov" controls width="640" muted loop></video>

- The recording shows the start screen flow, dynamic snake colors, pulsing food animation, live score updates, and the restart loop after Game Over

## ✨ Features
- Start, Playing, and Game Over states with keyboard-driven flow (`SPACE` to start/restart, `ESC` to quit)
- Scoreboard and prompts rendered with an arcade-style font (with automatic system fallbacks)
- Snake head in soft green, segments cycling rainbow hues as you move and grow
- Food represented as a pulsing red circle that snaps to the 20 px grid
- Smooth 60 FPS loop, movement cadence driven by a configurable timer
- Dark gray background for an OLED-friendly vibe

## 🛠 Requirements
- C++17 toolchain (Clang, GCC, or MSVC)
- [SFML 3](https://www.sfml-dev.org/) headers & libraries
  - Works out of the box with SFML 2.5 as well (`find_package(SFML 3 ...)` falls back to `find_package(SFML 2.5 ...)`)
- CMake 3.10+

### macOS setup (Homebrew)
```bash
brew install sfml
```

### Fonts / Assets
- Arcade font lives in `assets/fonts/PressStart2P.ttf`
  - The runtime first looks for `assets/fonts/PressStart2P.ttf` (ideal: copy/rename the file there)
  - If missing, it tries the parent/build-relative paths and finally falls back to common system fonts (Arial, Courier New, Verdana)

## 🚀 Build & Run
```bash
mkdir -p build
cd build
cmake ..
cmake --build .
./snake
```

The executable looks for assets relative to both the project root and the build directory, so running from `build/` works without extra copying.

## 🎮 Controls
| Key | Action |
| --- | --- |
| Arrow keys | Change direction (opposite turns are ignored) |
| Space | Start from the title screen or restart after Game Over |
| Escape / Close window | Quit the game |

## 🧱 Project Structure
```
Snake/
├── CMakeLists.txt
├── README.md
├── assets/
│   └── fonts/
│       └── PressStart2P.ttf
├── docs/
│   └── Registrazione Snake Game.mov
└── src/
    ├── Food.cpp
    ├── Game.cpp
    ├── Snake.cpp
    └── main.cpp
```
- `Game.cpp` orchestrates states, UI text, input, and rendering
- `Snake.cpp` owns movement logic and the dynamic color palette
- `Food.cpp` spawns, animates, and draws the pulsing food
- `main.cpp` simply boots `Game::run()`

## 🧭 Roadmap Ideas
- [ ] Sound effects and subtle background music
- [ ] Multiple difficulties (faster tick rate, shrinking arena)
- [ ] Persistent high-score table stored in JSON
- [ ] Pause screen and settings menu
- [ ] Touch or gamepad support (SFML input event mapping)

## ❤️ Thanks
Thanks for checking out Snake C++ Edition! If you try a tweak or add a feature, drop the recording in `docs/`—I’d love to see it.

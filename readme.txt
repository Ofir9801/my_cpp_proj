# 🎮 Advanced World Explorer

> A feature-rich, 2-player cooperative grid puzzle adventure built in C++, featuring dynamic room navigation, state recording, and a highly flexible file-driven level design.

![C++](https://img.shields.io/badge/C%2B%2B-17%2B-blue?style=flat-square&logo=c%2B%2B)
![Paradigm](https://img.shields.io/badge/Paradigm-OOP-green?style=flat-square)
![Architecture](https://img.shields.io/badge/Architecture-Modular%20Engine-orange?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)

---

## 👥 Authors & Contact

* **Ofir Eren**
* **Liran Malka**

---

## ✨ Features

- **2-Player Co-op Engine** - Simultaneous keyboard input tracking for independent player mechanics.
- **Dynamic Level Architecture** - Rooms, riddles, and configuration profiles are parsed fully from external custom files.
- **State Recording & Playback** - CLI-driven macros to record gameplay frames and replay them deterministically.
- **Checkpoint System** - Built-in slot allocation allowing users to save and load active memory states mid-game.
- **Robust Exception Handling** - Structured engine-level error mitigation that intercepts corrupt layouts, alerts the user, and reloads safely without memory leaks.

---

## 🕹️ Controls & Setup

> ⚠️ **Important:** Keyboard input language must be set to **English** for the movement mapping and game logic to function correctly.

### Player Keybindings

| Action | Player 1 | Player 2 |
|--------|----------|----------|
| **Move Up** | `W` | `I` |
| **Move Down** | `S` | `K` |
| **Move Left** | `A` | `J` |
| **Move Right** | `D` | `L` |
| **Stay / Wait** | `E` | `O` |
| **Dispose Item** | `Q` | `U` |

---

## 🧠 Game Mechanics & Room Logic

- **Synchronized Transitions** - Players transition between rooms seamlessly as a single unit, spawning at the coordinating entrance threshold.
- **Switch & Key Cryptography** - Doors are uniquely mapped to matching switch/key IDs. Mechanics assume a balanced ecosystem (equal weights of switches, keys, and target doors).
- **Riddle Persistence Engine** - Resolved interactive puzzle states are cached visually. Retrying or colliding with a solved riddle renders the historic solution to assist with the endgame.
- **Inventory Scaling Logic** - Fully solving Room 2 rewards players with extended storage allocations. Overflow management triggers an explicit choice-driven item discard prompt.
- **Permadeath Game Over** - If a dynamic element (e.g. bomb blast) eliminates a critical progress dependency like a master key or map node, the engine halts state and terminates execution safely.

### Highlighted Rooms
- **Room 3 (The Dark Room)** - A sensory-blind maze. Player 1 handles a localized light field, while Player 2's torch is isolated. Progress requires coordinated demolition to unite players.
- **Room 4 (The Vault)** - The climax zone. Implements a multi-layered barrier matrix and a complex "Master Riddle" requiring a 3-digit key combination synthesized from historical room answers (Hint: `132`).

---

## 📁 Project Structure

```text
advanced-world-explorer/
├── Rooms/
│   ├── adv-world01.screen     # Room 1 grid design and parameters
│   ├── adv-world02.screen     # Room 2 grid design
│   └── adv-world03.screen     # Room 3 configuration (Dark Room)
├── Riddles/
│   ├── Riddles02.riddle       # Puzzle dataset for Room 2
│   └── Riddles04.riddle       # Master Riddle dataset
├── saves01/                   # Session recording: Defeat state sample
├── saves02/                   # Session recording: Mid-transition explosion loss
├── saves03/                   # Session recording: Successful victory state
├── src/                       # Source files (.cpp, .h modules)
└── README.md

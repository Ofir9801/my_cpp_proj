# 🎮 Advanced World Explorer (C++ Project)

Welcome to our C++ gaming project! This is a feature-rich, 2-player grid-based puzzle adventure game featuring dynamic room navigation, riddle solving, item management, and an advanced game recording/checkpoint system.

---

## 👥 Authors & Contact
* **Liran Malka** |
* **Ofir Eren** |

---

## 🛠️ Technical Details & Acknowledgments
* **Color Implementation:** Based on the color implementation guidelines from Keren Kalif's instructional video provided with the assignment.
* **AI Tools Utilization:** We utilized AI tools (Google Gemini and Microsoft Copilot integrated with Git) primarily to optimize our program structure and improve overall code readability.
* **Robust Error Handling:** Implemented comprehensive exception handling across the engine to prevent unexpected crashes. The program dynamically identifies specific runtime errors, alerts the user, and allows them to resolve the issue and reload safely.

---

## 🕹️ Controls & Setup

> ⚠️ **Important:** Please ensure your keyboard input language is set to **English** before playing; otherwise, movement controls will not function.

| Action | Player 1 | Player 2 |
| :--- | :---: | :---: |
| **Move Up** | `W` | `I` |
| **Move Down** | `S` | `K` |
| **Move Left** | `A` | `J` |
| **Move Right** | `D` | `L` |
| **Stay / Wait** | `E` | `O` |
| **Dispose Item** | `Q` | `U` |

---

## 🧠 Game Mechanics & Room Design

* **Door & Room Navigation:** Door numbers correspond directly to room numbers (e.g., entering Door 2 always leads to Room 2). Players spawn at the location of the door they entered through, and can return to the previous room by stepping back into the same door. When transitioning between rooms, both players move together.
* **Switches & Keys Puzzle:** Every switch is linked to a specific door via a unique ID. In puzzle rooms, players must activate the designated switch and unlock the door using a key. Dynamic in-game messages guide the players through the required actions.
* **Design Assumption:** The map design assumes a balanced ecosystem, meaning an equal number of switches and keys relative to the number of doors.
* **Riddle Persistence:** Solved riddles remain visible on the screen. Bumping into a previously solved riddle will display the answer again to assist players later in the game.
* **Inventory Rewards:** Completing all riddles in Room 2 rewards players with an extra inventory slot. If players need to discard an item, a prompt will appear asking them to select which item to remove.
* **Game Over Logic:** If an explosion or bomb destroys a critical item required to progress (such as an essential key or door), the game will automatically alert the user and terminate safely.

### Special Rooms Showcase
* **Room 3 (The Dark Room):** Designed as a "dark room" maze. Player 1 spawns next to a torch, but the second torch is blocked. Players must strategically plant a bomb to reach the second torch and clear the room. *(Note: Any room can be configured as a dark room via the design file).*
* **Room 4 (The Vault):** The final challenge. It involves destroying multiple obstacles and solving a unique "Master Riddle." This riddle requires players to combine three separate digits (answers gathered from riddles in previous rooms) into a final code. *(Hint: If you get stuck, the correct master code is '132').*

---

## 📁 File Structures & Custom Formats

### 1. Riddle Files (`.riddle`)
Each riddle file is named according to its room number (e.g., `Riddles02.riddle`) and must be saved in the designated folder using the following structural format:
```text
Riddle: <riddle question>
Index of correct Answer: <number of the correct answer>
4 possible answers: <options[0:3]>

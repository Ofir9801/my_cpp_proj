Names of the students:
Liran Malka, id: 211626783
Ofir Eren, id: 323958082

 - Color Implementation: We based the color implementation on Keren Kalif's video, which was provided with the assignment.
 - AI Tools: We utilized AI tools (Google Gemini and Microsoft Copilot integrated with Git) primarily to optimize our program structure and improve code readability.
 - Controls: We updated the player keybindings for better usability. The current controls are:
Player 1: Right (D), Down (S), Up (W), Left (A), Stay (E), Dispose (Q)
Player 2: Right (L), Down (K), Up (I), Left (J), Stay (O), Dispose (U)

- Input Language: Please ensure your keyboard input language is set to English; otherwise, movement controls will not function.
- Game Over Logic: If a bomb destroys an item required to progress (such as an essential door or key), the game will alert the user and terminate.

Game Mechanics & Room Design:
- Door Logic: In this exercise, we linked door numbers to room numbers. For example, to enter Room 2 from Room 1, you must open the door marked with the digit '2'.
- Switches: Every switch is linked to a specific door via its ID; therefore, a specific switch can only open its corresponding door.
- Switch/Key Puzzle: In rooms containing switches, players must activate the designated switch linked to the door and unlock it with a key. In-game messages will guide the player on the required actions.
- Design Assumption: The room design assumes an equal number of switches and keys relative to the number of doors.
- Navigation: We implemented a system where entering a specific door number always leads to the corresponding room number (e.g., Door 2 always leads to Room 2).
- Room 3 (The Dark Room): Room 3 is designed as a "dark room" maze. Player 1 spawns next to a torch, but the second torch is blocked. the players must plant a bomb for reaching the second torch.
(you can set every room as dark, we choose room 3 for example)
- Riddle Persistence: Solved riddles remain on screen. Bumping into a solved riddle displays the answer again to assist players with the Vault Room.
- Room 4 (The Vault): We created a "Vault" room as a final challenge. It involves destroying multiple obstacles and solving a unique "Master Riddle." This riddle requires players to combine three digits (answers from riddles in previous rooms) into a final code.
	Hint: If you are having trouble answering, the correct code is '132'.
- Inventory Reward: Completing all riddles in Room 2 rewards the players with an extra inventory slot. If players need to discard an item, a prompt will ask them to select which item to remove.
- Room Transitions: When transitioning between rooms, both players move to the selected room together. Players spawn at the location of the door they entered through. You can return to the previous room by entering the door you just came from.

File Structures & Formats:
- Riddle File Structure:

Riddle: <riddle question>
Index of correct Answer: <number of the correct answer>
4 possible answers: <options[0:3]>
(Note: Each riddle file is named according to its room number, e.g., Riddles02.riddle, and must be saved in the designated folder.)

Room Design Files:
- New room designs must be saved in the Rooms folder using the naming convention adv-worldXX.screen (where XX represents the room number, e.g., adv-world03.screen).
- The file must contain the letter 'L' for the Legend (must be 3 lines). The 'L' character must be in the leftmost position of the line.
- if you want to set the room as a dark room, include the character 'D' in the leftmost position of the line in the start of the file (before the 'L' line).
- if you want to set the room that when the players answer all the riddles in the room correctly they get extra inventory slot, include the character 'E' in the leftmost position of the line in the start of the file (before the 'L' line).
- Map size: 80x22 (plus 3 extra lines for the legend).
- This structure follows the guidelines discussed in the forum: https://mama.mta.ac.il/mod/forum/discuss.php?d=3446

Save & Load Features:
- Save Recording: Added a function to save the game recording by entering -save when running from the command line.
- Load Recording: Added a function to replay the last saved game by entering -load from the command line.
- Silent Mode: Added a "Silent Mode" for auto-play (shows end result only) by entering -load -silent from the terminal.
- Exceptions: We implemented robust exception handling to prevent crashes. The program identifies specific errors, alerts the user, and allows them to fix the issue and reload the game.
- Save Slots: Added the option to save up to 10 distinct game states.
- To Load: In the menu, press '3' to load a specific checkpoint.
- To Save: During gameplay, pause the game and press 's'.
Note: When running in -save mode, the user cannot launch previous games; only new games can be started.

we added 3 folders of recoreded games for example:
saves01 - example of game the user loss
saves02 - example of game the user loss while transfer between rooms and explode objects
saves03 - example of game the user win 
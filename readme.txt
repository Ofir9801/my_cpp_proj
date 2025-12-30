Names of the students:
Liran Malka, id: 211626783
Ofir Eren, id: 323958082

- We based the color addition on the video of keren Kalif that you add to the assignment
- We used ai tools - gemini by google and copilot by Microsoft that was integrated with git. we used it mostly to efficient our program structure and readability.
- We changed the key binds for players for more comfortable using, now it is:
	player 1: right - D, down - S, up - W, left - A, stay - E, dispose - Q
	player 2: right - L, down - K, up - i, left - J, stay - O, dispose - U
- In case of the bomb destroying a required item to continue the game(like essential door or key), the game will alert the user and end the game.
- In this exercise we linked the door number to the room number and there so for entry room 2 from room 1, you need to open door with the digit 2.
- Every switch is linked to spefic door by his Id so only one switch can open its linked door.
- In a room that there are switches in it, you need to turn on the designated switch that linked to the door and also unlock with a key. the player will get messages through the game that will help him understand what he need to do.
- Make sure the keyboard is set to english otherwise you won't be able to move.
- The room design is based on the assumption that there is equal numbers of switches and keys to the number of doors.
- We made it so you can advance to EACH room from EACH room by entering the door with the room's number, for example. door 2 always lead to room 2.
- Room 3 is the dark one. Player 1 is spawned next to a torch and the second player's torch is blocked. Player 1 needs to plant a bomb to assist player 2 to reach his torch. This room is very challenging as its filled with walls and obstacles.
- The riddles remain after on screen after answering correctly and bumping into it states the correct answer, for vault purposes.
- We made a 'Vault' room as the 4th room with difficult challenge to solve - exploding many obstacles and solving one last great riddle to finish the challenge.
- this is unique riddle that requires players to collect 3 digits from the previous rooms(riddle answers) and combine them to one final answer.
  * If you are having a tough time answering, the correct answer is '132'.

- the stracture of the riddle txt is: notice that each riddle txt is named according to the room number it belongs to, for example room2riddle.txt and saved in designated folder
	Riddle: <riddle question>
	index of correct Answer: <number of the correct answer>
	4 possible answers:<options[0:3]>

- the stracture of the room.txt is: notice that each room txt is named according to the room number it belongs to, for example Room2.txt and saved in designated folder
	L - for legend (must be 3 lines)
	<room design> - by the size of 80x22 (legend is 3 lines extra)




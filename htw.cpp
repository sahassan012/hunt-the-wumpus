/*
*
* Description: Navigate a maze to kill the Wumpus.
* Implementation of a 1972 game by Gregory Yob.
*
*/

#include <iostream>
#include <iomanip>   // for setw()
#include <cctype>    // for toupper()
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // to seed random number generator with time(0)
#include <sstream>
#include <string>
#include <iomanip>
#include <stdio.h>

using namespace std;

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

//initializing the counter in the for loop that will keep track of the number of user attempts
int i = 1;

//initializing the moveNumber to be consistent with int i that tracks the for loop
int moveNumber = i;

//Declaring the struct that defines what each node should entail with a pointer to the next node
typedef struct node
{
	int moveNumber;
	int personLoc;
	int wumpusLoc;
	int bats1Loc;
	int bats2Loc;
	int pit1Loc;
	int pit2Loc;
	int arrowLoc;
	node* next;
}*nodePtr;

//Declaring and initializing the head, current, and temporary pointers to NULL value
nodePtr head = NULL;
nodePtr curr = NULL;
nodePtr temp = NULL;

//
//
//  AddNode:
//
void AddNode(int moveNumber, int personLoc, int wumpusLoc, int bats1Loc, int bats2Loc, int pit1Loc, int pit2Loc, int arrowLoc)
{
	//create a pointer n and have it point to a new nodePtr
	nodePtr n = new node;

	//find the node n is pointing to, access its next element and set the 'next' element to NULL
	n->next = NULL;

	//find the node n is pointing to, acces its data and set it equal to given value
	n->moveNumber = moveNumber;
	n->personLoc = personLoc;
	n->wumpusLoc = wumpusLoc;
	n->bats1Loc = bats1Loc;
	n->bats2Loc = bats2Loc;
	n->pit1Loc = pit1Loc;
	n->pit2Loc = pit2Loc;
	n->arrowLoc = arrowLoc;

	//If the list already exists, have curr pointer point to what head is pointing to
	//So if the list exists, we want the newly node created to be connected at the end of the list
	if (head != NULL) {
		curr = head;
		//and as long as it is not the end of the list, store current value as the next value
		while (curr->next != NULL)
		{
			curr = curr->next;
		}
		//Once it is at the end of the list, point the next element to n, which is the new node with data
		curr->next = n;
	}
	//If we don't have atleast one element in the list, tell the new node that it will be the front of the list
	else
	{
		head = n;
	}

}

//
//
//  DeleteNode:
//
void DeleteNode()
{
	//Create a node pointer and have it point to NULL(nothing)
	nodePtr delPtr = NULL;
	nodePtr prev = NULL;

	//Set temp and the current pointers to the head pointer
	temp = head;
	curr = head;

	//If the head pointer points to NULL, the list is empty, so display that message
	if (head == NULL) {
		cout << "List Empty" << endl;
	}

	//Otherwise, if the node that head is pointing to has a next pointer that is pointing to NULL
	//Then have delPtr point to the node I want to delete
	//Set the head to NULL and break the delPtr from the linked nodes or list
	else if (head->next == NULL) {
		delPtr = head;
		head = NULL;
		free(delPtr);
	}

	//Otherwise have delPtr point to the node to be deleted
	//While the node that delPtr is pointing to, has a next pointer that is not NULL
	//Set previous pointer to the delPtr and the new delPtr value to the next pointer
	else {
		delPtr = head;
		while (delPtr->next != NULL) {
			prev = delPtr;
			delPtr = delPtr->next;
		}
		//now set the next pointer from which the previous pointer exists to NULL
		//And break delPtr out of the node chain
		prev->next = NULL;
		free(delPtr);
	}

}

//
//
//  displayCave:
//
void displayCave()
{
    	   cout << "       ______18______             \n"
		<< "      /      |       \\           \n"
		<< "     /      _9__      \\          \n"
		<< "    /      /    \\      \\        \n"
		<< "   /      /      \\      \\       \n"
		<< "  17     8        10     19       \n"
		<< "  | \\   / \\      /  \\   / |    \n"
		<< "  |  \\ /   \\    /    \\ /  |    \n"
		<< "  |   7     1---2     11  |       \n"
		<< "  |   |    /     \\    |   |      \n"
		<< "  |   6----5     3---12   |       \n"
		<< "  |   |     \\   /     |   |      \n"
		<< "  |   \\       4      /    |      \n"
		<< "  |    \\      |     /     |      \n"
		<< "  \\     15---14---13     /       \n"
		<< "   \\   /            \\   /       \n"
		<< "    \\ /              \\ /        \n"
		<< "    16---------------20           \n"
		<< endl;
}//end displayCave()

//
//
//  displayInstructions:
//
void displayInstructions()
{
	   cout << "Hunt the Wumpus:                                             \n"
		<< "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
		<< "room has 3 tunnels leading to other rooms.                   \n"
		<< "                                                             \n"
		<< "Hazards:                                                     \n"
		<< "1. Two rooms have bottomless pits in them.  If you go there you fall and die.  \n"
		<< "2. Two other rooms have super-bats.  If you go there, the bats grab you and    \n"
		<< "   fly you to some random room, which could be troublesome.  Then those bats go\n"
		<< "   to a new room different from where they came from and from the other bats.  \n"
		<< "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and   \n"
		<< "   is too heavy for bats to lift.  Usually he is asleep.  Two things wake      \n"
		<< "   him up: Anytime you shoot an arrow, or you entering his room.  When he      \n"
		<< "   wakes he moves if he is in an odd-numbered room, but stays still otherwise. \n"
		<< "   After that, if he is in your room, he eats you and you die!                 \n"
		<< "                                                                               \n"
		<< "Moves:                                                                         \n"
		<< "On each move you can do the following, where input can be upper or lower-case: \n"
		<< "1. Move into an adjacent room.  To move enter 'M' followed by a space and      \n"
		<< "   then a room number.                                                         \n"
		<< "2. Shoot your guided arrow through a list of up to three adjacent rooms, which \n"
		<< "   you specify.  Your arrow ends up in the final room.                         \n"
		<< "   To shoot enter 'S' followed by the number of rooms (1..3), and then the     \n"
		<< "   list of the desired number (up to 3) of adjacent room numbers, separated    \n"
		<< "   by spaces. If an arrow can't go a direction because there is no connecting  \n"
		<< "   tunnel, it ricochets and moves to the lowest-numbered adjacent room and     \n"
		<< "   continues doing this until it has traveled the designated number of rooms.  \n"
		<< "   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You \n"
		<< "   automatically pick up the arrow if you go through a room with the arrow in  \n"
		<< "   it.                                                                         \n"
		<< "3. Enter 'R' to reset the person and hazard locations, useful for testing.     \n"
		<< "4. Enter 'C' to cheat and display current board positions.                     \n"
		<< "5. Enter 'D' to display this set of instructions.                              \n"
		<< "6. Enter 'P' to print the maze room layout.                                    \n"
		<< "7. Enter 'X' to exit the game.                                                 \n"
		<< "                                                                               \n"
		<< "Good luck!                                                                     \n"
		<< " \n"
		<< endl;
}//end displayInstructions()

struct killTheWumpus
{
	              int maze[21][3] = { {0,0,0 }, //Room 0 is ignored
					  {2,5,8 }, //Room 1 connections
					  {1,3,10},//Room 2 connections
					  {2,4,12}, //Room 3 connections
					  {3,5,14}, //Room 4 connections
					  {1,4,6 }, //Room 5 connections
					  {5,7,15}, //Room 6 connections
					  {6,8,17}, //Room 7 connections
					  {1,7,9}, //Room 8 connections
					  {8,10,18}, //Room 9 connections
					  {2,9,11}, //Room 10 connections
					  {10,12,19}, //Room 11 connections
					  {3,11,13}, //Room 12 connections
					  {12,14,20}, //Room 13 connections
					  {4,13,15}, //Room 14 connections
					  {6,14,16}, //Room 15 connections
					  {15,17,20}, //Room 16 connections
					  {7,16,18}, //Room 17 connections
					  {9,17,19}, //Room 18 connections
					  {11,18,20}, //Room 19 connections
					  {13,16,19}, //Room 20 connections
	};

	//intializing all locations for hazards, player, wumpus, and arrow
	int personLoc;
	int wumpusLoc;
	int bats1Loc;
	int bats2Loc;
	int pit1Loc;
	int pit2Loc;
	int arrowLoc;
	bool userHasArrow = true;

	//This function will check and if the rooms adjacent to the players have any hazards and display a message
	void adjacentRoomCheck() {
		//Checks if there is a wumpus in adjacent rooms of player's location
		if ((maze[wumpusLoc][0] == personLoc) || (maze[wumpusLoc][1] == personLoc) || (maze[wumpusLoc][2] == personLoc)) {
			cout << " You smell a stench.";
		}

		//Checks if player is in an adjacent room OF one of the rooms in which the pits are present
		if ((maze[pit1Loc][0] == personLoc) || (maze[pit1Loc][1] == personLoc) || (maze[pit1Loc][2] == personLoc) ||
			(maze[pit2Loc][0] == personLoc) || (maze[pit2Loc][1] == personLoc) || (maze[pit2Loc][2] == personLoc)) {
			cout << "You feel a draft. ";
		}

		//Checks if player is in an adjacent room OF one of the rooms in which the bats are present
		if ((maze[bats1Loc][0] == personLoc) || (maze[bats1Loc][1] == personLoc) || (maze[bats1Loc][2] == personLoc) ||
			(maze[bats2Loc][0] == personLoc) || (maze[bats2Loc][1] == personLoc) || (maze[bats2Loc][2] == personLoc)) {
			cout << "You hear rustling of bat wings. ";
		}
		cout << endl;
	}

	//This function is called when a user enters letter 'm' or 'M'
	void movePlayer() {
		//this is where the user is prompted to enter room #
		int room;
		//cout << "Enter room to move player to:" << endl;
		cin >> room;

		//If statement that makes sure that the player is moving only through adjacent rooms
		if ((room == maze[personLoc][0]) || (room == maze[personLoc][1]) || (room == maze[personLoc][2])) {
			//if the user enters an invalid room (Less than 1 or more than 20), this code asks the user to enter a correct room number
			//this sets the personLoc value to the value provided
			if ((room > 20) || (room < 1)) {
				cout << "Enter a room between 1-20" << endl;
			}

			//When you enter a room with the Wumpus in it, 
			//if that room is odd-numbered then the Wumpus will move to the lowest-numbered adjacent room.  
			//Else it will snap your neck and the game is over!

			//if the user enters a room with the wumpus
			else if (room == wumpusLoc) {
				//sets room value to person's location and displays message before quiting 
				//this value will check if the room entered is an even 
				int remainder = room % 2;
				//If the room number entered is even, the wumpus kills you
				if (remainder == 0) {
					personLoc = room;
					cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
					cout << "It is over." << endl;
					cout << endl;
					exitGame();
				}
				//If the room number entered is odd, the wumpus relocates in to the lowest numbered adjacent room
				else {
					wumpusLoc = maze[wumpusLoc][0];
					personLoc = room;
					cout << "You hear a slithering sound, as the Wumpus slips away." << endl;
					cout << "Whew, that was close!" << endl;
				}
			}

			//If the user enters a room with a pit, message is displayed and program calls exitGame function to exit
			else if ((room == pit1Loc) || (room == pit2Loc)) {
				personLoc = room;
				cout << "Aaaaaaaaahhhhhh.... " << endl;
				cout << "    You fall into a pit and die." << endl;
				exitGame();
			}

			//If the user enters a room with bats, message is displayed and bats relocate the player
			else if (room == bats1Loc) {
				//statement to make sure that the bats relocate the player to a room that the bats were not in before 
				//AND to a room that the other bats are not in
				do {
					personLoc = (rand() % 20) + 1;
				} while ((personLoc == bats2Loc) || (personLoc == bats1Loc));

				//statement to make sure that the bats will relocate to a room that does isn't the player's room, where they were before, or where the other bats are
				do {
					bats1Loc = (rand() % 20) + 1;
				} while ((bats1Loc == personLoc) || (bats1Loc == bats2Loc) || (bats1Loc == room));
				//Displaying the message when player enters a room with bats
				cout << "Woah... you're flying! You've just been transported by bats to room " << personLoc << endl;
			}

			//If the user enters the second room with bats
			else if (room == bats2Loc) {
				//statement to make sure that the bats relocate the player to a room that the bats were not in before 
				//AND to a room that the other bats are not in
				do {
					personLoc = (rand() % 20) + 1;
				} while ((personLoc == bats1Loc) || (personLoc == bats2Loc));

				//statement to make sure that the bats will relocate to a room that does isn't the player's room, where they were before, or where the other bats are
				do {
					bats2Loc = (rand() % 20) + 1;
				} while ((bats2Loc == personLoc) || (bats2Loc == bats1Loc) || (bats2Loc == room));
				//Displaying the message when player enters a room with bats
				cout << "Woah... you're flying! You've just been transported by bats to room " << personLoc << endl;
			}

			//If player enters a room with the arrow, the value for userHasArrow changes to true and arrow location changes to -1
			else if (room == arrowLoc) {
				//Message is displayed
				//Bool value for userHasArrow turns to true and arrow location value to -1
				cout << "Congratulations, you found the arrow and can once again shoot." << endl;
				userHasArrow = true;
				personLoc = arrowLoc;
				arrowLoc = -1;
			}
			else {
				//if neither of the previous if statements are met then, the room entered simply becomes person's location
				personLoc = room;
			}
		}
		else {
			//If the player enters a room above 20 or less than 1
			//Value for game instant 'i' is decreased by one. This prevents the try from being counted
			cout << "Invalid move.  Please retry. " << endl;
			i = i - 1;
		}
	}

	//This function generates random location for player, wumpus, hazards, and arrow
	void distinctRandLocs() {
		//check if every value is distinct for every possibility, and if user input is valid (between 1-20)
		//initialize arrowLoc to -1 as we are to allow the player an arrow at the beginning of the game
		arrowLoc = -1;
		//bats1 location is randomized and set to a number between 1-20 that is anything but bats2 location
		bats1Loc = (rand() % 20) + 1;
		do {
			bats2Loc = (rand() % 20) + 1;
		} while (bats2Loc == bats1Loc);

		//pit1 location is randomized and set to a number between 1-20 that is anything but bats1 location and bats2 location
		do {
			pit1Loc = (rand() % 20) + 1;
		} while ((pit1Loc == bats1Loc) || (pit1Loc == bats2Loc));

		//pit2 location is randomized and set to a number between 1-20 that is anything but bats1, bats2, and pit1 location
		do {
			pit2Loc = (rand() % 20) + 1;
		} while ((pit2Loc == bats1Loc) || (pit2Loc == bats2Loc) || (pit2Loc == pit1Loc));

		//wumpus location is randomized and set to a number between 1-20 that is anything but bats1, bats2, pit1, and pit2 location
		do {
			wumpusLoc = (rand() % 20) + 1;
		} while ((wumpusLoc == bats1Loc) || (wumpusLoc == bats2Loc) || (wumpusLoc == pit1Loc) || (wumpusLoc == pit2Loc) || (wumpusLoc == arrowLoc));

		//person location is randomized and set to a number between 1-20 that is anything but bats1, bats2, pit1, pit2, and person location
		do {
			personLoc = (rand() % 20) + 1;
		} while ((personLoc == bats1Loc) || (personLoc == bats2Loc) || (personLoc == pit1Loc) || (personLoc == pit2Loc) || (personLoc == wumpusLoc));

	}

	void shootArrow() {
		//If the user chooses to shoot an array, the program checks if the user has an arrow
		//If the user does not have an arrow, it will display a message
		if (userHasArrow == false) {
			cout << "Sorry, you can't shoot an arrow you don't have.  Go find it." << endl;
		}

		//If the user has the arrow then, user is allowed to choose rooms to shoot through
		else {
			//User is prompted to enter # of rooms
			//Initializing limit of rooms and the 3 different possible rooms
			cout << "Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: " << endl;
			int rmLimit, shootRm1, shootRm2, shootRm3;
			cin >> rmLimit;

			//Statement that will handle what happens if the user chooses shoot room limit as 1
			if (rmLimit == 1) {
				//Player enters in room 1
				if (cin >> shootRm1) {
					//If the location of room 1 matches any of the rooms adjacent to person's room
					if ((shootRm1 == maze[personLoc][0]) || (shootRm1 == maze[personLoc][1]) || (shootRm1 == maze[personLoc][2])) {
						//AND if the location of room 1 matches the wumpus's location
						if (shootRm1 == wumpusLoc) {
							//Displays victory message, and calls exit game function
							cout << "Wumpus has just been pierced by your deadly arrow!" << endl;
							cout << "Congratulations on your victory, you awesome hunter you. " << endl;
							exitGame();
						}
						//If all previous if statements are unmet, then user has arrow value becomes false, and the arrow lands in the intended room
						//The wumpus also moves into the least numbered adjacent room
						else {
							userHasArrow = false;
							arrowLoc = shootRm1;
							wumpusLoc = maze[wumpusLoc][0];
							//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
							if (wumpusLoc == personLoc) {
								cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
								cout << "It is over." << endl;
								exitGame();
							}
						}
					}

					//If the location of room 1 does not match any of the rooms adjacent to person's room
					else if ((shootRm1 != maze[personLoc][0]) || (shootRm1 != maze[personLoc][1]) || (shootRm1 != maze[personLoc][2])) {
						//Arrow ricochets to the lowest numbered adjacent room of room 1
						//Wumpus moves to the lowest numbered adjacent room from its own location and User has arrow value changes to false 
						arrowLoc = maze[personLoc][0];
						userHasArrow = false;
						cout << "Room " << shootRm1 << " is not adjacent.  Arrow ricochets..." << endl;

						//If the arrow ricochets back to the player, he dies
						if (personLoc == arrowLoc) {
							cout << "You just shot yourself, and are dying." << endl;
							cout << "It didn't take long, you're dead." << endl;
							exitGame();
						}
						//If the arrow ricochets and lands in a room where the wumpus is, you kill the wumpus and the game calls exit game function
						else if (wumpusLoc == arrowLoc) {
							cout << "Your arrow ricochet killed the Wumpus, you lucky dog!" << endl;
							cout << "Accidental victory, but still you win!" << endl;
							exitGame();
						}
						//Wumpus relocates only IF the arrow does not land in its room
						wumpusLoc = maze[wumpusLoc][0];
						//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
						if (wumpusLoc == personLoc) {
							cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
							cout << "It is over." << endl;
							exitGame();
						}

					}
				}
			}

			//Statement that will handle what happens if the user chooses shoot room limit as 2
			else if (rmLimit == 2) {
				//Player enters room 1 value
				if (cin >> shootRm1) {
					//if room 1 value is a room adjacent to the person's location
					if ((shootRm1 == maze[personLoc][0]) || (shootRm1 == maze[personLoc][1]) || (shootRm1 == maze[personLoc][2])) {
						//If room 1 is equal to the wumpus's location
						if (shootRm1 == wumpusLoc) {
							cout << "Wumpus has just been pierced by your deadly arrow!" << endl;
							cout << "Congratulations on your victory, you awesome hunter you. " << endl;
							exitGame();
						}
						//Else user loses arrow and the arrow lands in intended room
						//Wumpus DOES not move here because there is another room that the program anticipates the user will enter
						else {
							userHasArrow = false;
							arrowLoc = shootRm1;
						}
					}
					//If room 1 value isn't a room adjacent to the person's location
					else if (((shootRm1 != maze[personLoc][0]) || (shootRm1 != maze[personLoc][1]) || (shootRm1 != maze[personLoc][2]))) {
						arrowLoc = maze[personLoc][0];
						userHasArrow = false;
						cout << "Room " << shootRm1 << " is not adjacent.  Arrow ricochets..." << endl;

						//If the arrow ricochets back to the player, he dies
						if (personLoc == arrowLoc) {
							cout << "You just shot yourself, and are dying." << endl;
							cout << "It didn't take long, you're dead." << endl;
							exitGame();
						}
						//If the arrow ricochets, kills the wumpus on accident
						else if (wumpusLoc == arrowLoc) {
							cout << "Your arrow ricochet killed the Wumpus, you lucky dog!" << endl;
							cout << "Accidental victory, but still you win!" << endl;
							exitGame();
						}
						//Wumpus relocates only IF the arrow does not land in its room
						wumpusLoc = maze[wumpusLoc][0];
						//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
						if (wumpusLoc == personLoc) {
							cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
							cout << "It is over." << endl;
							exitGame();
						}
					}
					//If user enters in one rooms that is adjacent to user's room location
					//and enters in a second room that is adjacent to the first room
					if (cin >> shootRm2) {
						//If room 2 value matches an adjacent room to room 1
						if ((shootRm2 == maze[shootRm1][0]) || (shootRm2 == maze[shootRm1][1]) || (shootRm2 == maze[shootRm1][2])) {
							//And if room 2 is the same location as the wumpus's
							if (shootRm2 == wumpusLoc) {
								cout << "Wumpus has just been pierced by your deadly arrow!" << endl;
								cout << "Congratulations on your victory, you awesome hunter you. " << endl;
								exitGame();
							}
							//If user enters the second room to shoot as the room he is in, he dies
							else if (shootRm2 == personLoc) {
								//Displays message that you shot yourself
								cout << "You just shot yourself. " << endl;
								cout << "Maybe Darwin was right.  You're dead." << endl;
								exitGame();
							}
							//Otherwise, user loses arrow and it lands in the intended room and wumpus moves to the lowest numbered adjacent room from its loc
							else {
								userHasArrow = false;
								arrowLoc = shootRm2;
								wumpusLoc = maze[wumpusLoc][0];
								//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
								if (wumpusLoc == personLoc) {
									cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
									cout << "It is over." << endl;
									exitGame();
								}
							}
						}
						//If room 2 is NOT a room adjacent to room 1
						else if (((shootRm2 != maze[shootRm1][0]) || (shootRm2 != maze[shootRm1][1]) || (shootRm2 != maze[shootRm1][2]))) {
							//Arrow ricochets ricochets and the wumpus and the arrow relocates to the lowest numbered adjacent room
							//User no longer has arrow
							arrowLoc = maze[shootRm1][0];
							userHasArrow = false;
							cout << "Room " << shootRm2 << " is not adjacent.  Arrow ricochets..." << endl;

							//If the arrow ricochets back to the player, he dies
							if (personLoc == arrowLoc) {
								cout << "You just shot yourself, and are dying." << endl;
								cout << "It didn't take long, you're dead." << endl;
								exitGame();
							}
							//If the arrow ricochets and lands in a room with the wumpus, you win!
							else if (wumpusLoc == arrowLoc) {
								cout << "Your arrow ricochet killed the Wumpus, you lucky dog!" << endl;
								cout << "Accidental victory, but still you win!" << endl;
								exitGame();
							}
							//Wumpus relocates only IF the arrow does not land in its room
							wumpusLoc = maze[wumpusLoc][0];
							//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
							if (wumpusLoc == personLoc) {
								cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
								cout << "It is over." << endl;
								exitGame();
							}
						}
					}
				}
			}


			//Statement that will handle what happens if the user chooses shoot room limit as 3
			else if (rmLimit == 3) {
				//if user enters in only one room that is adjacent to user's room location
				if (cin >> shootRm1) {
					//If Room 1 is adjacent to person's location
					if ((shootRm1 == maze[personLoc][0]) || (shootRm1 == maze[personLoc][1]) || (shootRm1 == maze[personLoc][2])) {
						//If Room 1 is the wumpus's location
						if (shootRm1 == wumpusLoc) {
							cout << "Wumpus has just been pierced by your deadly arrow!" << endl;
							cout << "Congratulations on your victory, you awesome hunter you. " << endl;
							exitGame();
						}
						//If it doesn't hit the wumpus, it simply lands in the intended room and user loses the arrow
						else {
							userHasArrow = false;
							arrowLoc = shootRm1;
						}
					}
					//if room 1 is NOT equal to a room adjacent to the person's location
					else if (((shootRm1 != maze[personLoc][0]) || (shootRm1 != maze[personLoc][1]) || (shootRm1 != maze[personLoc][2]))) {
						//Arrow ricochets ricochets and the wumpus and the arrow relocates to the lowest numbered adjacent room
						//User no longer has arrow
						arrowLoc = maze[personLoc][0];
						userHasArrow = false;
						cout << "ShootRm1 " << shootRm1 << " is not adjacent.  Arrow ricochets..." << endl;

						//If the arrow ricochets back to the player, you die
						if (personLoc == arrowLoc) {
							cout << "You just shot yourself, and are dying." << endl;
							cout << "It didn't take long, you're dead." << endl;
							exitGame();
						}
						//If the arrow ricochets and lands in a room with the wumpus, you win!
						else if (wumpusLoc == arrowLoc) {
							cout << "Your arrow ricochet killed the Wumpus, you lucky dog!" << endl;
							cout << "Accidental victory, but still you win!" << endl;
							exitGame();
						}
						//Wumpus relocates only IF the arrow does not land in its room
						wumpusLoc = maze[wumpusLoc][0];
						//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
						if (wumpusLoc == personLoc) {
							cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
							cout << "It is over." << endl;
							exitGame();
						}
						return;
					}
					//If user enters in one rooms that is adjacent to user's room location
					//and enters in a second room that is adjacent to the first room
					if (cin >> shootRm2) {
						//If room 2 is a room adjacent to room 1
						if ((shootRm2 == maze[shootRm1][0]) || (shootRm2 == maze[shootRm1][1]) || (shootRm2 == maze[shootRm1][2])) {
							//If room 2 is wumpus's location
							if (shootRm2 == wumpusLoc) {
								cout << "Wumpus has just been pierced by your deadly arrow!" << endl;
								cout << "Congratulations on your victory, you awesome hunter you. " << endl;
								exitGame();
							}
							//If user enters the second room to shoot as the room he is in, he dies
							else if (shootRm2 == personLoc) {
								//Displays message that you shot yourself
								cout << "You just shot yourself. " << endl;
								cout << "Maybe Darwin was right.  You're dead." << endl;
								exitGame();
							}
							//If you don't hit the wumpus, then you lose arrow and it lands in room 2
							else {
								userHasArrow = false;
								arrowLoc = shootRm2;
							}
						}
						//If room 2 is not adjacent to room 1
						else if (((shootRm2 != maze[shootRm1][0]) || (shootRm2 != maze[shootRm1][1]) || (shootRm2 != maze[shootRm1][2]))) {
							//Arrow ricochets ricochets and the wumpus and the arrow relocates to the lowest numbered adjacent room
							//User no longer has arrow
							arrowLoc = maze[shootRm1][0];
							userHasArrow = false;
							cout << "ShootRm2 " << shootRm2 << " is not adjacent.  Arrow ricochets..." << endl;

							//If the arrow ricochets back to the player, he dies
							if (personLoc == arrowLoc) {
								cout << "You just shot yourself, and are dying." << endl;
								cout << "It didn't take long, you're dead." << endl;
								exitGame();
							}
							//If the arrow ricochets and lands in a room with the wumpus, you win
							else if (wumpusLoc == arrowLoc) {
								cout << "Your arrow ricochet killed the Wumpus, you lucky dog!" << endl;
								cout << "Accidental victory, but still you win!" << endl;
								exitGame();
							}
							//Wumpus relocates only IF the arrow does not land in its room
							wumpusLoc = maze[wumpusLoc][0];
							//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
							if (wumpusLoc == personLoc) {
								cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
								cout << "It is over." << endl;
								exitGame();
							}
							return;
						}


						//If user enters in one rooms that is adjacent to user's room location
						//and enters in a second room that is adjacent to the first room
						//and enters in a third room that is adjacent to the second room
						if (cin >> shootRm3) {
							//If room 3 is adjacent to room 2
							if ((shootRm3 == maze[shootRm2][0]) || (shootRm3 == maze[shootRm2][1]) || (shootRm3 == maze[shootRm2][2])) {
								//If room 3 is where the wumpus is
								if (shootRm3 == wumpusLoc) {
									cout << "Wumpus has just been pierced by your deadly arrow!" << endl;
									cout << "Congratulations on your victory, you awesome hunter you. " << endl;
									exitGame();
								}
								//If user enters the third room to shoot as the room he is in, he dies
								else if (shootRm3 == personLoc) {
									//Displays message that you shot yourself
									cout << "You just shot yourself, and are dying." << endl;
									cout << "It didn't take long, you're dead. " << endl;
									exitGame();
								}
								//If you didn't get the wumpus
								else {
									wumpusLoc = maze[wumpusLoc][0];
									userHasArrow = false;
									arrowLoc = shootRm3;
									//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
									if (wumpusLoc == personLoc) {
										cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
										cout << "It is over." << endl;
										exitGame();
									}
								}
							}
							//If Room 3 is not adjacent room 2
							else if (((shootRm3 != maze[shootRm2][0]) || (shootRm3 != maze[shootRm2][1]) || (shootRm3 != maze[shootRm2][2]))) {
								//Arrow ricochets ricochets and the wumpus and the arrow relocates to the lowest numbered adjacent room
								//User no longer has arrow
								arrowLoc = maze[shootRm2][0];
								userHasArrow = false;
								cout << "Room " << shootRm3 << " is not adjacent.  Arrow ricochets..." << endl;

								//If the arrow ricochets back to the player, he dies
								if (personLoc == arrowLoc) {
									cout << "You just shot yourself, and are dying." << endl;
									cout << "It didn't take long, you're dead." << endl;
									exitGame();
								}
								//If the arrow lands in the wumpus's location
								else if (wumpusLoc == arrowLoc) {
									cout << "Your arrow ricochet killed the Wumpus, you lucky dog!" << endl;
									cout << "Accidental victory, but still you win!" << endl;
									exitGame();
								}
								//Wumpus relocates only IF the arrow does not land in its room
								wumpusLoc = maze[wumpusLoc][0];
								//If the wumpus moves into the same room as the player, as a result of missing, then he kills the player
								if (wumpusLoc == personLoc) {
									cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
									cout << "It is over." << endl;
									exitGame();
								}

							}

						}
					}

				}

			}

		}
	}

	//This function allows the player to set the distinct values for each location
	void resetLocations() {
		//temp values to store user input and check if they are each distinct
		int tempPersonLoc, tempWumpusLoc, tempBats1Loc, tempBats2Loc, tempPit1Loc, tempPit2Loc, tempArrowLoc;

		//Prompts the user to enter 7 room locations
		cout << "(Remember arrow value of -1 means it is with the person.)" << endl;
		cout << "Enter the 7 room locations (1..20) for player, wumpus, bats1, bats2, pit1, pit2, and arrow:" << endl;
		cin >> tempPersonLoc >> tempWumpusLoc >> tempBats1Loc >> tempBats2Loc >> tempPit1Loc >> tempPit2Loc >> tempArrowLoc;
		cout << endl;

		//check if every value is distinct for every possibility, and if user input is valid (between 1-20)
		//If and/or else statements below display a message if the user enters locations that are not distinct OR if they are more than 20 or less than 1
		if ((tempPersonLoc > 20) || (tempPersonLoc < 1) || (tempPersonLoc == tempWumpusLoc) || (tempPersonLoc == tempBats1Loc) || (tempPersonLoc == tempBats2Loc) || (tempPersonLoc == tempPit1Loc) || (tempPersonLoc == tempPit2Loc) || (tempPersonLoc == tempArrowLoc)) {
			cout << "Please enter distinct room numbers." << endl;
			cout << "Nothing was relocated." << endl;
		}
		else if ((tempWumpusLoc > 20) || (tempWumpusLoc < 1) || (tempWumpusLoc == tempBats1Loc) || (tempWumpusLoc == tempBats2Loc) || (tempWumpusLoc == tempPit1Loc) || (tempWumpusLoc == tempPit2Loc) || (tempWumpusLoc == tempArrowLoc)) {
			cout << "Please enter distinct room numbers." << endl;
			cout << "Nothing was relocated." << endl;
		}
		else if ((tempBats1Loc > 20) || (tempBats1Loc < 1) || (tempBats1Loc == tempBats2Loc) || (tempBats1Loc == tempPit1Loc) || (tempBats1Loc == tempPit2Loc) || (tempBats1Loc == tempArrowLoc)) {
			cout << "Please enter distinct room numbers." << endl;
			cout << "Nothing was relocated." << endl;
		}
		else if ((tempBats2Loc > 20) || (tempBats2Loc < 1) || (tempBats2Loc == tempPit1Loc) || (tempBats2Loc == tempPit2Loc) || (tempBats2Loc == tempArrowLoc)) {
			cout << "Please enter distinct room numbers." << endl;
			cout << "Nothing was relocated." << endl;
		}
		else if ((tempPit1Loc > 20) || (tempPit1Loc < 1) || (tempPit1Loc == tempPit2Loc) || (tempPit1Loc == tempArrowLoc)) {
			cout << "Please enter distinct room numbers." << endl;
			cout << "Nothing was relocated." << endl;
		}
		else if ((tempPit2Loc > 20) || (tempPit2Loc < 1) || (tempPit2Loc == tempArrowLoc)) {
			cout << "Please enter distinct room numbers." << endl;
			cout << "Nothing was relocated." << endl;
		}

		//If they are each distinct and betwen 1-20
		else {
			//Reassigns user input for person location to the maze  corresponding location on the maze
			if (tempPersonLoc == maze[tempPersonLoc - 1][0]) {
				personLoc = maze[tempPersonLoc - 1][0];
			}
			else if (tempPersonLoc == maze[tempPersonLoc - 1][1]) {
				personLoc = maze[tempPersonLoc - 1][1];
			}
			else if (tempPersonLoc == maze[tempPersonLoc - 1][2]) {
				personLoc = maze[tempPersonLoc - 1][2];
			}
			else if (tempPersonLoc == 1) {
				personLoc = maze[2][0];
			}

			//Reassigns user input for Wumpus location to the maze  corresponding location on the maze
			if (tempWumpusLoc == maze[tempWumpusLoc - 1][0]) {
				wumpusLoc = maze[tempWumpusLoc - 1][0];
			}
			else if (tempWumpusLoc == maze[tempWumpusLoc - 1][1]) {
				wumpusLoc = maze[tempWumpusLoc - 1][1];
			}
			else if (tempWumpusLoc == maze[tempWumpusLoc - 1][2]) {
				wumpusLoc = maze[tempWumpusLoc - 1][2];
			}
			else if (tempWumpusLoc == 1) {
				wumpusLoc = maze[2][0];
			}

			//Reassigns user input for Bats1 location to the maze corresponding location on the maze
			if (tempBats1Loc == maze[tempBats1Loc - 1][0]) {
				bats1Loc = maze[tempBats1Loc - 1][0];
			}
			else if (tempBats1Loc == maze[tempBats1Loc - 1][1]) {
				bats1Loc = maze[tempBats1Loc - 1][1];
			}
			else if (tempBats1Loc == maze[tempBats1Loc - 1][2]) {
				bats1Loc = maze[tempBats1Loc - 1][2];
			}
			else if (tempBats1Loc == 1) {
				bats1Loc = maze[2][0];
			}

			//Reassigns user input for Bats2 location to the maze corresponding location on the maze
			if (tempBats2Loc == maze[tempBats2Loc - 1][0]) {
				bats2Loc = maze[tempBats2Loc - 1][0];
			}
			else if (tempBats2Loc == maze[tempBats2Loc - 1][1]) {
				bats2Loc = maze[tempBats2Loc - 1][1];
			}
			else if (tempBats2Loc == maze[tempBats2Loc - 1][2]) {
				bats2Loc = maze[tempBats2Loc - 1][2];
			}
			else if (tempBats2Loc == 1) {
				bats2Loc = maze[2][0];
			}

			//Reassigns user input for Pit1 location to the maze corresponding location on the maze
			if (tempPit1Loc == maze[tempPit1Loc - 1][0]) {
				pit1Loc = maze[tempPit1Loc - 1][0];
			}
			else if (tempPit1Loc == maze[tempPit1Loc - 1][1]) {
				pit1Loc = maze[tempPit1Loc - 1][1];
			}
			else if (tempPit1Loc == maze[tempPit1Loc - 1][2]) {
				pit1Loc = maze[tempPit1Loc - 1][2];
			}
			else if (tempPit1Loc == 1) {
				pit1Loc = maze[2][0];
			}

			//Reassigns user input for Pit2 location to the maze corresponding location on the maze
			if (tempPit2Loc == maze[tempPit2Loc - 1][0]) {
				pit2Loc = maze[tempPit2Loc - 1][0];
			}
			else if (tempPit2Loc == maze[tempPit2Loc - 1][1]) {
				pit2Loc = maze[tempPit2Loc - 1][1];
			}
			else if (tempPit2Loc == maze[tempPit2Loc - 1][2]) {
				pit2Loc = maze[tempPit2Loc - 1][2];
			}
			else if (tempPit2Loc == 1) {
				pit2Loc = maze[2][0];
			}

			//Reassigns user input for arrow location to the maze corresponding location on the maze
			if (tempArrowLoc == maze[tempArrowLoc - 1][0]) {
				arrowLoc = maze[tempArrowLoc - 1][0];
			}
			else if (tempArrowLoc == maze[tempArrowLoc - 1][1]) {
				arrowLoc = maze[tempArrowLoc - 1][1];
			}
			else if (tempArrowLoc == maze[tempArrowLoc - 1][2]) {
				arrowLoc = maze[tempArrowLoc - 1][2];
			}
			else if (tempArrowLoc == 1) {
				arrowLoc = maze[2][0];
			}

			//If user entered -1 for arrowRoom, then userHasArrow value changes to true
			if (tempArrowLoc == -1) {
				userHasArrow = true;
			}
			//If user has entered a room number other than -1, the user does not have the arrow
			else {
				userHasArrow = false;
			}
		}
	}
	void displayLocations() {
		cout << "Cheating! Game elements are in the following rooms:" << endl;
		cout << "Player ";
		cout << "Wumpus ";
		cout << "Bats1 ";
		cout << "Bats2 ";
		cout << "Pit1 ";
		cout << "Pit2 ";
		cout << "Arrow " << endl;

		cout << "  " << personLoc;
		cout << left << setw(1) << "      " << wumpusLoc;
		cout << left << setw(1) << "      " << bats1Loc;
		cout << left << setw(1) << "     " << bats2Loc;
		cout << left << setw(1) << "   " << pit1Loc;
		cout << left << setw(1) << "    " << pit2Loc;
		cout << left << setw(1) << "    " << arrowLoc;
		cout << endl;
		cout << endl;
	}
	void displayInstruction() { displayInstructions(); }
	void printMaze() { displayCave(); }
	void exitGame() {
		cout << endl;
		cout << "Exiting Program ..." << endl;
		exit(0);
	}
};


//
//
//  main:
//
int main()
{
	// Seed the random number generator.  Change seed to time(0) to change output each time.
	srand(time(0));
	//srand(1);

	//Calling the struct for the Hunt the Wumpus game
	killTheWumpus p1;

	//Calling distinctRandLocs function to randomize each location
	p1.distinctRandLocs();

	//Declaring char for user input on move type
	//Declaring int moveNumber and initializing to 0 to handle 
	//an additional element that will be stored in each node
	char moveType;
	int moveNumber = 0;

	//After the start of the game, create the first node to store the random location values
	AddNode(moveNumber, p1.personLoc, p1.wumpusLoc, p1.bats1Loc, p1.bats2Loc, p1.pit1Loc, p1.pit2Loc, p1.arrowLoc);
	
	//Loop that will allow the user a given amount of tries, depending on integer i
	for (i = 1; i < MAX_LINE_LENGTH; i++)
	{
		//Storing i value into moveNumber as for loop advances
		moveNumber = i;

		//This bool variable will tell the program when it is okay to add a node and starts off as true
		bool addNodeCheck = true;

		//Takes user input and displays prompt accordingly
		cout << "You are in room " << p1.personLoc << ".";
		p1.adjacentRoomCheck();
		cout << endl;
		cout << i << ". Enter your move (or 'D' for directions): " << endl;
		cin >> moveType;

		//If user wants to move, this calls movePlayer function
		if ((moveType == 'M') || (moveType == 'm')) {
			p1.movePlayer();
		}
		//If user wants to shoot, this calls shootArrow function
		else if ((moveType == 'S') || (moveType == 's')) {
			p1.shootArrow();
		}
		//If user wants to reset location, this calls resetLocations function and restarts loop
		else if ((moveType == 'R') || (moveType == 'r')) {
			p1.resetLocations();

			//Resets the moveNumber and for loop
			i = 0;
		}
		//If user wants to display locations, this calls displayLocations function and keeps the previous loop value
		else if ((moveType == 'C') || (moveType == 'c')) {
			p1.displayLocations();
			//Decreases the moveNumebr and for loop advancement as displaying locations does not coutn as a move
			i = i - 1;
		}
		//If user wants to display instruction, this calls displayInstruction function and keeps the previous loop value
		else if ((moveType == 'D') || (moveType == 'd')) {
			p1.displayInstruction();
			i = i - 1;
			
			//This sets addNodeCheck variable to false as displaying instructions should not be a reason to add a node 
			addNodeCheck = false;
		}
		//If user wants to print maze, this calls printMaze function and keeps the previous loop value
		else if ((moveType == 'P') || (moveType == 'p')) {
			p1.printMaze();
			i = i - 1;
			//This sets addNodeCheck variable to false as printing the maze should not be a reason to add a node 
			addNodeCheck = false;
		}
		//If user wants to exit game, this calls exitGame function
		else if ((moveType == 'X') || (moveType == 'x')) {
			p1.exitGame();
		}
		//If user wants to exit game, this calls exitGame function
		else if ((moveType == 'U') || (moveType == 'u')) {
			//If i value or moveNumber is 1 or less than one, then program displays the message as you cannot undo at the beginning of the game
			if (i <= 1) {
				cout << "Sorry, you can't undo past the beginning of the game.  Please retry." << endl;
				//Setting moveNumber or i value back to 0
				i = 0;
			}
			//Otherwise, if i is greater than 1
			else {
				//Call DeleteNode function to delete the last node at the node list
				DeleteNode();
				//Here the program knows not to add a node 
				addNodeCheck = false;

				//Traverse the list with a while loop and print each curr value 
				//And advance to the data that the next curr value holds until it is NULL
				curr = head;
				
				//If curr value isn't NULL
				if (curr != NULL) {
					//Then as long as the next pointer from the current pointer isn't NULL
					//Have temp pointer point to curr pointer
					//Also have the next pointer from the temp pointer be the curr pointer
					while (curr->next != NULL) {
						temp = curr;
						curr = temp->next;
					}
					//Once that is done, store the values from the last node back into the game
					p1.personLoc = curr->personLoc;
					p1.wumpusLoc = curr->wumpusLoc;
					p1.bats1Loc = curr->bats1Loc;
					p1.bats1Loc = curr->bats1Loc;
					p1.pit1Loc = curr->pit1Loc;
					p1.pit2Loc = curr->pit2Loc;
					p1.arrowLoc = curr->arrowLoc;
				}
				//Move i value or moveNumber back 2 numbers as the move has been undone
				i = i - 2;	
			}
		}
		//if user does not choose any of the entries above, program displays a message
		else {
			cout << "Please make a valid entry." << endl;
			i = i - 1;
			addNodeCheck = false;
		}

		if ((addNodeCheck == true) && (moveType != 'C') && (moveType != 'c')) {
			//Call addNode function to create a new node to store the location value in it
			moveNumber = i;
			AddNode(moveNumber, p1.personLoc, p1.wumpusLoc, p1.bats1Loc, p1.bats2Loc, p1.pit1Loc, p1.pit2Loc, p1.arrowLoc);
		}
	}
	cout << "game over!" << endl;
}//end main()

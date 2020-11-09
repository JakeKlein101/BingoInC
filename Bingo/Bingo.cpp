#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 5
#define HIT 'X'
#define MAX_NAME_LEN 20
#define MIN_NUM 1
#define MAX_NUM 75
#define FALSE 0
#define TRUE !FALSE
#define PLAYERS 2

typedef struct bingoGame {
	int* numsDrawn;
	int numOfNumsDrawn;
	struct bingoPlayer* players;
	int numPlayers;
} bingoGame;
typedef struct bingoPlayer {
	char name[MAX_NAME_LEN];
	char board[BOARD_SIZE][BOARD_SIZE];
} bingoPlayer;

void initGame(bingoGame* game, int players);
void initBingoPlayer(bingoPlayer* pPlayer);
int updateGame(bingoGame* game, int num);
void updatePlayer(bingoPlayer* player, int num);
void printBoard(bingoPlayer player);
void printBoards(bingoGame game);
int hasWon(bingoPlayer player);
int drawNumber(int numsDrawn[], int len);
void gameLoop(void);


void initArr(int arr[], int len)
// initiates array to -1.
{
	int i = 0, j = 0;
	for (i = 0; i < len; i++)
	{
		arr[i] = -1;
	}
}

void initBingoPlayer(bingoPlayer* pPlayer)
{
/* This function initiates a bingo player struct.
It gets a pointer to a bingoPlayer structs and updates the name by input.
After that it initiates the player bingo board by putting random numbers in it.
*/
	int randomizedNum = 0;
	int i = 0, j = 0;
	int usedNumbersArray[MAX_NUM + 1];
	initArr(usedNumbersArray, MAX_NUM + 1);
	printf("What's your name?\n");
	fgets(pPlayer->name, MAX_NAME_LEN, stdin);
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			randomizedNum = drawNumber(usedNumbersArray, (BOARD_SIZE * BOARD_SIZE));
			pPlayer->board[i][j] = randomizedNum;
			usedNumbersArray[randomizedNum]++;
		}
	}
	printf("done initializing board\n");
}

void updatePlayer(bingoPlayer* player, int num)
{
/* This function updates the player details
It
and checks if this number is in the player board, if it is, it switches the place in the board to 'X'
Input: bingoPlayer struct pointer and number randomized
Output: None
*/
	int i = 0, j = 0;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if ((int)player->board[i][j] == num)
			{
				player->board[i][j] = HIT;
			}
		}
	}
}

void printBoard(bingoPlayer player)
{
	/* This function prints a player board
	Input: bingoPlayer
	Output: None
	*/
	int i = 0, j = 0;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (player.board[i][j] == HIT)
			{
				printf(" X  ");
			}
			else
			{
				printf(" %2d ", (int)player.board[i][j]);
			}
		}
		printf("\n\n");
	}
}

void initGame(bingoGame* game, int players)
{
	// initiates a bingo game object.
	game->numPlayers = players;
	game->players = (bingoPlayer*)malloc(sizeof(bingoPlayer) * players);
	game->numOfNumsDrawn = 0;
	game->numsDrawn = (int*)malloc(sizeof(int) * (MAX_NUM + 1));
	initArr(game->numsDrawn, MAX_NUM + 1);
}

int hasWon(bingoPlayer player) 
{
	// checks if player has a row of hits on his board.
	int i = 0, j = 0, vCounter = 0;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (player.board[i][j] == HIT)
			{
				vCounter++;
			}
			if (vCounter == 5)
			{
				return TRUE;
			}
		}
		if (vCounter < 5)
		{
			vCounter = 0;
		}
	}
	return FALSE;
}

int updateGame(bingoGame* game, int num)
{
	int i = 0;
	game->numsDrawn[num]++;
	game->numOfNumsDrawn++;
	for (i = 0; i < game->numPlayers; i++)
	{
		updatePlayer(&game->players[i], num);
	}
	for (i = 0; i < game->numPlayers; i++)
	{
		if (hasWon(game->players[i]))
		{
			return i;
		}
	}
	return -1;
}

void printBoards(bingoGame game)
{
/*
This function prints all players' boards
Input: bingoPlayer
Output: None
*/
	int i = 0;
	for (i = 0; i < game.numPlayers; i++)
	{
		printf("Board of %s: \n", game.players[i].name);
		printBoard(game.players[i]);
	}
}

int drawNumber(int numsDrawn[], int len) 
// draws random numbers that their index in the given list has value -1. 
{
	
	int i = 0;
	int num = 0;
	int duplicate = TRUE;
	while (duplicate)
	{
		num = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;	
		if (numsDrawn[num] == -1)
		{
			duplicate = FALSE;
		}
	}
	return num;
}

void gameLoop(void)
{
	srand(time(NULL));
	bingoGame game = {};
	bingoGame* pGame = &game;
	int drawnNumber = 0;
	int winnerIndex = 0;
	int i = 0;


	bingoPlayer playerOne = {};
	bingoPlayer playerTwo = {};
	bingoPlayer* pPlayerOne = &playerOne;
	bingoPlayer* pPlayerTwo = &playerTwo;

	initGame(pGame, PLAYERS);
	for (i = 0; i < pGame->numPlayers; i++)
	{
		initBingoPlayer(&(pGame->players[i]));
	}
	printBoards(game);
	int won = FALSE;
	while (!won)
	{
		drawnNumber = drawNumber(pGame->numsDrawn, pGame->numOfNumsDrawn);
		printf("The number that was drawn is: %d\n", drawnNumber);
		winnerIndex = updateGame(pGame, drawnNumber);
		if (winnerIndex != -1)
		{
			won = TRUE;
			printf("\n%s has won the game!\n", game.players[winnerIndex].name);
			printf("Here are the final boards:\n");
			printBoards(game);
		}
		else
		{
			printf("Here are the boards after the draw!\n");
			printBoards(game);
			printf("Press any key to roll another one!\n");
			getchar();
		}
	}
	free(pGame->numsDrawn);
	free(pGame->players);
}
int main(void)
{
	gameLoop();
}

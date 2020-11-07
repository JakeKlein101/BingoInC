#include <stdio.h>
#include <stdlib.h>

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
int generateRandomsNoRepeat(int duplicatesArray[][BOARD_SIZE]);
int drawNumber(bingoGame* game);


int generateRandomsNoRepeat(int duplicatesArray[][BOARD_SIZE]) 
// generates a random number between 1 - 75 that is also not in the 2d array given and returns it.
{
	int i = 0, j = 0;
	int num = 0;
	num = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (num == duplicatesArray[i][j])
			{
				generateRandomsNoRepeat(duplicatesArray);
			}
		}
	}
	return num;
}

void initArr(int arr[][BOARD_SIZE], int len)
// initiates array to -1.
{
	int i = 0, j = 0;
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len; j++)
		{
			arr[i][j] = -1;
		}
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
	int usedNumbersArray[BOARD_SIZE][BOARD_SIZE];
	initArr(usedNumbersArray, BOARD_SIZE);
	printf("What's your name?\n");
	fgets(pPlayer->name, MAX_NAME_LEN, stdin);
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			randomizedNum = generateRandomsNoRepeat(usedNumbersArray);
			pPlayer->board[i][j] = randomizedNum;
			usedNumbersArray[i][j] = randomizedNum;
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
	game->numsDrawn = (int*)malloc(sizeof(int) * MAX_NUM);
}

int hasWon(bingoPlayer player)
{
	// checks if player has a row of hits on his board.
	int i = 0, j = 0;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (player.board[j][i] == HIT)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

int updateGame(bingoGame* game, int num)
{
	int i = 0;
	game->numsDrawn[game->numOfNumsDrawn] = num;
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
	return FALSE;
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

int drawNumber(bingoGame* game)
{
	int i = 0;
	int num = 0;
	num = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;
	for (i = 0; i < game->numOfNumsDrawn; i++)
	{
		if (game->numsDrawn[i] == num)
		{
			drawNumber(game);
		}
	}
	return num;

}

int main(void)
{
	int drawnNumber = 0;
	int windex = 0;
	int i = 0;
	int won = FALSE;
	bingoGame game = {};
	bingoPlayer playerOne = {};
	bingoPlayer playerTwo = {};
	bingoPlayer* pPlayerOne = &playerOne;
	bingoPlayer* pPlayerTwo = &playerTwo;
	bingoGame* pGame = &game;
	initGame(pGame, PLAYERS);
	for (i = 0; i < pGame->numPlayers; i++)
	{
		initBingoPlayer(&(pGame->players[i]));
	}
	printBoards(game);
	while (!won)
	{	
		drawnNumber = drawNumber(pGame);
		printf("The number that was drawn is: %d", drawnNumber);
		windex = updateGame(pGame, drawnNumber);
		if (windex != FALSE)
		{
			won = TRUE;
			printf("%s has won the game!\n", game.players[windex].name);
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

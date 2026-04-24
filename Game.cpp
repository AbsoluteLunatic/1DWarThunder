#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define N 11
typedef enum GameObject
{
	WALL = -1,
	EMPTY,
	LTANK,
	RTANK,
	BREAKABLE_WALL
};
typedef struct Location
{
	int nLine,nCol;
}Location;
Location Directions[8] = {{-1,-1},{ -1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
Location Positions[2] = { {(N + 4) / 2,2},{(N + 4) / 2,N+1} };
int strike[][3] = { 0 };
GameObject Map[N + 4][N + 4] = { WALL };
void init_map(int *bWallCount)
{
	for (int i = 0; i < N+4; i++)
	{
		for (int j = 0; j < N+4; j++)
		{
			Map[i][j] = WALL;
		}
	}
	for (int i = 2; i < N + 2; i++)
	{
		for (int j = 2; j < N + 2; j++)
		{
			if (i == (N + 4) / 2)
			{
				if (j == 2)
					Map[i][j] = LTANK;
				else if (j == N + 1)
					Map[i][j] = RTANK;
				else if (j % 2)
				{
					Map[i][j] = BREAKABLE_WALL;
					(*bWallCount)++;
				}
				else
					Map[i][j] = EMPTY;
			}
			else if ((i % 2) && (j % 2))
			{
				Map[i][j] = BREAKABLE_WALL;
				(*bWallCount)++;
			}
			else
				Map[i][j] = EMPTY;
		}
	}
		
}
Location get_direction()
{
	int dir;
	Location Direction;
	scanf("%d", &dir);
	puts("");
	switch (dir)
	{
	case 1:
		Direction = Directions[1];
		break;
	case 2:
		Direction = Directions[4];
		break;
	case 3:
		Direction = Directions[6];
		break;
	case 4:
		Direction = Directions[3];
		break;
	default:
		puts("Invalid Direction.\n");
		exit(1);
	}
	return Direction;
}
Location get_direction(int *dir)
{
	Location Direction;
	scanf("%d", &(*dir));
	puts("");
	switch (*dir)
	{
	case 1:
		Direction = Directions[1];
		break;
	case 2:
		Direction = Directions[4];
		break;
	case 3:
		Direction = Directions[6];
		break;
	case 4:
		Direction = Directions[3];
		break;
	default:
		puts("Invalid Direction.\n");
		exit(1);
	}
	return Direction;
}
void turn_right(Location* Direction,int* dir)
{
	switch (*dir)
	{
	case 1:
		*Direction = Directions[4];
		(*dir)++;
		break;
	case 2:
		*Direction = Directions[6];
		(*dir)++;
		break;
	case 3:
		*Direction = Directions[3];
		(*dir)++;
		break;
	case 4:
		*Direction = Directions[1];
		*dir = 1;
		break;
	default:
		puts("Invalid Direction.\n");
		exit(1);
	}
}
void move(GameObject tank)
{
	int steps;
	puts("Enter the direction to move:\n");
	Location Direction = get_direction();
	puts("Enter the steps to take:\n");
	scanf("%d", &steps);
	puts("");
	while (steps-- && !Map[Positions[tank - 1].nLine + Direction.nLine][Positions[tank - 1].nCol +
		Direction.nCol])
	{
		Map[Positions[tank - 1].nLine][Positions[tank - 1].nCol] = EMPTY;
		Positions[tank - 1].nLine += Direction.nLine;
		Positions[tank - 1].nCol += Direction.nCol;
		Map[Positions[tank - 1].nLine][Positions[tank - 1].nCol] = tank;
	}
}
void shell(GameObject tank,GameObject *winner,int *bWallCount)
{
	puts("Enter the direction to shoot at:\n");
	Location Direction = get_direction();
	Location shell = Positions[tank - 1];
	while (!Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol])
	{
		shell.nLine += Direction.nLine;
		shell.nCol += Direction.nCol;
	}
	if (Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol] == RTANK)
	{
		*winner = LTANK;
	}
	if (Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol] != WALL)
	{
		Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol] = EMPTY;
		(*bWallCount)--;
	}
}
void switch_shell(GameObject tank, GameObject *winner, int *bWallCount)
{
	puts("Enter the starting direction to shoot at:\n");
	int dir;
	Location Direction = get_direction(&dir), shell = Positions[tank-1];
	int duration = 5;
	while ((!Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol] ||
		Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol] == WALL) && duration)
	{
		if (Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol] == WALL)
		{
			turn_right(&Direction, &dir);
			duration--;
		}
		shell.nLine += Direction.nLine;
		shell.nCol += Direction.nCol;
	}
	if (duration)
	{
		switch (Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol])
		{
		case RTANK:
			*winner = LTANK;
			break;
		case LTANK:
			*winner = RTANK;
			break;
		case BREAKABLE_WALL:
			(*bWallCount)--;
			break;
		}
		Map[shell.nLine + Direction.nLine][shell.nCol + Direction.nCol] = EMPTY;
	}
}
void airstrike(GameObject *winner,int *bWallCount)
{
	int nLine, nCol;
	srand((unsigned)time(NULL));
	puts("Enter the coordinates to call an airstrike on.\n");
	scanf("%d,%d", &nLine, &nCol);
	puts("");
	nLine += 2;
	nCol += 2;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			strike[i][j] = rand() % 2;
		}
	}
	if (strike[1][1] && Map[nLine][nCol] && Map[nLine][nCol] != WALL)
	{
		switch (Map[nLine][nCol])
		{
		case LTANK:
			*winner = RTANK;
			break;
		case RTANK:
			*winner = LTANK;
			break;
		case BREAKABLE_WALL:
			(*bWallCount)--;
			break;
		}
		Map[nLine][nCol] = EMPTY;
	}
	for (int i = 0; i < 8; i++)
	{
		if (strike[Directions[i].nLine + 1][Directions[i].nCol + 1])
		{
			switch (Map[nLine + Directions[i].nLine][nCol + Directions[i].nCol])
			{
			case LTANK:
				if (*winner == LTANK)
					*winner = WALL;
				else *winner = RTANK;
				break;
			case RTANK:
				if (*winner == RTANK)
					*winner = WALL;
				else *winner = LTANK;
				break;
			case BREAKABLE_WALL:
				(*bWallCount)--;
				break;
			}
			if (Map[nLine + Directions[i].nLine][nCol + Directions[i].nCol] != WALL)
				Map[nLine + Directions[i].nLine][nCol + Directions[i].nCol] = EMPTY;
		}
	}
}
void turn(GameObject tank,int *selection,GameObject* winner,int *bWallCount)
{
	puts("Enter 1 to move.\n");
	puts("Enter 2 to fire in a set direction.\n");
	puts("Enter 3 to fire in a changing direction.\n");
	puts("Enter 4 to call an airstrike.\n");
	puts("Enter -999 to quit the game.\n");
	scanf("%d", &(*selection));
	puts("");
	switch (*selection)
	{
	case 1:
		move(tank);
		break;
	case 2:
		shell(tank, winner, bWallCount);
		break;
	case 3:
		switch_shell(tank, winner, bWallCount);
		break;
	case 4:
		airstrike(winner, bWallCount);
		break;
	default:
		if (*selection != -999)
		{
			puts("Invalid Selection.");
			exit(2);
		}
	}
}
//-----------------------------------------------------------------------------
// Tank Game
// -----------
//
// General : The program simulates a tank game.
//
// Input : Option selection and detail of turn operations.
//
// Process : Uses the current position of the tanks and direction array to 
//			 handle game operations.
//
// Output : Prints the current state of the map and an options menu.
//
//-----------------------------------------------------------------------------
// Programmer : 
// Student No : 
// Date : 22.02.2024
//-----------------------------------------------------------------------------
int main()
{
	GameObject winner = EMPTY, turn_tank = LTANK;
	int bWallCount = 0, selection = 0;
	init_map(&bWallCount);
	while(selection!=-999&&!winner&&bWallCount)
	{
		for (int i = 2; i < N + 2; i++)
		{
			for (int j = 2; j < N + 2; j++)
				printf("%d\t", Map[i][j]);
			puts("\n");
		}
		if (turn_tank == LTANK)
		{
			puts("Left Tank:\n");
			turn(LTANK,&selection,&winner,&bWallCount);
			turn_tank = RTANK;
		}
		else
		{
			puts("Right Tank:\n");
			turn(RTANK, &selection, &winner, &bWallCount);
			turn_tank = LTANK;
		}
	}
	if (winner)
	{
		char *msg = (char*)malloc(11*sizeof(char));
		if (winner == LTANK)
			strcpy(msg, "Left tank");
		else if (winner == RTANK)
			strcpy(msg, "Right tank");
		else strcpy(msg, "Wall");
		printf("Game over. The winner is: %s",msg);
		free(msg);
	}
	else if(!bWallCount)
	{
		puts("Game over. It is a draw.");
	}
	else
	{
		puts("Bye bye.");
	}
}
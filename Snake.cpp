#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
using namespace std;

// =======================================================================
enum direction
{
	Up, Left, Down, Right	
};
enum item
{
	Empty, Wall, Player1, Player2, Food1, Food2, Bot1
};
enum color
{
	Black, Blue, Green, Aqua, Red, Purple, Yellow, White, Gray,
	LightBlue, LightGreen, LightAqua, LightRed, LightPurple, LightYellow, LightWhite
};
// =======================================================================
item ground[200][110];
bool changeFood = false;
// =======================================================================
void gotoxy(int x, int y)
{
   COORD coord;
   coord.X = x;
   coord.Y = y;
   SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}
int random(int MIN, int MAX)
{
	return MIN + rand() % (MAX - MIN + 1);
}
void setColor(color text, color ground)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, text + ground * 16);
}
// =======================================================================
bool timeProcess(int &time, int check)
{
	if (time == check) 
	{
		time = 1;
		return true;
	}
	else 
	{
		time++;
		return false;
	}
}
// =======================================================================
class Snake
{
	public:
		int length;
		int bodyPosition[200][3];
		int footPrint[3];
		direction direct;
		int faceSnake[3];
		int score;
		bool movable;
		int runSpeed;
		bool die;
		int checkIncrease;
		color colorSnake;
		int dieTime;
		int runTime;
		item typeSnake;
		
		void getFace()
		{
			switch (direct)
			{
				case Up:
					faceSnake[1] = bodyPosition[1][1];
					faceSnake[2] = bodyPosition[1][2] - 1;
					break;
				case Down:
					faceSnake[1] = bodyPosition[1][1];
					faceSnake[2] = bodyPosition[1][2] + 1;
					break;
				case Left:
					faceSnake[2] = bodyPosition[1][2];
					faceSnake[1] = bodyPosition[1][1] - 1;
					break;
				case Right:
					faceSnake[2] = bodyPosition[1][2];
					faceSnake[1] = bodyPosition[1][1] + 1;
					break;			
			}
		}
		void drawSnake()
		{
			setColor(colorSnake, Black);
			gotoxy(footPrint[1], footPrint[2]);
			cout << " ";
			gotoxy(bodyPosition[1][1], bodyPosition[1][2]);
			cout << "@";
			for (int i = 2 ; i <= length ; i++)
			{
				gotoxy(bodyPosition[i][1], bodyPosition[i][2]);
				cout << "*";
			}
			setColor(LightWhite, Black);
			
		}
		void runSnake()
		{
			footPrint[1] = bodyPosition[length][1];
			footPrint[2] = bodyPosition[length][2];
			for (int i = length ; i > 1 ; i--)
			{
				bodyPosition[i][1] = bodyPosition[i-1][1];
				bodyPosition[i][2] = bodyPosition[i-1][2];
			}
			switch (direct)
			{
				case Up:
					bodyPosition[1][2]--;
					break;
				case Down:
					bodyPosition[1][2]++;
					break;
				case Left:
					bodyPosition[1][1]--;
					break;
				case Right:
					bodyPosition[1][1]++;
					break;			
			}
			if (checkIncrease > 0)
			{
				bodyPosition[length + 1][1] = footPrint[1];
				bodyPosition[length + 1][2] = footPrint[2];
				footPrint[1] = 1;
				footPrint[2] = 1;
				length++;
				checkIncrease--;
			}
		}
		void respawnSnake(int coordx, int coordy, direction di, int leng, item type, int speed)
		{
			if (di == Right)
			{
				for (int i = leng ; i >= 1 ; i--)
				{
					bodyPosition[i][1] = coordx + leng - i;
					bodyPosition[i][2] = coordy;
					ground[bodyPosition[i][1]] [bodyPosition[i][2]] = type;
				}
				footPrint[1] = bodyPosition[leng][1] - 1;
				footPrint[2] = bodyPosition[leng][2];
			}
			else if (di == Left)
			{
				for (int i = 1; i <= leng; i++)
				{
					bodyPosition[i][1] = coordx + i - 1;
					bodyPosition[i][2] = coordy;
					ground[bodyPosition[i][1]] [bodyPosition[i][2]] = type;	
				}
				footPrint[1] = bodyPosition[leng][1] + 1;
				footPrint[2] = bodyPosition[leng][2];
			}
			length = leng;
			direct = di;
			checkIncrease = 0;
			getFace();
			die = false;
			runSpeed = speed;
			score = 0;
		}
		void checkSnake()
		{
			switch (ground[faceSnake[1]][faceSnake[2]])
			{
				case Empty:
					runSnake();
					ground[footPrint[1]] [footPrint[2]] = Empty;
					ground[bodyPosition[1][1]] [bodyPosition[1][2]] = typeSnake;
					break;
				case Food1:
					runSnake();
					ground[footPrint[1]] [footPrint[2]] = Empty;
					ground[bodyPosition[1][1]] [bodyPosition[1][2]] = typeSnake;
					changeFood = true;
					score += 10;
					if (score % 50 == 0) checkIncrease++;
					break;
				case Food2:
					runSnake();
					ground[footPrint[1]] [footPrint[2]] = Empty;
					ground[bodyPosition[1][1]] [bodyPosition[1][2]] = typeSnake;
					changeFood = true;
					score += 30;
					if (score % 50 == 0 || score % 50 == 10 || score % 50 == 20) checkIncrease++;
					break;	
				default:	
					movable = false;
					die = true;
					break;
			}
		}
};
// =======================================================================
Snake snakeX, bot1;

// =======================================================================
void drawBackground()
{
	int i;
	for (i = 20 ; i<= 100 ; i++)
	{
		gotoxy(i, 0);
		cout << "#";
		gotoxy(i, 29);
		cout << "#";
	}
	for (i = 0 ; i <= 29 ; i++)
	{
		gotoxy(20, i);
		cout << "#";
		gotoxy(100, i);
		cout << "#";
	}
	gotoxy(7, 20);
	cout << "Score";
	gotoxy(110, 4);
	cout << "Score";
	
}
void drawScore()
{
	gotoxy(7, 21);
	cout << "       ";
	gotoxy(7, 21);
	cout << snakeX.score;
	gotoxy(110,5);
	cout << "       ";
	gotoxy(110,5);
	cout << bot1.score;
}
// =======================================================================

int main ()
{
	int i, j, l, key, countTime = 1;
	bool check;
	srand((int)time(0));
	// Prepare Background
	
	for (i = 20 ; i <= 100 ; i++)
	{
		ground[i][0] = Wall;
		ground[i][29] = Wall;
	}
	for (i = 0 ; i <= 29 ; i++)
	{
		ground[20][i] = Wall;
		ground[100][i] = Wall;
	}
	for (i = 21 ; i <= 99 ; i++)
		for (j = 1 ; j <= 28 ; j++)
			ground[i][j] = Empty;		
	for (i = 1 ; i <= 10 ; i++)
	{
		j = random(21, 99);
		l = random(1, 28);
		if (ground[j][l] == Empty)
		{
			ground[j][l] = Food1;
			gotoxy(j, l);
			cout << "-";
		}
		else i--;
	}
	// Make a snake
	snakeX.respawnSnake(25, 5, Right, 5, Player1, 5);
										bot1.respawnSnake(85, 5, Left, 5, Bot1, 5);

	
	snakeX.movable = true;
	snakeX.colorSnake = Red;
										bot1.movable = true;
										bot1.colorSnake = LightWhite;
	// Draw First
	drawBackground();
	snakeX.drawSnake();
										bot1.drawSnake();
	drawScore();									
	// Time variable
	int foodTime = 1;
	snakeX.dieTime = 1;
										bot1.dieTime = 1;
	int bot1Time = 1;									
	Loop1: ; // < Loop 1 >
	
	// Part 1: Get keyboard input
	if (kbhit())
	{
		key = getch();
		switch (key)
		{
			case 'w':
				if (snakeX.direct != Down) snakeX.direct = Up;
				break;
			case 'a':
				if (snakeX.direct != Right) snakeX.direct = Left;
				break;
			case 's':
				if (snakeX.direct != Up) snakeX.direct = Down;
				break;
			case 'd':
				if (snakeX.direct != Left) snakeX.direct = Right;
				break;
				
			case 8:
				getch();
				break;
				
			case 27: 
				return 0;
				break;		
		}
	}
	snakeX.getFace();
	// Part 2: Update game internals
	
	if (snakeX.movable && !snakeX.die) 
	{
		if (timeProcess(snakeX.runTime, snakeX.runSpeed))
		{
			snakeX.checkSnake();
			if (changeFood) 
			{
				drawScore();
				changeFood = false;
			}
			if (snakeX.movable)
			{
				snakeX.getFace();
				snakeX.drawSnake();
			}
		}	
	}
	// Bot
	if (bot1.movable && !bot1.die)
	{
		if (timeProcess(bot1.runTime, bot1.runSpeed))
		{
			bot1.checkSnake();
			if (changeFood) 
			{
				drawScore();
				changeFood = false;	
			}
			if (bot1.movable)
			{
				bot1.getFace();
				bot1.drawSnake();
			}
		// Random control
		if (bot1.movable && !bot1.die)
		{
			item temp = ground[bot1.faceSnake[1]][bot1.faceSnake[2]];
			if (temp != Empty && temp != Food1 && temp != Food2)
			{
				int temp1 = (bot1.direct + random(1, 2) * 2 + 1) % 4;
				bot1.direct = static_cast<direction> (temp1);
				bot1.getFace();
			}
			static int bot1_random = random(5, 20);
			if (bot1.bodyPosition[1][1] == 21 || bot1.bodyPosition[1][1] == 99 || bot1.bodyPosition[1][2] == 1 || bot1.bodyPosition[1][2] == 18)
			{
				if (bot1.bodyPosition[1][1] == 21) 
				{
					if (random(0, 2)) bot1.direct = Right; 
				}
				else if (bot1.bodyPosition[1][1] == 99) 
				{
					if (random(0, 2)) bot1.direct = Left; 
				}
				else if (bot1.bodyPosition[1][2] == 1) 
				{
					if (random(0, 2)) bot1.direct = Down; 
				}
				else if (random(0, 2)) bot1.direct = Up;
			}
			else if (timeProcess(bot1Time, bot1_random))
			{
				int temp1 = (bot1.direct + random(1, 2) * 2 + 1) % 4;
				bot1.direct = static_cast<direction> (temp1);
				bot1.getFace();
				bot1_random = random(5, 20);
			}
		}
		}
	}
	// Snake Die
	if (snakeX.die)
	{
		if (snakeX.dieTime == 1)
		{
			for (i = 1; i <= snakeX.length; i++)
			{
				ground[snakeX.bodyPosition[i][1]] [snakeX.bodyPosition[i][2]] = Food2;
				gotoxy(snakeX.bodyPosition[i][1], snakeX.bodyPosition[i][2]);
				cout << "+";
			}
			snakeX.dieTime++;
		}
		
		if (timeProcess(snakeX.dieTime, 50))
		{
			snakeX.respawnSnake(25, 5, Right, 5, Player1, 5);
			drawScore();
			snakeX.movable = true;
			snakeX.die = false;
		}	
	}
	// Bot die
	if (bot1.die)
	{
		if (bot1.dieTime == 1)
		{
			for (i = 1; i <= bot1.length; i++)
			{
				ground[bot1.bodyPosition[i][1]] [bot1.bodyPosition[i][2]] = Food2;
				gotoxy(bot1.bodyPosition[i][1], bot1.bodyPosition[i][2]);
				cout << "+";
			}
			bot1.dieTime++;
		}
		
		if (timeProcess(bot1.dieTime, 50))
		{
			bot1.respawnSnake(85, 5, Left, 5, Bot1, 5);
			drawScore();
			bot1.movable = true;
			bot1.die = false;
		}	
	}
	// Time controlling
	if (timeProcess(foodTime, 100))
	{
		{ // Make food
			check = true;
			while (check)
			{
				j = random(21, 99);
				l = random(1, 28);
				if (ground[j][l] == Empty)
				{
					ground[j][l] = Food1;
					gotoxy(j, l);
					cout << "-";
					check = false;
				}
			}
		}
		
	}
	
	
	// < Loop 1 >
	countTime++;
	if (countTime == 1001) countTime = 1;
	Sleep(10);
	goto Loop1;
	
}

#include<iostream>
#include<windows.h>
#include<time.h>
using namespace std;
#include<conio.h>
#include<math.h>

enum DIRECTION {UP=72,DOWN=80,LEFT=75,RIGHT=77};

struct pos
{
	int r, c;	//position rows and columns
};
struct snake
{
	int size;  //snake size
	char sbs;  //snake body symbol
	pos* sp;   //snake position
	char shs;  //snake head symbol
	int score;
	DIRECTION D;
};
struct langar  //word of punjabi meaning food...
{
	int score;  //langar score
	pos lp;		//langar position
	char ls;    //langar symbol
};

void Init(snake& S, langar& L, int Rows, int Cols);
void gotoRowCol(int r, int c);
bool IsValid(const snake& S, const pos& p);
void UpdateLangar(langar& L, const snake& S, int Rows, int Cols);
void DisplaySnake(const snake& S, int Rows);
void RemoveSnake(const snake& S);
void UpdateSnake(snake& S, langar& L, int Rows, int Cols,float& speed);
void DisplayLangar(const langar& L, const snake& S, int Rows);
void RegrowSnake(snake& S, int Score);
bool SnakeKilled(const snake& S);
void ChangeColour(int k);


int main()
{
	float speed = 70;
	char ci;
	srand(time(0));
	snake S;
	langar L;
	int Rows = 50, Cols = 50;
	Init(S, L, Rows, Cols);
	while (true)
	{
		if (_kbhit())
		{
			ci = _getch();
			if (ci == -32)
			{
				ci = _getch();
				switch (ci)
				{
					case UP:
						if(S.D!=DOWN)
						S.D=UP;
						break;
					case DOWN:
						if (S.D != UP)
						S.D=DOWN;
						break;
					case RIGHT:
						if (S.D != LEFT)
						S.D = RIGHT;
						break;
					case LEFT:
						if (S.D != RIGHT)
						S.D = LEFT;
						break;
				}
			}
		}
		if (SnakeKilled(S) == true)
		{
			break;
		}
		UpdateSnake(S, L, Rows, Cols, speed);
		ChangeColour(S.score%13+2);
		DisplaySnake(S,Rows);
		DisplayLangar(L, S, Rows);
		Sleep(speed);
		RemoveSnake(S);
	}
	system("cls");
	gotoRowCol(Rows / 2, Cols / 2 - 15);
	cout << "GAME OVER!!!  YOUR SCORE IS : " << S.score;
	return _getch();
}


void gotoRowCol(int r, int c)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = c;
	scrn.Y = r;
	SetConsoleCursorPosition(hOuput, scrn);
}
bool IsValid(const snake& S, const pos& p)
{
	for (int i = 0; i < S.size; i++)
	{
		if (S.sp[i].r == p.r && S.sp[i].c == p.c) return false;
	}
	return true;
}
void UpdateLangar(langar& L, const snake& S, int Rows, int Cols)
{
	do
	{
		L.lp.r = rand() % Rows;
		L.lp.c = rand() % Cols;
	} while (!IsValid(S, L.lp));
}
void Init(snake& S, langar& L, int Rows, int Cols)
{
	S.size = 1;
	S.shs = -37;
	S.sbs = '*';
	S.score = 0;
	S.D = RIGHT;
	S.sp = new pos[1];
	S.sp[0].r = Rows / 2;
	S.sp[0].c = Cols / 2;
	L.score = 1;
	L.ls = '$';
	UpdateLangar(L, S, Rows, Cols);
}
void DisplaySnake(const snake& S, int Rows)
{
	for (int i = 0; i < S.size; i++)
	{
		gotoRowCol(S.sp[i].r, S.sp[i].c);
		if (i == 0)
			cout << S.shs;
		else
			cout << S.sbs;
	}
}
void RemoveSnake(const snake& S)
{
	for (int i = 0; i < S.size; i++)
	{
		gotoRowCol(S.sp[i].r, S.sp[i].c);
		cout << " ";
	}
}
void UpdateSnake(snake& S, langar& L, int Rows, int Cols,float& speed)
{
	int ri = S.sp[0].r;
	int ci = S.sp[0].c;
	switch (S.D)
	{
	case UP:
		ri--;
		break;
	case DOWN:
		ri++;
		break;
	case LEFT:
		ci--;
		break;
	case   RIGHT:
		ci++;
		break;
	}
	if (ri >= Rows) ri = 0;
	if (ri < 0) ri = Rows - 1;
	if (ci >= Cols) ci = 0;
	if (ci < 0) ci = Cols - 1;
	for (int i = S.size - 1; i >= 1; i--)
	{
		S.sp[i] = S.sp[i - 1];
	}
	if (ri == L.lp.r && ci == L.lp.c)
	{
		RegrowSnake(S, L.score);
		S.score++;
		speed -= 3;
		UpdateLangar(L, S, Rows, Cols);
	}
	S.sp[0].r = ri;
	S.sp[0].c = ci;
}
void DisplayLangar(const langar& L, const snake& S, int Rows)
{
	gotoRowCol(L.lp.r, L.lp.c);
	cout << L.ls;
	gotoRowCol(Rows, 0);
	cout << "Score : " << S.score;
}
void RegrowSnake(snake& S, int Score)
{
	int NewSize = S.size + Score;
	pos* N = new pos[NewSize];
	for (int i = 0; i < S.size; i++)
	{
		N[i].r = S.sp[i].r;
		N[i].c = S.sp[i].c;
	}
	delete[] S.sp;
	S.sp = N;
	S.size = NewSize;
}
bool SnakeKilled(const snake& S)
{
	for (int i = 1; i < S.size; i++)
	{
		if (S.sp[i].r == S.sp[0].r && S.sp[i].c == S.sp[0].c) return true;
	}
	return false;
}
void ChangeColour(int k)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);
}
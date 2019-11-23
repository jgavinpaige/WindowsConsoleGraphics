#include "ConsoleEngine.h"
#include <iostream>
#include <vector>
#include <chrono>

using std::chrono_literals::operator""ms;

class Snake : public ConsoleGame
{
	std::vector<vec::Vector2> snake;
	int direction = 1, appleX, appleY;
	int width, height;

	void OnCreate() override
	{
		//initialize game board
		width = GetWidth(), height = GetHeight();
		for (int x = 0; x < width; x++)
			SetChar(x, 0, 219), SetChar(x, height - 1, 219);
		for (int y = 0; y < height; y++)
			SetChar(0, y, 219), SetChar(width - 1, y, 219);

		//initialize snake
		snake = { {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1} };

		//set apple
		appleX = width / 2;
		appleY = height / 2;
		SetChar(appleX, appleY, 219, FOREGROUND_RED | FOREGROUND_INTENSITY);

		srand(time(0));
	}
	void OnUpdate() override
	{
		auto start = std::chrono::system_clock::now();

		bool dirSet = false;
		while (std::chrono::system_clock::now() - start < 120ms)
		{
			if (!dirSet)
			{
				if (GetKeyState(VK_UP) < 0 && direction != 0 && direction != 2)
					direction = 0, dirSet = true;
				if (GetKeyState(VK_RIGHT) < 0 && direction != 1 && direction != 3)
					direction = 1, dirSet = true;
				if (GetKeyState(VK_DOWN) < 0 && direction != 2 && direction != 0)
					direction = 2, dirSet = true;
				if (GetKeyState(VK_LEFT) < 0 && direction != 3 && direction != 1)
					direction = 3, dirSet = true;
			}
		}

		vec::Vector2 head = snake.front();
		switch (direction)
		{
		case 0:
			head.Y--; break;
		case 1:
			head.X++; break;
		case 2:
			head.Y++; break;
		case 3:
			head.X--; break;
		}

		if (head.X >= GetWidth() - 1 || head.Y >= GetHeight()-1 || head.X < 1 || head.Y < 1)
		{
			Stop();
			return;
		}

		snake.pop_back();

		for(vec::Vector2 vec : snake)
			if (head.X == vec.X && head.Y == vec.Y)
			{
				Stop();
				return;
			}

		snake.insert(snake.begin(), head);

		FillRect(1, 1, width - 2, height - 2, ' ');

		SetChar(appleX, appleY, 219, FOREGROUND_RED | FOREGROUND_INTENSITY);

		for (vec::Vector2 vec : snake)
			SetChar(vec.X, vec.Y, 219, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		if (head.X == appleX && head.Y == appleY)
		{
			for (int i = 0; i < 5; i++)
				snake.push_back(snake.back());
			do
			{
				appleX = rand() % width;
				appleY = rand() % height;
			} while (GetChar(appleX, appleY) != ' ');
			SetChar(appleX, appleY, 219, FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
	}
};

int main()
{
	Snake snakeGame;
	snakeGame.CreateConsole(70, 50, 15, 15, ' ');
	snakeGame.Start();
}
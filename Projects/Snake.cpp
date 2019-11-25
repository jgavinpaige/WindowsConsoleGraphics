#include "ConsoleEngine.h"
#include <vector>
#include <queue>
#include <chrono>
#include <fstream>

using std::chrono_literals::operator""ms;

class Snake : public ConsoleGame
{
	std::vector<vec::Vector2> snake;
	std::queue<int> directionQueue;
	int direction = 1, appleX, appleY;
	int width, height;
	std::ofstream logFile;

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

		logFile.open("logFile.txt");

		srand(time(0));
	}
	void OnUpdate() override
	{
		auto start = std::chrono::system_clock::now();

		while (std::chrono::system_clock::now() - start < 120ms);

		if (directionQueue.size())
		{
			direction = directionQueue.front();
			directionQueue.pop();
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

	void OnKeyPress(int keyCode) override
	{
		int currentDirection = directionQueue.size() ? directionQueue.back() : direction;

		if (keyCode == VK_UP && currentDirection != 0 && currentDirection != 2)
			directionQueue.push(0);
		if (keyCode == VK_RIGHT && currentDirection != 1 && currentDirection != 3)
			directionQueue.push(1);
		if (keyCode == VK_DOWN && currentDirection != 2 && currentDirection != 0)
			directionQueue.push(2);
		if (keyCode == VK_LEFT && currentDirection != 3 && currentDirection != 1)
			directionQueue.push(3);
	}

	void OnKeyRelease(int keyCode) override {}
};

int main()
{
	Snake snakeGame;
	snakeGame.CreateConsole(70, 50, 15, 15, ' ');
	snakeGame.Start();
}
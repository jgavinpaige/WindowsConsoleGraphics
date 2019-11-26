#include "ConsoleEngine.h"
#include <stack>
#include <vector>
#include <chrono>

struct MazeSquare
{
	static enum direction {UP = 0b1000, DOWN = 0b0100, LEFT = 0b0010, RIGHT = 0b0001};

	bool visited = false;
	int direction = 0;
};

class Maze : public ConsoleGame
{
private:
	MazeSquare* m_grid;
	std::stack<vec::Vector2> position;
	int m_gridWidth, m_gridHeight;

public:
	void OnCreate() override
	{
		m_gridWidth = (GetWidth() - 1) / 3;
		m_gridHeight = (GetHeight() - 1) / 3;
		m_grid = new MazeSquare[m_gridWidth * m_gridHeight];
		position.push({ 0, 0 });

		srand(time(0));
	}
	void OnUpdate() override
	{
		vec::Vector2 pos = position.top();
		std::vector<vec::Vector2> nextPositions;

		m_grid[pos.X + pos.Y * m_gridWidth].visited = true;

		//get possible next positions
		if (pos.X > 0 && !m_grid[(pos.X - 1) + pos.Y * m_gridWidth].visited)
			nextPositions.push_back({ pos.X - 1, pos.Y });
		if (pos.Y > 0 && !m_grid[pos.X + (pos.Y - 1) * m_gridWidth].visited)
			nextPositions.push_back({ pos.X, pos.Y - 1 });
		if (pos.X < m_gridWidth - 1 && !m_grid[(pos.X + 1) + pos.Y * m_gridWidth].visited)
			nextPositions.push_back({ pos.X + 1, pos.Y });
		if (pos.Y < m_gridHeight - 1 && !m_grid[pos.X + (pos.Y + 1) * m_gridWidth].visited)
			nextPositions.push_back({ pos.X, pos.Y + 1 });

		//pick a directions
		if (nextPositions.size())
		{
			vec::Vector2 next = nextPositions[rand() % nextPositions.size()];
			if (next.X > pos.X)
				m_grid[pos.X + pos.Y * m_gridWidth].direction |= MazeSquare::RIGHT;
			else if (next.X < pos.X)
				m_grid[pos.X + pos.Y * m_gridWidth].direction |= MazeSquare::LEFT;
			else if (next.Y > pos.Y)
				m_grid[pos.X + pos.Y * m_gridWidth].direction |= MazeSquare::DOWN;
			else
				m_grid[pos.X + pos.Y * m_gridWidth].direction |= MazeSquare::UP;

			position.push(next);
		}
		else
			position.pop();

		if (!position.size())
			Stop();

		//display grid to console
		for (int i = 0; i < m_gridWidth * m_gridHeight; i++)
		{
			FillRect(i % m_gridWidth * 3, i / m_gridWidth * 3, 4, 4, 219);
			FillRect(i % m_gridWidth * 3 + 1, i / m_gridWidth * 3 + 1, 2, 2, ' ');
		}

		for (int i = 0; i < m_gridWidth * m_gridHeight; i++)
		{
			if (m_grid[i].direction & MazeSquare::UP)
				FillRect(i % m_gridWidth * 3 + 1, i / m_gridWidth * 3, 2, 1, ' ');
			if (m_grid[i].direction & MazeSquare::DOWN)
				FillRect(i % m_gridWidth * 3 + 1, i / m_gridWidth * 3 + 3, 2, 1, ' ');
			if (m_grid[i].direction & MazeSquare::LEFT)
				FillRect(i % m_gridWidth * 3, i / m_gridWidth * 3 + 1, 1, 2, ' ');
			if (m_grid[i].direction & MazeSquare::RIGHT)
				FillRect(i % m_gridWidth * 3 + 3, i / m_gridWidth * 3 + 1, 2, 2, ' ');
		}
	}
	void OnKeyPress(int keyCode) override {}
	void OnKeyRelease(int keyCode) override {}
};

int main()
{
	Maze mazeGame;
	mazeGame.CreateConsole(120, 80, 10, 10, ' ');
	mazeGame.Start();
}
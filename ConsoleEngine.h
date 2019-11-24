#pragma once

#include <Windows.h>
#include <thread>

class ConsoleGame
{
private:
	HANDLE m_outHandle, m_inHandle;
	CHAR_INFO* m_screen;
	COORD m_bufferSize;
	SMALL_RECT m_screenDimensions;
	bool m_running;
	bool m_keyDown[256];

	void UpdateKeys();
public:
	HANDLE GetOutputHandle();
	HANDLE GetInputHandle();
	CHAR_INFO* GetBuffer();
	bool CreateConsole(int consoleWidth, int consoleHeight, int charWidth, int charHeight, char fillCharacter);
	void Start();
	void Stop();
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnKeyPress(int keyCode) = 0;
	virtual void OnKeyRelease(int keyCode) = 0;

	bool SetTitle(const char*);
	void SetChar(int x, int y, char ch);
	void SetChar(int x, int y, char ch, int attribute);
	void FillRect(int x, int y, int width, int height, char ch);
	void FillRect(int x, int y, int width, int height, char ch, int attribute);
	void SetAll(char ch);
	void SetAll(char ch, int attribute);

	char GetChar(int x, int y);
	int GetWidth();
	int GetHeight();
};

namespace vec
{
	struct Vector2
	{
		int X, Y;
	};

	struct Vector3
	{
		int X, Y, Z;
	};
}

HANDLE ConsoleGame::GetOutputHandle()
{
	return m_outHandle;
}

HANDLE ConsoleGame::GetInputHandle()
{
	return m_inHandle;
}

CHAR_INFO* ConsoleGame::GetBuffer()
{
	return m_screen;
}

bool ConsoleGame::CreateConsole(int consoleWidth, int consoleHeight, int charWidth, int charHeight, char fillCharacter)
{
	m_outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_inHandle = GetStdHandle(STD_INPUT_HANDLE);

	m_bufferSize = { static_cast<short>(consoleWidth), static_cast<short>(consoleHeight) };
	m_screenDimensions = { 0, 0, static_cast<short>(consoleWidth - 1), static_cast<short>(consoleHeight - 1) };

	SMALL_RECT minWindow = { 0, 0, 1, 1 };

	if (!SetConsoleWindowInfo(m_outHandle, true, &minWindow))
		return false;
	if(!SetConsoleScreenBufferSize(m_outHandle, m_bufferSize))
		return false;

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = charWidth;
	cfi.dwFontSize.Y = charHeight;
	cfi.nFont = 0;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	if (!SetCurrentConsoleFontEx(m_outHandle, false, &cfi))
		return false;
	if(!SetConsoleWindowInfo(m_outHandle, true, &m_screenDimensions))
		return false;

	m_screen = new CHAR_INFO[consoleWidth * consoleHeight];
	SetAll(fillCharacter, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(m_outHandle, &cursorInfo))
		return false;

	cursorInfo.bVisible = false;
	if (!SetConsoleCursorInfo(m_outHandle, &cursorInfo))
		return false;

	OnCreate();
	WriteConsoleOutput(m_outHandle, m_screen, m_bufferSize, { 0, 0 }, &m_screenDimensions);

	memset(m_keyDown, 0, sizeof(bool) * 256);

	return true;
}

void ConsoleGame::Start()
{
	std::thread keyHandler(&ConsoleGame::UpdateKeys, this);

	m_running = true;
	while (m_running)
	{
		OnUpdate();
		WriteConsoleOutput(m_outHandle, m_screen, m_bufferSize, { 0, 0 }, &m_screenDimensions);
	}

	keyHandler.join();
}

void ConsoleGame::Stop()
{
	m_running = false;
}

bool ConsoleGame::SetTitle(const char* string)
{
	return SetConsoleTitle(string);
}

void ConsoleGame::SetChar(int x, int y, char ch)
{
	m_screen[x + y * GetWidth()].Char.AsciiChar = ch;
}

void ConsoleGame::SetChar(int x, int y, char ch, int attribute)
{
	m_screen[x + y * GetWidth()].Char.AsciiChar = ch;
	m_screen[x + y * GetWidth()].Attributes = attribute;
}

void ConsoleGame::FillRect(int xPos, int yPos, int width, int height, char fillChar)
{
	for (int y = yPos; y < yPos + height;y++)
		for (int x = xPos; x < xPos + width; x++)
			SetChar(x, y, fillChar);
}

void ConsoleGame::FillRect(int xPos, int yPos, int width, int height, char fillChar, int attribute)
{
	for (int y = yPos; y < yPos + height;y++)
		for (int x = xPos; x < xPos + width; x++)
			SetChar(x, y, fillChar, attribute);
}

void ConsoleGame::SetAll(char ch)
{
	for (int i = 0; i < GetWidth() * GetHeight(); i++)
		m_screen[i].Char.AsciiChar = ch;
}

void ConsoleGame::SetAll(char ch, int attribute)
{
	for (int i = 0; i < GetWidth() * GetHeight(); i++)
	{
		m_screen[i].Char.AsciiChar = ch;
		m_screen[i].Attributes = attribute;
	}
}

char ConsoleGame::GetChar(int x, int y)
{
	return m_screen[x + y * GetWidth()].Char.AsciiChar;
}

int ConsoleGame::GetWidth()
{
	return m_bufferSize.X;
}

int ConsoleGame::GetHeight()
{
	return m_bufferSize.Y;
}

void ConsoleGame::UpdateKeys()
{
	while (m_running)
	{
		for (int key = 0; key < 256; key++)
		{
			if (!m_keyDown[key] && GetAsyncKeyState(key) < 0)
			{
				m_keyDown[key] = true;
				OnKeyPress(key);
			}

			if (m_keyDown[key] && GetAsyncKeyState(key) >= 0)
			{
				m_keyDown[key] = false;
				OnKeyRelease(key);
			}
		}
	}
}
# WindowsConsoleGraphics
Console graphics in the windows console using c++

### Using the Engine
The game engine consists entirely inside of ConsoleEngine.h so it can just be copied into any header file and included in any project on Windows machines

To use the engine, a class must extend from the `ConsoleGame` abstract class and define the following member functions:
```
	void OnCreate()
	void OnUpdate()
```

Optionally, the following member functions can be defined for additional functionality:
```
	void OnStart()
	void OnStop()
	void OnKeyPress(int keyCode)
	void OnKeyRelease(int keyCode)
```


Once a child object of `ConsoleGame` is created, the first call must be to the member function:
```
	bool CreateConsole(int consoleWidth, int consoleHeight, int charWidth, int charHeight, char fillChar)
```

After the console is initialized, the member function `Start()` can be safely called to start the game loop.

### Documentation
```
void OnCreate()
```

* Called when the console window is created

```
void OnUpdate()
```

* Called continuously while the game is running

```
void OnStart()
```

* Called immediately after `Start()` and is the executed before the first `OnUpdate()` call


```
void OnStop()
```

* Called when the `OnUpdate()` functions exits
* *Not called if `OnUpdate()` is interrupted*

```
void OnKeyPress(int key)
```

* Invoked whenever a key is pressed down
* `key` specifies the keycode of the key that is pressed down which is defined in the [Windows API](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)

```
void OnKeyRelease(int key)
```

* Invoked whenever a key is released
* `key` specifies the keycode of the key that is pressed down which is defined in the [Windows API](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)

```
void Refresh()
```

* Called to update the screen

```
bool CreateConsole(int cW, int cH, int chW, int chH, char fillChar) 
```
* Called to create the console with specified dimensions and initialize every charcter to the fillChar.
* `cW` specifies the width (in characters) of the console.
* `cH` specifies the width (in characters) of the console.
* `chW` specifies the width of the displayed characters in the console.
* `chH` specifies the height of the displayed characters in the console.
* `fillChar` specifies what character to fill each slot in the console with

```
void SetTitle(const char*)
```

* Sets the title for the console window

```
void SetChar(int x, int y, char ch)
void SetChar(int x, int y, char ch, int attribute)
```

* Sets the character (and optionally color) for a location on the screen
* `x` defines the x location of the character on the console
* `y` defines the y location of the character on the console
* `ch` defines the character to set the position to
* `attribute` specifies the color attribute for the position defined in the [Windows API](https://docs.microsoft.com/en-us/windows/console/char-info-str)

```
void FillRect(int x, int y, int width, int height)
void FillRect(int x, int y, int width, int height, int attribute)
```

* Sets a rectangle of characters (and optionally color) on the screen
* `x` defines the location of the leftmost character in the rectangle
* `y` defines the location of the uppermost character in the rectangle
* `width` specifies how many characters to fill in starting at and to the right of `x`
* `height` specifies how many characters to fill in stating at and below `y`
* `ch` defines the character to set the position to
* `attribute` specifies the color attribute for the position defined in the [Windows API](https://docs.microsoft.com/en-us/windows/console/char-info-str)


```
void SetAll(char ch)
void SetAll(char ch, int attribute)
```

* Sets all characters on the screen to be a specific character (and optionally color)
* `ch` defines the character to set the position to
* `attribute` specifies the color attribute for the position defined in the [Windows API](https://docs.microsoft.com/en-us/windows/console/char-info-str)

```
char GetChar(int x, int y)
```

* Returns the character at a position on the screen
* `x` defines the x location of the character on the console
* `y` defines the y location of the character on the console

```
int GetWidth()
```

* Returns the console width as number of characters across

```
int GetHeight()
```

* Returns the console height as number of characters top to bottom

```
HANDLE GetOutputHandle()
```

* Returns the output [HANDLE](https://docs.microsoft.com/en-us/windows/console/console-handles) for the console

```
HANDLE GetInputHandle()
```

* Returns the input [HANDLE](https://docs.microsoft.com/en-us/windows/console/console-handles) for the console

```
CHAR_INFO* GetBuffer()
```

* Returns a pointer to an array of [CHAR_INFO](https://docs.microsoft.com/en-us/windows/console/char-info-str) which makes up the display and is reffered to as a [buffer](https://docs.microsoft.com/en-us/windows/console/console-screen-buffers)

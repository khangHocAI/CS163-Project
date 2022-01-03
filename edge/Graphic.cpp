#include"suffixTree.h"
void gotoxy(int x, int y)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x ,y };
	SetConsoleCursorPosition(output, pos);
}
void color(int x)
{
	HANDLE hconsoleColor;
	hconsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hconsoleColor, x);
}
void clear2() // fuction clear things better than system("clear")
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	/*FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);*/
	SetConsoleCursorPosition(console, topLeft);
}
void clear() // fuction clear things better than system("clear")
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	/*FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);*/
	SetConsoleCursorPosition(console, topLeft);
	system("color F7");
}
void FullScreen()
{
	HANDLE Output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Coordinates = { 300, 300 }; //238 la chieu dai 86 la chieu rong
	SetConsoleScreenBufferSize(Output, Coordinates);
	SMALL_RECT Coordinates2 = { 0, 0, 0, 0 }; // canh chuan toa do
	SetConsoleWindowInfo(Output, TRUE, &Coordinates2);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}
void VeKhungCH(int x, int y, int width, int height, bool draw) {
	//v? khung ch? nh?t
	if (draw) {
		//v? trý?c 4 góc	
		//góc trên bên trái
		gotoxy(x, y);
		cout << char(218);
		//v? góc trên bên ph?i
		gotoxy(x + width, y);
		cout << char(191);
		//v? góc dý?i bên trái
		gotoxy(x, y + height);
		cout << char(192);
		//v? góc dý?i bên ph?i
		gotoxy(x + width, y + height);
		cout << char(217);

		//v? chi?u ngang bên trên
		for (int i = x + 1; i < (x + width); i++) {
			gotoxy(i, y);
			cout << char(196);
		}
		//v? chi?u ngang bên dý?i
		for (int i = x + 1; i < (x + width); i++) {
			gotoxy(i, y + height);
			cout << char(196);
		}
		//v? chi?u d?c bên trái
		for (int i = y + 1; i < (y + height); i++) {
			gotoxy(x, i);
			cout << char(179);
		}
		//v? chi?u d?c bên ph?i
		for (int i = y + 1; i < (y + height); i++) {
			gotoxy(x + width, i);
			cout << char(179);
		}
	}
	else {
		//v? trý?c 4 góc
		//góc trên bên trái
		gotoxy(x, y);
		cout << " ";
		//v? góc trên bên ph?i
		gotoxy(x + width, y);
		cout << " ";
		//v? góc dý?i bên trái
		gotoxy(x, y + height);
		cout << " ";
		//v? góc dý?i bên ph?i
		gotoxy(x + width, y + height);
		cout << " ";

		//v? chi?u ngang bên trên
		for (int i = x + 1; i < (x + width); i++) {
			gotoxy(i, y);
			cout << " ";
		}
		//v? chi?u ngang bên dý?i
		for (int i = x + 1; i < (x + width); i++) {
			gotoxy(i, y + height);
			cout << " ";
		}
		//v? chi?u d?c bên trái
		for (int i = y + 1; i < (y + height); i++) {
			gotoxy(x, i);
			cout << " ";
		}
		//v? chi?u d?c bên ph?i
		for (int i = y + 1; i < (y + height); i++) {
			gotoxy(x + width, i);
			cout << " ";
		}

	}
}
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
void introduce()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
	gotoxy(0, 0);
	ShowConsoleCursor(false);
	for (int i = 1; i <= 50; i++)
	{
		clear2();
		gotoxy(0, 0);
		string text = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t        _/          _/_/      _/_/    _/_/_/    _/_/_/  _/      _/    _/_/_/\n\t\t\t\t\t\t       _/        _/    _/  _/    _/  _/    _/    _/    _/_/    _/  _/\n\t\t\t\t\t\t      _/        _/    _/  _/_/_/_/  _/    _/    _/    _/  _/  _/  _/  _/_/\n\t\t\t\t\t\t     _/        _/    _/  _/    _/  _/    _/    _/    _/    _/_/  _/    _/\n\t\t\t\t\t\t    _/_/_/_/    _/_/    _/    _/  _/_/_/    _/_/_/  _/      _/    _/_/_/\t";
		cout << text;
		int a2 = rand() % 15 + 1;
		color(a2);
		cout << "\n\n\n\n\n\n\t\t\t\t\t\t\t     ";
		for (int j = 1; j <= i; j++)
		{
			int a2 = rand() % 15 + 1;
			color(a2);
			cout << "|";
		}
		int a3 = rand() % 15 + 1;
		color(a3);
		//	color(15);
		cout << "\n\n\n\t\t\t\t\t\t\t\t\t\t    " << i * 2 << "%";
		VeKhungCH(59, 27, i + 3, 2, true);
		Sleep(10);
	}
	clear2();
	system("color F7");
}

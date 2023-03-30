#include "Game_Drunk_t.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <algorithm>

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void GoToXY(short x, short y) { SetConsoleCursorPosition(hStdOut, { x, y }); }

void ConsoleCursorVisible(bool show, short size) {
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hStdOut, &structCursorInfo);
	structCursorInfo.bVisible	= show;
	structCursorInfo.dwSize		= size;
	SetConsoleCursorInfo(hStdOut, &structCursorInfo);
}

void Show_Arrows(std::wstring* Arrows, const int SIZE) {
	GoToXY(2, 27);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
	for (int i = 0; i < SIZE; i++) {
		std::wcout << Arrows[i] << L" ";
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	auto fot_output	= _setmode(_fileno(stdout), _O_U16TEXT);
	auto for_input	= _setmode(_fileno(stdin), _O_U16TEXT);

	HWND hWindowConsole = GetConsoleWindow();
	RECT rect;
	GetWindowRect(hWindowConsole, &rect);
	MoveWindow(hWindowConsole, rect.left, rect.top, 500, 500, TRUE);
	SetWindowLong(hWindowConsole, GWL_STYLE, GetWindowLong(hWindowConsole, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	SetConsoleTitle(L"DRUNK");
	ConsoleCursorVisible(false, 100);

	std::wstring Menu[]		{ L"НАЧАТЬ ИГРУ", L"ПРАВИЛА", L"ВЫХОД" };
	std::wstring Options[]	{ L"КОМП", L"ДРУГ" };
	std::wstring Playning[]	{ L"2", L"3" };

	const int SIZE = 4;
	std::wstring Arrows[SIZE]{ L"\x2190", L"\x2191", L"\x2192", L"\x2193" };

	int active_menu		= 0;
	int active_options	= 0;
	int active_playning = 0;

	const char ESC		= 27;
	const char ENTER	= 13;
	const char UP		= 72;
	const char DOWN		= 80;
	const char LEFT		= 75;
	const char RIGHT	= 77;

	Game_Drunk_t GAME;

	char ch;
	while (true) {
		system("cls");
		int x = 25, y = 11;
		GoToXY(x, y);
		for (int i = 0; i < size(Menu); i++) {
			if (i == active_menu)
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			else
				SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);

			GoToXY(x, y++);
			std::wcout << Menu[i] << std::endl;
		}

		Show_Arrows(Arrows, SIZE);

		ch = _getch();
		if (ch == -32) ch = _getch();
		switch (ch) {
		case ESC:
			exit(0);

		case UP:
			if (active_menu > 0)
				--active_menu;
			break;

		case DOWN:
			if (active_menu < size(Menu) - 1)
				++active_menu;
			break;

		case ENTER:
			switch (active_menu) {
			case 0:
				while (true) {
					if (active_menu == 0) {
						while (true) {
							if (active_menu > 0)
								break;
							system("cls");
							x = 25, y = 11;
							GoToXY(x, y);

							SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
							std::wcout << L"РЕЖИМ" << " ";
							SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
							std::wcout << Options[active_options] << std::endl;

							GoToXY(x, ++y);

							SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
							std::wcout << L"ИГРАЮТ" << " ";
							std::wcout << Playning[active_playning] << std::endl;

							GoToXY(x, ++y);
							if (active_menu == 2)
								SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
							else
								SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
							std::wcout << L"ПЬЁМ";

							Show_Arrows(Arrows, SIZE);

							ch = _getch();
							if (ch == -32) ch = _getch();

							switch (ch) {
							case LEFT:
								if (active_options > 0)
									--active_options;
								break;

							case RIGHT:
								if (active_options == 0)
									++active_options;
								break;

							case DOWN:
								if (active_menu < 2)
									++active_menu;
								break;

							case UP:
								if (active_menu > 0)
									--active_menu;
								break;
							}
						}
					}
					else {
						while (true) {
							if (active_menu == 0)
								break;
							system("cls");
							x = 25, y = 11;
							GoToXY(x, y);

							SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
							std::wcout << L"РЕЖИМ" << " ";
							std::wcout << Options[active_options] << std::endl;

							GoToXY(x, ++y);

							SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
							std::wcout << L"ИГРАЮТ" << " ";

							if (active_menu != 2)
								SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
							std::wcout << Playning[active_playning] << std::endl;

							GoToXY(x, ++y);
							if (active_menu == 2)
								SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
							else
								SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
							std::wcout << L"ПЬЁМ";

							Show_Arrows(Arrows, SIZE);
							SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

							ch = _getch();
							if (ch == -32) ch = _getch();

							switch (ch) {
							case LEFT:
								if (active_menu != 2) {
									if (active_playning > 0)
										--active_playning;
								}
								break;

							case RIGHT:
								if (active_menu != 2) {
									if (active_playning == 0)
										++active_playning;
								}
								break;

							case DOWN:
								if (active_menu < 2)
									active_menu++;
								break;

							case UP:
								if (active_menu > 0)
									active_menu--;
								break;

							case ENTER:
								if (active_menu == 2) {
									system("cls");

									GAME.SetPeople(active_options);
									GAME.SetQuantity_Players(active_playning + 2);
									GAME.Create_Players();
									GAME.Distribute_Cards();
									GAME.Game() == GAME.DRAW ? GAME.Show_Draw() : GAME.Show_Winner();

									system("PAUSE > null");
								}
							}
						}
					}
				}
			case 1:
				system("cls");
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				std::wcout << L"Добро пожаловать в бар! Сегодня ты постараешься не пропить все деньги, "
					L"ведь если они у тебя закончатся, то ты   -   пьяница. Карты - это деньги. "
					L"В начале их у всех поровну.\nВо время совместного пития кладём карты по одной. "
					L"Забирает все выложенные карты тот, у кого карты крупнее, и кладёт   себе в карман (сброс). "
					L"Так по кругу, пока не останется   один экономный богатей со всей колодой деньжат."
					L"\n\nЕсли выложенные карты одинаково крупные, объявляется спор. Спорящие кладут по одной"
					L"карте рубашкой вверх (заклад), а затем ещё по одной открыто. У кого карта больше - забирает себе все карты спора."
					L"\n\nЕсли у игрока нет карт для спора - он выигрывает спор.\nЕсли у игрока нет карт для заклада, то спор ведётся без"
					L"   заклада.\nПомни - шестёрка больше туза!"
					L"\n\nХороших тебе собутыльников!! Не пропей своё состояние!";

				Show_Arrows(Arrows, SIZE);
				system("PAUSE > null");
				break;
			case 2:
				exit(0);
			}
		}
	}

	system("PAUSE > null");

	return 0;
}
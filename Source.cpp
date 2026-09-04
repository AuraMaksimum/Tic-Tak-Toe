#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <ctime>

struct Console {
	HANDLE console;

	Console() {
		SetConsoleOutputCP(65001);
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		setCursor({ 100, FALSE });
	}

	void setCursor(CONSOLE_CURSOR_INFO cur) {
		SetConsoleCursorInfo(console, &cur);
	}

	void setCoord(COORD pos) {
		SetConsoleCursorPosition(console, pos);
	}

	COORD getCoord() {
		CONSOLE_SCREEN_BUFFER_INFO pos;
		return pos.dwCursorPosition;
	}

	void setColor(WORD screen, WORD text) {
		SetConsoleTextAttribute(console, (screen << 4) | text);
	}

	operator HANDLE() {
		return console;
	}

	void fill(CHAR znak = ' ', DWORD len = 1000, COORD pos = { 0, 1 }) {
		DWORD ignore;

		FillConsoleOutputCharacterA(console, znak, len, pos, &ignore);
		FillConsoleOutputAttribute(console, 7, len, pos, &ignore);
	}

	bool setFontSize(COORD size) {
		_CONSOLE_FONT_INFOEX info {};

		info.cbSize = sizeof info;
		info.dwFontSize = size;

		if (SetCurrentConsoleFontEx(console, FALSE, &info)) {
			return true;
		}

		return false;
	}

	COORD getFontSize() {
		_CONSOLE_FONT_INFOEX info {};
		info.cbSize = sizeof info;

		GetCurrentConsoleFontEx(console, FALSE, &info);
		return info.dwFontSize;
	}

};

Console console;

std::string menu[3] = {
	"K\xC3\xB3\xC5\x82ko i Krzy\xC5\xBCyk",
	"Gracz vs AI",
	"Gracz vs Gracz"
};

namespace Menu {
	
	void w(short& y) {
		console.setCoord({ 0, y });
		std::cout << "                                          \r";
		std::cout << menu[y];
		--y;

		if (y == 0) {
			y = 2;
		}

		console.setCoord({ 0, y });
		std::cout << "                                          \r";
		std::cout << ">" << menu[y];
	}

	void s(short& y) {
		console.setCoord({ 0, y });
		std::cout << "                                          \r";
		std::cout << menu[y];
		y = (y + 1) % 3;

		if (y == 0) {
			y = 1;
		}

		console.setCoord({ 0, y });
		std::cout << "                                          \r";
		std::cout << ">" << menu[y];
	}

	void print(short& y) {
		console.setCoord({ 0, 0 });

		for (int i = 0; i < 3; i++) {
			std::cout << "                                                    \r";

			if (i == y) {
				std::cout << ">";
			}

			std::cout << menu[i] << '\n';
		}

		console.setCoord({ 0, 0 });
		console.setColor(0, 3);
		std::cout << "K\xC3\xB3\xC5\x82ko";
		console.setColor(0, 7);
		std::cout << " i ";
		console.setColor(0, 4);
		std::cout << "Krzy\xC5\xBCyk";
		console.setColor(0, 7);
	}

}

namespace game {
	//0 - kółko
	//1 - krzyżyk

	bool gamer(byte mat[3][3], bool player) {
		console.setCoord({ 1, 4 });

		bool cur = true;

		while (true) {
			console.setCoord({ 1, 4 });

			if (cur) {
				std::cout << "█";
			}
			else {
				std::cout << " ";
			}

			cur = !cur;
			std::cout.flush();
			Sleep(1000);
		}

		return true;
	}

	void play(byte type, bool(*oponent)(byte[3][3], bool), bool player) {
		byte mat[3][3] = { 0 };
		bool now = false;

		/*
			   |   |   
			---+---+---
			   |   |
			---+---+---
			   |   |
		*/

		console.fill();
		console.setCoord({ 0, 2 });
		std::cout << ">";
		console.setColor(0, 3);
		std::cout << "O\n";
		console.setColor(0, 7);
		std::cout << "\n";
		std::cout << "   |   |   \n";
		std::cout << "---+---+---\n";
		std::cout << "   |   |   \n";
		std::cout << "---+---+---\n";
		std::cout << "   |   |   ";
		std::cout.flush();

		while (true) {
			gamer(mat, now);
		}

	}

}

int main() {
	short y = 1;
	console.setCoord({ 0, 0 });
	char znak;

	Menu::print(y);
	std::cout.flush();
	game::play(0, game::gamer, false);
	while (true) {

		if (_kbhit() > 0) {
			znak = _getch();

			switch (znak) {

			case 'w':
				Menu::w(y);
				break;

			case 's':
				Menu::s(y);
				break;
				
			case 'W':
				Menu::w(y);
				break;

			case 'S':
				Menu::s(y);

			}

			std::cout.flush();
		}

		Sleep(1000 / 60);
	}

}
/*
* raw(?) I/O
*/
#include "io.h"
#include "common.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // 행, 열 반대로 전달
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int backcolor, int textcolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (backcolor << 4) + textcolor);
}


void set_color_cursor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printc(POSITION pos, char ch, int backcolor, int textcolor)
{
	if (backcolor >= 0) {
		set_color(backcolor, textcolor);
	}
	gotoxy(pos);
	printf("%c", ch);
}

void print_cursor(POSITION pos, char ch, int color)
{
	if (color >= 0) {
		set_color_cursor(color);
	}
	gotoxy(pos);
	printf("%c", ch);
}

KEY get_key(void) {
	if (!_kbhit())
	{  // 입력된 키가 있는지 확인
		return k_none;
	}

	int byte = _getch();    // 입력된 키를 전달 받기

	switch (byte)
	{
	case 'q':
		return k_quit;  // 'q'를 누르면 종료
	case 224:
		byte = _getch();  // MSB 224가 입력 되면 1바이트 더 전달 받기
		switch (byte)
		{
		case 72: return k_up;
		case 75: return k_left;
		case 77: return k_right;
		case 80: return k_down;
		case 32: return k_select;  // 스페이스바 입력 처리


		default: return k_undef;
		}
		
	case 32:
		return k_select;  // 스페이스바 입력 처리
	case 27:
		return k_cancel;  // ESC 처리

	default: return k_undef;
	}
}
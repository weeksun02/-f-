/*
* raw(?) I/O
*/
#include "io.h"
#include "common.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // ��, �� �ݴ�� ����
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
	{  // �Էµ� Ű�� �ִ��� Ȯ��
		return k_none;
	}

	int byte = _getch();    // �Էµ� Ű�� ���� �ޱ�

	switch (byte)
	{
	case 'q':
		return k_quit;  // 'q'�� ������ ����
	case 224:
		byte = _getch();  // MSB 224�� �Է� �Ǹ� 1����Ʈ �� ���� �ޱ�
		switch (byte)
		{
		case 72: return k_up;
		case 75: return k_left;
		case 77: return k_right;
		case 80: return k_down;
		case 32: return k_select;  // �����̽��� �Է� ó��


		default: return k_undef;
		}
		
	case 32:
		return k_select;  // �����̽��� �Է� ó��
	case 27:
		return k_cancel;  // ESC ó��

	default: return k_undef;
	}
}
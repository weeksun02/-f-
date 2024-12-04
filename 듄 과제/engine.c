#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);
void select_object(void);
void deselect_object(void);

/* ================= control =================== */
int sys_clock = 0;       // system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };
bool object_selected = false;  // ������Ʈ ���� ����
char selected_object = ' ';    // ���õ� ������Ʈ


/* ================= game data =================== */
//char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };

RESOURCE resource = {
    .spice = 0,
    .spice_max = 0,
    .population = 0,
    .population_max = 0
};

OBJECT_SAMPLE obj = {
    .pos = {1, 1},
    .dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
    .repr = "Iamsandworm",
    .speed = 300,           // 'speed' �ʵ� �߰�
    .next_move_time = 300
};

/* Ÿ�̸� ���� */
clock_t last_key_time = 0;
DIRECTION last_direction = d_stay;

/* ================= main() =================== */
int main(void) {
    srand((unsigned int)time(NULL));

    init();
    intro();
    display(resource, map, cursor);

    while (1)
    {
        KEY key = get_key();


        if (is_arrow_key(key))
        {
            cursor_move(ktod(key));
        }
        else if (is_arrow_key(key) == 0 && key != k_cancel)
        {
            switch (key)
            {
            case k_quit: outro();
            case k_select: select_object();
                break;  // �����̽��ٷ� ����

            case k_none:
            case k_undef:
            default: break;
            }
        }
        else if (key == k_cancel)// ESC�� ���� ���
        {
            deselect_object();
        }

        sample_obj_move();
        display(resource, map, cursor);
        Sleep(TICK);
        sys_clock += 10;
    }
}

/* ================= subfunctions =================== */
void intro(void) {
    printf("DUNE 1.5\n");
    Sleep(2000);
    system("cls");
}

void outro(void) {
    printf("exiting...\n");
    exit(0);
}

void init(void) {
    for (int j = 0; j < MAP_WIDTH; j++) {
        map[0][0][j] = '#';
        map[0][MAP_HEIGHT - 1][j] = '#';
    }

    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        map[0][i][0] = '#';
        map[0][i][MAP_WIDTH - 1] = '#';
        for (int j = 1; j < MAP_WIDTH - 1; j++) {
            map[0][i][j] = ' ';
        }
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[1][i][j] = -1;
        }
    }

    map[1][obj.pos.row][obj.pos.column] = "Iamsandworm";
}

void cursor_move(DIRECTION dir) {
    clock_t current_time = clock();
    double time_diff = (double)(current_time - last_key_time) / CLOCKS_PER_SEC;

    // �⺻ �̵� �Ÿ�: �� ĭ
    int move_distance = 1;

    // ���� �������� �� �� ���� �Է� �� �߰� �̵� (0.3�� �̳�)
    if (last_direction == dir && time_diff < 0.3) {
        move_distance = 7;  // �� �� Ŭ�� �� �̵� �Ÿ�: 3ĭ
    }

    // ���� ��ġ ����� (�������� �����)
    print_cursor(padd(map_pos, cursor.current), ' ', COLOR_CURSOR);

    // Ŀ�� �̵� ó��
    for (int i = 0; i < move_distance; i++) {
        POSITION curr = cursor.current;
        POSITION new_pos = pmove(curr, dir);

        if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
            1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

            cursor.previous = cursor.current;
            cursor.current = new_pos;
        }
    }

    // ������ �Է� �ð��� ���� ������Ʈ
    last_key_time = current_time;
    last_direction = dir;
}

/* ================= object selection =================== */
void select_object()
{
    char object = map[0][cursor.current.row][cursor.current.column];
    //  printf("Debug: Selected object at (%d, %d): %c\n", cursor.current.row, cursor.current.column, object);
      //char  input_bar = (POSITION)a;
    char c = "                                                          ";

    if (object == 'B' || object == 'W' || object == 'H' ||
        object == 'S' || object == 'P' || object == 'R') {
        object_selected = true;
        selected_object = object;
        gotoxy(status_window_pos);
        switch (object)
        {
        case 'B':


            printf("");
            printf("[����â]:���õ� ������Ʈ�� '����(B)'�Դϴ�    ");
            gotoxy(status_window_pos_2);
            printf("                                      ");
            gotoxy(command_window_pos_2);
            printf("H : �Ϻ����� ����(�Ǽ����X,������-50)         ");


            //  gotoxy(status_window_pos_2);                                          
            //  printf("asdfsdfsdfasdfsadfasdfasdf.");


            break;
        case 'W':
            printf("[����â] ���õ� ������Ʈ�� '�����(W)'�Դϴ�  ");
            gotoxy(status_window_pos_2);
            printf(":�縷��������& �����̽��� ������       ");

            gotoxy(command_window_pos_2);
            printf("                                               ");


            break;
        case 'H':
            printf("[����â]:���õ� ������Ʈ�� '�Ϻ�����(H)'�Դϴ�");
            gotoxy(status_window_pos_2);
            printf("                                       ");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        case 'S':
            printf("[Status]: Spice field selected.");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        case 'P':
            printf("");
            printf("[����â]:���õ� ������Ʈ�� '����(P)'�Դϴ�    ");
            gotoxy(status_window_pos_2);
            printf(":�ǹ������������                      ");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        case 'R':
            printf("[����â]:���õ� ������Ʈ�� '����(R)'�Դϴ�    ");
            gotoxy(status_window_pos_2);
            printf(":������� ����Ҽ� ����                ");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        default:
            printf("[����â]: Unknown object selected.");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        }
    }
    else {
        object_selected = false;
        selected_object = ' ';
        gotoxy(status_window_pos);

        printf("[����â]: '�縷 ����'�Դϴ�                       ");
        gotoxy(status_window_pos_2);
        printf(":�⺻����(��ĭ),�ǹ��� ������ ����        ");
        gotoxy(command_window_pos_2);
        printf("                                               ");
        //gotoxy(status_window_pos_2);
        //printf("[Status]: No valid object selected.");
    }
}

void deselect_object()
{
    object_selected = false;
    selected_object = ' ';
    gotoxy(status_window_pos);
    printf("[����â]:                                      ");
    gotoxy(status_window_pos_2);
    printf("                                       ");
}

POSITION sample_obj_next_position(void) {
    POSITION diff = psub(obj.dest, obj.pos);
    DIRECTION dir;

    // ��ǥ ������ �����ϸ� �������� ����
    if (diff.row == 0 && diff.column == 0) {
        obj.dest = (obj.dest.row == 1 && obj.dest.column == 1) ?
            (POSITION) {
            MAP_HEIGHT - 2, MAP_WIDTH - 2
        } :
            (POSITION) {
            1, 1
        };
            return obj.pos;  // ���� ��ġ �״�� ��ȯ
    }

    // �̵� ���� ���
    dir = (abs(diff.row) >= abs(diff.column)) ? (diff.row >= 0 ? d_down : d_up) :
        (diff.column >= 0 ? d_right : d_left);

    POSITION next_pos = pmove(obj.pos, dir);

    // ��ȿ�� ��ġ���� üũ
    if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
        1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
        map[1][next_pos.row][next_pos.column] < 0) {
        return next_pos;
    }
    else {
        return obj.pos;  // �̵� �Ұ����ϸ� ���� ��ġ �״�� ��ȯ
    }
}
void sample_obj_move(void) {
    if (sys_clock <= obj.next_move_time) return;

    // ���� ��ġ �����
    for (int i = 0; i < strlen(obj.repr); i++) {
        map[1][obj.pos.row][obj.pos.column + i] = -1; // ��ü�� ������ ������ ���
    }

    // �� ��ġ ���
    POSITION next_pos = sample_obj_next_position();

    // ��('#')�� �ִ��� Ȯ��
    for (int i = 0; i < strlen(obj.repr); i++) {
        if (map[1][next_pos.row][next_pos.column + i] == '#') {
            // ���� ������ �̵����� ����
            return;
        }
    }

    // ���� ������ �� ��ġ�� �̵�
    obj.pos = next_pos;

    // �� ��ġ�� repr ��� (�� ���ھ� ���)
    for (int i = 0; i < strlen(obj.repr); i++) {
        map[1][obj.pos.row][obj.pos.column + i] = obj.repr[i];  // ��ü�� �����ϴ� ������ ���� ���
    }

    // ���� �̵� �ð� ����
    obj.next_move_time = sys_clock + obj.speed;
}













/*
POSITION sample_obj_next_position(void) {
    POSITION diff = psub(obj.dest, obj.pos);
    DIRECTION dir;

    if (diff.row == 0 && diff.column == 0) {
        obj.dest = (obj.dest.row == 1 && obj.dest.column == 1) ?
            (POSITION) {
            MAP_HEIGHT - 2, MAP_WIDTH - 2
        } : (POSITION) { 1, 1 };
            return obj.pos;
    }

    dir = (abs(diff.row) >= abs(diff.column)) ? (diff.row >= 0 ? d_down : d_up) :
        (diff.column >= 0 ? d_right : d_left);

    POSITION next_pos = pmove(obj.pos, dir);
    if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
        1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
        map[1][next_pos.row][next_pos.column] < 0) {
        return next_pos;
    }
    else {
        return obj.pos;
    }
}





void sample_obj_move(void) {
    if (sys_clock <= obj.next_move_time) return;
   

    //////////////////////////////////////////////////////

    
    map[1][obj.pos.row][obj.pos.column] = -1;
    obj.pos = sample_obj_next_position();
    map[1][obj.pos.row][obj.pos.column] = obj.repr;
    obj.next_move_time = sys_clock + obj.speed;
    
}
*/

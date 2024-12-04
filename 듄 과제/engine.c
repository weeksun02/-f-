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
bool object_selected = false;  // 오브젝트 선택 여부
char selected_object = ' ';    // 선택된 오브젝트


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
    .speed = 300,           // 'speed' 필드 추가
    .next_move_time = 300
};

/* 타이머 설정 */
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
                break;  // 스페이스바로 선택

            case k_none:
            case k_undef:
            default: break;
            }
        }
        else if (key == k_cancel)// ESC로 선택 취소
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

    // 기본 이동 거리: 한 칸
    int move_distance = 1;

    // 같은 방향으로 두 번 연속 입력 시 추가 이동 (0.3초 이내)
    if (last_direction == dir && time_diff < 0.3) {
        move_distance = 7;  // 두 번 클릭 시 이동 거리: 3칸
    }

    // 이전 위치 지우기 (공백으로 덮어쓰기)
    print_cursor(padd(map_pos, cursor.current), ' ', COLOR_CURSOR);

    // 커서 이동 처리
    for (int i = 0; i < move_distance; i++) {
        POSITION curr = cursor.current;
        POSITION new_pos = pmove(curr, dir);

        if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
            1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

            cursor.previous = cursor.current;
            cursor.current = new_pos;
        }
    }

    // 마지막 입력 시간과 방향 업데이트
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
            printf("[상태창]:선택된 오브젝트는 '본진(B)'입니다    ");
            gotoxy(status_window_pos_2);
            printf("                                      ");
            gotoxy(command_window_pos_2);
            printf("H : 하베스터 생산(건설비용X,내구도-50)         ");


            //  gotoxy(status_window_pos_2);                                          
            //  printf("asdfsdfsdfasdfsadfasdfasdf.");


            break;
        case 'W':
            printf("[상태창] 선택된 오브젝트는 '샌드웜(W)'입니다  ");
            gotoxy(status_window_pos_2);
            printf(":사막의포식자& 스파이스의 생산자       ");

            gotoxy(command_window_pos_2);
            printf("                                               ");


            break;
        case 'H':
            printf("[상태창]:선택된 오브젝트는 '하베스터(H)'입니다");
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
            printf("[상태창]:선택된 오브젝트는 '장판(P)'입니다    ");
            gotoxy(status_window_pos_2);
            printf(":건물짓기전에깔기                      ");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        case 'R':
            printf("[상태창]:선택된 오브젝트는 '바위(R)'입니다    ");
            gotoxy(status_window_pos_2);
            printf(":샌드웜은 통과할수 없음                ");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        default:
            printf("[상태창]: Unknown object selected.");
            gotoxy(command_window_pos_2);
            printf("                                               ");
            break;
        }
    }
    else {
        object_selected = false;
        selected_object = ' ';
        gotoxy(status_window_pos);

        printf("[상태창]: '사막 지형'입니다                       ");
        gotoxy(status_window_pos_2);
        printf(":기본지형(빈칸),건물을 지을수 없음        ");
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
    printf("[상태창]:                                      ");
    gotoxy(status_window_pos_2);
    printf("                                       ");
}

POSITION sample_obj_next_position(void) {
    POSITION diff = psub(obj.dest, obj.pos);
    DIRECTION dir;

    // 목표 지점에 도달하면 목적지를 변경
    if (diff.row == 0 && diff.column == 0) {
        obj.dest = (obj.dest.row == 1 && obj.dest.column == 1) ?
            (POSITION) {
            MAP_HEIGHT - 2, MAP_WIDTH - 2
        } :
            (POSITION) {
            1, 1
        };
            return obj.pos;  // 현재 위치 그대로 반환
    }

    // 이동 방향 계산
    dir = (abs(diff.row) >= abs(diff.column)) ? (diff.row >= 0 ? d_down : d_up) :
        (diff.column >= 0 ? d_right : d_left);

    POSITION next_pos = pmove(obj.pos, dir);

    // 유효한 위치인지 체크
    if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
        1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
        map[1][next_pos.row][next_pos.column] < 0) {
        return next_pos;
    }
    else {
        return obj.pos;  // 이동 불가능하면 현재 위치 그대로 반환
    }
}
void sample_obj_move(void) {
    if (sys_clock <= obj.next_move_time) return;

    // 이전 위치 지우기
    for (int i = 0; i < strlen(obj.repr); i++) {
        map[1][obj.pos.row][obj.pos.column + i] = -1; // 객체가 차지한 공간을 비움
    }

    // 새 위치 계산
    POSITION next_pos = sample_obj_next_position();

    // 벽('#')이 있는지 확인
    for (int i = 0; i < strlen(obj.repr); i++) {
        if (map[1][next_pos.row][next_pos.column + i] == '#') {
            // 벽에 닿으면 이동하지 않음
            return;
        }
    }

    // 벽이 없으면 새 위치로 이동
    obj.pos = next_pos;

    // 새 위치에 repr 출력 (한 문자씩 출력)
    for (int i = 0; i < strlen(obj.repr); i++) {
        map[1][obj.pos.row][obj.pos.column + i] = obj.repr[i];  // 객체가 차지하는 공간에 문자 출력
    }

    // 다음 이동 시간 설정
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

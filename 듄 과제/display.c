#include "display.h"
#include "io.h"

// 좌상단 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };
const POSITION system_message_pos = { MAP_HEIGHT + 1, 0 };
const POSITION command_window_pos = { MAP_HEIGHT + 1, MAP_WIDTH + 5 };
const POSITION command_window_pos_2 = { MAP_HEIGHT + 2, MAP_WIDTH + 5 };
const POSITION status_window_pos = { 1, MAP_WIDTH + 5 };
const POSITION status_window_pos_2 = { 2, MAP_WIDTH + 13 };

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
ColorKinds colorkinds;

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);

void display(
    RESOURCE resource,
    char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
    CURSOR cursor)
{
    
    //////////////////////////
    
    display_resource(resource);
    display_map(map);
    
    display_cursor(cursor);
    
    set_color(white, black);
    gotoxy(system_message_pos);
    printf("[시스템 메세지]: ");

    gotoxy(command_window_pos);
    printf("[명량창]:");

    gotoxy(status_window_pos);
    if (object_selected)
    {
        printf("[상태창]:");
        // printf("[Status]: Object '%c' selected", selected_object);
    }
    else {
        printf("[Status]: ");
        //printf("[Status]: No object selected");
    }

}

void display_resource(RESOURCE resource) {
    // 플레이어 본부 B (좌하단)
 //   set_color(COLOR_B);
    gotoxy(padd(resource_pos, (POSITION) { 17, 1 }));
    map[0][16][1] = 'B';
    //printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 17, 2 }));
    map[0][16][2] = 'B';
    //printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 16, 1 }));
    map[0][15][1] = 'B';
    //printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 16, 2 }));
    map[0][15][2] = 'B';
    //printf("B");

    // 플레이어 하베스터 H (좌하단)
  //  set_color(COLOR_B);
    gotoxy(padd(resource_pos, (POSITION) { 15, 1 }));
    map[0][14][1] = 'H';
    //printf("H");

    // 장판 P (좌하단, 2x2)
 //   set_color(COLOR_P);
    gotoxy(padd(resource_pos, (POSITION) { 16, 3 }));
    map[0][15][3] = 'P';
    //printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 16, 4 }));
    map[0][15][4] = 'P';
    //printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 17, 3 }));
    map[0][16][3] = 'P';
    //printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 17, 4 }));
    map[0][16][4] = 'P';
    //printf("P");

    /* 적 본부 B(우상단) */
  //  set_color(COLOR_BB);
    gotoxy(padd(resource_pos, (POSITION) { 2, 57 }));
    //printf("B");
    map[0][1][57] = 'B';
    gotoxy(padd(resource_pos, (POSITION) { 3, 57 }));
    //printf("B");
    map[0][2][57] = 'B';
    gotoxy(padd(resource_pos, (POSITION) { 2, 58 }));
    //printf("B");
    map[0][1][58] = 'B';
    gotoxy(padd(resource_pos, (POSITION) { 3, 58 }));
    //printf("B");
    map[0][2][58] = 'B';

    /* 장판 P (우상단, 2x2) */
 //   set_color(COLOR_P);
    gotoxy(padd(resource_pos, (POSITION) { 2, 55 }));
    map[0][1][55] = 'P';
    //printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 3, 55 }));
    map[0][2][55] = 'P';
    //printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 2, 56 }));
    map[0][1][56] = 'P';
    //printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 3, 56 }));
    map[0][2][56] = 'P';
    //printf("P");

    // 적 하베스터 H (우상단)
//    set_color(COLOR_BB);
    gotoxy(padd(resource_pos, (POSITION) { 4, 58 }));
    map[0][3][58] = 'H';
    //printf("H");

    // 스파이스 매장지 5
 //   set_color(COLOR_5);
    gotoxy(padd(resource_pos, (POSITION) { 13, 1 }));
    map[0][12][1] = '5';
    //printf("5");
    gotoxy(padd(resource_pos, (POSITION) { 6, 58 }));
    map[0][5][58] = '5';
    //printf("5");

    // 샌드웜 W
 //   set_color(COLOR_W);
    gotoxy(padd(resource_pos, (POSITION) { 3, 9 }));
    // printf("W");
    map[0][2][9] = 'W';
    gotoxy(padd(resource_pos, (POSITION) { 14, 44 }));
    //  printf("W");
    map[0][13][44] = 'W';

    // 바위 R (2x2 및 1x1)
 //   set_color(COLOR_R);
    gotoxy(padd(resource_pos, (POSITION) { 6, 19 }));
    map[0][5][19] = 'R';
    //printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 6, 20 }));
    map[0][5][20] = 'R';
    //printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 7, 19 }));
    map[0][6][19] = 'R';
    //printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 7, 20 }));
    map[0][6][20] = 'R';
    //printf("R");
    //
    gotoxy(padd(resource_pos, (POSITION) { 15, 28 }));
    map[0][14][28] = 'R';
    //printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 15, 27 }));
    map[0][14][27] = 'R';
    //printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 16, 28 }));
    map[0][15][28] = 'R';
    //printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 16, 27 }));
    map[0][15][27] = 'R';
    //printf("R");
    //
    gotoxy(padd(resource_pos, (POSITION) { 13, 14 }));
    map[0][12][14] = 'R';
    //printf("R");
    //
    gotoxy(padd(resource_pos, (POSITION) { 15, 54 }));
    map[0][14][54] = 'R';
    //printf("R");

    gotoxy(padd(resource_pos, (POSITION) { 5, 52 }));
    map[0][4][52] = 'R';
    //printf("R");

    set_color_cursor(COLOR_DEFAULT);
}

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            for (int k = 0; k < N_LAYER; k++) {
                if (src[k][i][j] >= 0) {
                    dest[i][j] = src[k][i][j];
                }
            }
        }
    }
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH])
{
    project(map, backbuf);

    int back_color = 0;


    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (frontbuf[i][j] != backbuf[i][j])
            {
                POSITION pos = { i, j };

                switch (backbuf[i][j])
                {
                case 'B':
                    back_color = skyblue;
                    printc(padd(map_pos, pos), backbuf[i][j], back_color, white);

                    break;
                case 'H':
                    back_color = sky;
                    printc(padd(map_pos, pos), backbuf[i][j], back_color, white);
                    break;
                case 'P':
                    back_color = gray;
                    printc(padd(map_pos, pos), backbuf[i][j], back_color, white);
                    break;
                case '5':
                    back_color = orange;
                    printc(padd(map_pos, pos), backbuf[i][j], back_color, white);
                    break;
                case 'W':
                    back_color = brightyellow;
                    printc(padd(map_pos, pos), backbuf[i][j], back_color, white);
                    break;
                case 'R':
                    back_color = gray;
                    printc(padd(map_pos, pos), backbuf[i][j], back_color, white);
                    break;
                default:
                    back_color = white;
                    printc(padd(map_pos, pos), backbuf[i][j], white, black);
                    frontbuf[i][j] = backbuf[i][j];
                    break;
                }

                //  printc(padd(map_pos, pos), backbuf[i][j], back_color, black);


            }

        }
    }
}

void display_cursor(CURSOR cursor) {
    POSITION prev = cursor.previous;
    POSITION curr = cursor.current;

    // SetConsoleCursor

    char ch = frontbuf[prev.row][prev.column];
    print_cursor(padd(map_pos, prev), ch, COLOR_CURSOR); //COLOR_DEFAULT

    ch = frontbuf[curr.row][curr.column];
    print_cursor(padd(map_pos, curr), ch, COLOR_DEFAULT); //COLOR_CURSOR

}
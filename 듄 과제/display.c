#include "display.h"
#include "io.h"

// 좌상단 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };
const POSITION system_message_pos = { MAP_HEIGHT + 1, 0 };
const POSITION command_window_pos = { MAP_HEIGHT + 1, MAP_WIDTH + 5 };
const POSITION status_window_pos = { 0, MAP_WIDTH + 5 };

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);

void display(
    RESOURCE resource,
    char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
    CURSOR cursor)
{
    display_resource(resource);
    display_map(map);
    display_cursor(cursor);

    gotoxy(system_message_pos);
    printf("[System Message]: ...");

    gotoxy(command_window_pos);
    printf("[Commands]: ...");

    gotoxy(status_window_pos);
    printf("[Status]: Selected unit info ...");
}

void display_resource(RESOURCE resource) {
    // 플레이어 본부 B (좌하단)
    set_color(COLOR_B);
    gotoxy(padd(resource_pos, (POSITION) { 17, 1 }));
    printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 17, 2 }));
    printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 16, 1 }));
    printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 16, 2 }));
    printf("B");

    // 플레이어 하베스터 H (좌하단)
    set_color(COLOR_B);
    gotoxy(padd(resource_pos, (POSITION) { 15, 1 }));
    printf("H");

    // 장판 P (좌하단, 2x2)
    set_color(COLOR_P);
    gotoxy(padd(resource_pos, (POSITION) { 16, 3 }));
    printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 16, 4 }));
    printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 17, 3 }));
    printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 17, 4 }));
    printf("P");

    // 적 본부 B (우상단)
    set_color(COLOR_BB);
    gotoxy(padd(resource_pos, (POSITION) { 2, 57 }));
    printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 3, 57 }));
    printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 2, 58 }));
    printf("B");
    gotoxy(padd(resource_pos, (POSITION) { 3, 58 }));
    printf("B");
    // 장판 P (우상단, 2x2)
    set_color(COLOR_P);
    gotoxy(padd(resource_pos, (POSITION) { 2, 55 }));
    printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 3, 55 }));
    printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 2, 56 }));
    printf("P");
    gotoxy(padd(resource_pos, (POSITION) { 3, 56 }));
    printf("P");

    // 적 하베스터 H (우상단)
    set_color(COLOR_BB);
    gotoxy(padd(resource_pos, (POSITION) { 4, 58 }));
    printf("H");

    // 스파이스 매장지 5
    set_color(COLOR_5);
    gotoxy(padd(resource_pos, (POSITION) { 13, 1 }));
    printf("5");
    gotoxy(padd(resource_pos, (POSITION) { 6, 58 }));
    printf("5");

    // 샌드웜 W
    set_color(COLOR_W);
    gotoxy(padd(resource_pos, (POSITION) { 3, 9 }));
    printf("W");
    gotoxy(padd(resource_pos, (POSITION) { 14, 44 }));
    printf("W");

    // 바위 R (2x2 및 1x1)
    set_color(COLOR_R);
    gotoxy(padd(resource_pos, (POSITION) { 6, 19 }));
    printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 6, 20 }));
    printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 7, 19 }));
    printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 7, 20 }));
    printf("R");
    //
    gotoxy(padd(resource_pos, (POSITION) { 15, 28 }));
    printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 15, 27 }));
    printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 16, 28 }));
    printf("R");
    gotoxy(padd(resource_pos, (POSITION) { 16, 27 }));
    printf("R");
    //
    gotoxy(padd(resource_pos, (POSITION) { 13, 14 }));
    printf("R");
    //
    gotoxy(padd(resource_pos, (POSITION) { 15, 54 }));
    printf("R");

    gotoxy(padd(resource_pos, (POSITION) { 5, 52 }));
    printf("R");

    set_color(COLOR_DEFAULT);
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

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
    project(map, backbuf);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (frontbuf[i][j] != backbuf[i][j]) {
                POSITION pos = { i, j };
                printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
            }
            frontbuf[i][j] = backbuf[i][j];
        }
    }
}

void display_cursor(CURSOR cursor) {
    POSITION prev = cursor.previous;
    POSITION curr = cursor.current;

    char ch = frontbuf[prev.row][prev.column];
    printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

    ch = frontbuf[curr.row][curr.column];
    printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}

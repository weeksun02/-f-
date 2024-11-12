#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <assert.h>

/* ================= system parameters =================== */
#define TICK 10        // time unit(ms)

#define N_LAYER 2
#define MAP_WIDTH    60
#define MAP_HEIGHT   18


//
extern bool object_selected;
extern char selected_object;

//
/* ================= 위치와 방향 =================== */
typedef struct {
    int row, column;
} POSITION;

typedef struct {
    POSITION previous;
    POSITION current;
} CURSOR;

typedef enum {
    k_none = 0, k_up, k_right, k_left, k_down,
    k_quit, k_select,  // 스페이스바로 선택
    k_cancel,
    k_undef,
} KEY;

typedef enum {
    d_stay = 0, d_up, d_right, d_left, d_down
} DIRECTION;

inline POSITION padd(POSITION p1, POSITION p2) {
    POSITION p = { p1.row + p2.row, p1.column + p2.column };
    return p;
}

inline POSITION psub(POSITION p1, POSITION p2) {
    POSITION p = { p1.row - p2.row, p1.column - p2.column };
    return p;
}

#define is_arrow_key(k)        (k_up <= (k) && (k) <= k_down)
#define ktod(k)        (DIRECTION)(k)
inline POSITION dtop(DIRECTION d) {
    static POSITION direction_vector[] = { {0, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 0} };
    return direction_vector[d];
}
#define pmove(p, d)        (padd((p), dtop(d)))

typedef struct {
    int spice;
    int spice_max;
    int population;
    int population_max;
} RESOURCE;

typedef struct {
    POSITION pos;
    POSITION dest;
    char repr;
    int speed;
    int next_move_time;
} OBJECT_SAMPLE;

#endif

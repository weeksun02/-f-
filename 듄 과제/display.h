#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

#define COLOR_DEFAULT    15
#define COLOR_CURSOR     112
#define COLOR_RESOURCE   112
#define COLOR_R          8   // Grey color
#define COLOR_W          14  // Yellow color
#define COLOR_P          8   // Dark Grey color
#define COLOR_B          9   // Blue color
#define COLOR_BB         4   // Blue color
#define COLOR_H          1   // Light Blue color
#define COLOR_5          12  // Orange color

extern const POSITION resource_pos;
extern const POSITION system_message_pos;
extern const POSITION command_window_pos;
extern const POSITION status_window_pos;
extern const POSITION map_pos; // display.h¿¡ ¼±¾ð


void display(
    RESOURCE resource,
    char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
    CURSOR cursor
);

#endif

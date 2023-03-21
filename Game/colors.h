//
// Created by rymo on 11/23/22.
//

#ifndef RESET
#define RESET 0
#endif

#ifndef BLUE
#define BLUE 2          /* Blue */
#endif

#ifndef BLACK
#define BLACK   3      /* Black */
#endif

#ifndef RED
#define RED     4      /* Red */
#endif

#ifndef GREEN
#define GREEN   5      /* Green */
#endif

#ifndef YELLOW
#define YELLOW  5      /* Yellow */
#endif

#ifndef MAGENTA
#define MAGENTA 6      /* Magenta */
#endif

#ifndef CYAN
#define CYAN    7      /* Cyan */
#endif

#ifndef WHITE
#define WHITE   RESET      /* White */
#endif

#ifndef START_COLORS
#define START_COLORS { \
start_color();                             \
init_pair(RESET, COLOR_WHITE,COLOR_BLACK); \
init_pair(BLUE,COLOR_BLUE,COLOR_BLACK);    \
init_pair(BLACK,COLOR_BLACK,COLOR_BLACK);  \
init_pair(RED,COLOR_RED,COLOR_BLACK);      \
init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);  \
init_pair(YELLOW,COLOR_YELLOW,COLOR_BLACK);\
init_pair(MAGENTA,COLOR_MAGENTA,COLOR_BLACK); \
init_pair(CYAN,COLOR_CYAN,COLOR_BLACK);    \
}
#endif
#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "ansi.h"
#include "commondefs.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#pragma warning(disable: 4996)

#define LEVEL_COUNT_MIN 1
#define LEVEL_COUNT_MAX 5

#define MAP_SIZE_X 30u
#define MAP_SIZE_Y 33u

#define GHOST_COUNT 4

#define FG_COMMENT ANSI_FG_RGB(130, 170, 255)
#define FG_PROMPT  ANSI_FG_RGB(137, 221, 255)
#define FG_WARNING ANSI_FG_RGB(078, 201, 176)
#define FG_ERROR   ANSI_FG_RGB(255, 102, 102)
#define FG_NUMBER  ANSI_FG_RGB(235, 118, 105)
#define FG_OPTION  ANSI_FG_RGB(255, 203, 107)

#define __PREPEND_INFO_SYMBOL(current)   L"#   " current
#define __PREPEND_PROMPT_SYMBOL(current) L">   " current
#define __PREPEND_ISSUE_SYMBOL(current)  L"~   " current
#define __PREPEND_ERROR_SYMBOL(current)  L"!   " current
#define CONSOLE_INFO(...)       ANSI_SET_FG(FG_COMMENT, SELECT(__PREPEND_INFO_SYMBOL, __VA_ARGS__))
#define CONSOLE_PROMPT(...)     ANSI_SET_FG(FG_PROMPT,  SELECT(__PREPEND_PROMPT_SYMBOL, __VA_ARGS__)) ANSI_CREATE_ATTR(FG_PROMPT)
#define CONSOLE_ISSUE(...)      ANSI_SET_FG(FG_WARNING, SELECT(__PREPEND_ISSUE_SYMBOL, __VA_ARGS__))
#define CONSOLE_ERROR(...)      ANSI_SET_FG(FG_ERROR,   SELECT(__PREPEND_ERROR_SYMBOL, __VA_ARGS__))  ANSI_CREATE_ATTR(FG_PROMPT)
#define CONSOLE_NUMBER(context) ANSI_SET_FG(FG_NUMBER, context)
#define CONSOLE_OPTION(context) ANSI_SET_FG(FG_OPTION, context)

#define __CLEAR_LOG                  ANSI_CURSOR_MOVE_BEGINNING_OF_PREVIOUS() ANSI_CLEAR_FORWARD()
#define FEATURE_WORK_IN_PROGRESS     __CLEAR_LOG CONSOLE_ISSUE(L"This feature is a work in progress, please try again: ")
#define UNKNOWN_CONVERSION_SPECIFIER __CLEAR_LOG CONSOLE_ISSUE(L"Unknown conversion specifier. ")
#define TOO_FEW_ARGUMENTS            __CLEAR_LOG CONSOLE_ERROR(L"Too few arguments, please try again: ")
#define TOO_MANY_ARGUMENTS           __CLEAR_LOG CONSOLE_ERROR(L"Too many arguments, please try again: ")
#define ARGUMENT_TOO_LONG            __CLEAR_LOG CONSOLE_ERROR(L"Argument too long, please try again: ")
#define INVALID_NUMBER_FORMAT        __CLEAR_LOG CONSOLE_ERROR(L"Invalid number format, please try again: ")
#define INVALID_OPTION               __CLEAR_LOG CONSOLE_ERROR(L"Invalid option, please try again: ")
#define VALUE_OUT_OF_RANGE           __CLEAR_LOG CONSOLE_ERROR(L"Value out of the provided range, please try again: ")
#define LEVEL_NOT_AVAILABLE          __CLEAR_LOG CONSOLE_ERROR(L"%u score in level %u is required to unlock this level, please try again: ")

#define BG_GREY_1 ANSI_BG_RGB(020, 020, 050)
#define BG_GREY_2 ANSI_BG_RGB(015, 005, 015)
#define BG_GREY_3 ANSI_BG_RGB(025, 020, 025)
#define BG_GREY_4 ANSI_BG_RGB(005, 025, 005)
#define BG_GREY_5 ANSI_BG_RGB(015, 015, 025)

#define FG_BORDER_1 ANSI_FG_RGB(019, 056, 194)
#define FG_BORDER_2 ANSI_FG_RGB(050, 033, 122)
#define FG_BORDER_3 ANSI_FG_RGB(114, 088, 130)
#define FG_BORDER_4 ANSI_FG_RGB(020, 120, 081)
#define FG_BORDER_5 ANSI_FG_RGB(060, 120, 160)
#define FG_DOOR     ANSI_FG_RGB(200, 200, 200)
#define FG_COIN     ANSI_FG_RGB(220, 156, 127)
#define FG_PACMAN   ANSI_FG_RGB(254, 254, 000)

typedef enum _Tile
{
    TILE_ERROR,
    TILE_EMPTY,
    TILE_HOME,
    TILE_WALL,
    TILE_DOOR,

    TILE_COIN,
    TILE_POWERUP,
    TILE_FRUIT_CHERRY,
    TILE_FRUIT_STRAWBERRY,
    TILE_FRUIT_ORANGE,
    TILE_FRUIT_APPLE,
    TILE_FRUIT_MELON,
    TILE_FRUIT_GALAXIAN,
    TILE_FRUIT_BELL,
    TILE_FRUIT_KEY,
} Tile;

typedef struct _Pacman
{
    Vector2LongDouble home;
    Vector2LongDouble position;
    long double speed;
    Vector2Int32 direction;
} Pacman;

typedef enum _GhostMode
{
    GHOST_MODE_SCATTER,
    GHOST_MODE_CHASE,
} GhostMode;

typedef enum _GhostState
{
    GHOST_STATE_NORMAL,
    GHOST_STATE_FRIGHTENED,
    GHOST_STATE_EATEN,
} GhostState;

typedef struct _Ghost
{
    Vector2LongDouble home;
    Vector2LongDouble position;
    Vector2Int32 direction;
    Vector2LongDouble old_tile;
    GhostState state;
} Ghost;

typedef void (*Routine)(void);

typedef struct _Coroutine
{
    clock_t start_time;
    long double duration;
    Routine routine;
} Coroutine;

errno_t pacman(void);

int32_t console_scan_input(const char *format, ...);

int32_t program_coroutine_register(long double duration, Routine routine);

void program_coroutine_unregister(uint32_t index);

void program_coroutine_trigger_all(clock_t now);

void program_coroutine_unregister_all(void);

void pacman_commence(void);

void pacman_loading_screen(void);

bool pacman_main_menu(void);

void pacman_scan_level(void);

void pacman_show_highest_scores(void);

void pacman_terminate(void);

void game_map_initialize_level(void);

void game_map_initialize_round(void);

void game_map_initialize_actors(void);

void game_save_progress(void);

void game_draw_symbol(uint32_t x, uint32_t y, const wchar_t *const symbol);

void game_draw_tile(uint32_t x, uint32_t y);

void game_draw_map(void);

void game_draw_top_bar(void);

void game_pacman_move(long double delta_time, uint32_t *current_coin_count);

bool game_pacman_check_coins(void);

bool game_pacman_check_collision(void);

void game_ghosts_move(long double delta_time);

void game_ghost_reverse_direction(uint32_t index);

void game_ghost_mode_alternate(void);

void game_ghosts_frighten(void);

void game_fruit_cycle(void);

void game_fruit_spawn(void);

void game_fruit_despawn(void);

uint32_t game_control_flow(void);

void game_close(void);

#endif

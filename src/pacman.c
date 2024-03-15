#include "ansi.h"
#include "commondefs.h"
#include "macro_utils.h"
#include <math.h>
#include <windows.h>
#include "pacman.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int32_t current_level = LEVEL_COUNT_MIN;
static uint32_t unlocked_levels = LEVEL_COUNT_MIN + 1;

static Tile tiles[MAP_SIZE_X * MAP_SIZE_Y];

static const wchar_t *const level_map_foreground_colors[LEVEL_COUNT_MAX] = { FG_BORDER_1, FG_BORDER_2, FG_BORDER_3, FG_BORDER_4, FG_BORDER_5 };
static const wchar_t *const level_map_background_colors[LEVEL_COUNT_MAX] = { BG_GREY_1, BG_GREY_2, BG_GREY_3, BG_GREY_4, BG_GREY_5 };
static const wchar_t *const level_maps[LEVEL_COUNT_MAX] =
{
    {
        L"##############################"
        L"##############################"
        L"##************##************##"
        L"##*####*#####*##*#####*####*##"
        L"##0####*#####*##*#####*####0##"
        L"##*####*#####*##*#####*####*##"
        L"##**************************##"
        L"##*####*##*########*##*####*##"
        L"##*####*##*########*##*####*##"
        L"##******##****##****##******##"
        L"#######*##### ## #####*#######"
        L"#######*##### ## #####*#######"
        L"     ##*##          ##*##     "
        L"#######*## ###--### ##*#######"
        L"#######*## ###HH### ##*#######"
        L"       *   ##CBAD##   *       "
        L"#######*## ######## ##*#######"
        L"#######*## ######## ##*#######"
        L"     ##*##     F    ##*##     "
        L"#######*## ######## ##*#######"
        L"#######*## ######## ##*#######"
        L"##************##************##"
        L"##*####*#####*##*#####*####*##"
        L"##*####*#####*##*#####*####*##"
        L"##0**##       P        ##**0##"
        L"####*##*##*########*##*##*####"
        L"####*##*##*########*##*##*####"
        L"##******##****##****##******##"
        L"##*##########*##*##########*##"
        L"##*##########*##*##########*##"
        L"##**************************##"
        L"##############################"
        L"##############################"
    },
    {
        L"##############################"
        L"##############################"
        L"##******##**********##******##"
        L"##0####*##*########*##*####0##"
        L"##*####*##*########*##*####*##"
        L"##**************************##"
        L"####*##*#####*##*#####*##*####"
        L"####*##*#####*##*#####*##*####"
        L"####*##*#####*##*#####*##*####"
        L"    *##*******##*******##*    "
        L"####*##### ######## #####*####"
        L"####*##### ######## #####*####"
        L"  ##*                    *##  "
        L"  ##*##### ###--### #####*##  "
        L"  ##*##### ###HH### #####*##  "
        L"  ##*##*   ##CBAD##   *##*##  "
        L"####*##*## ######## ##*##*####"
        L"####*##*## ######## ##*##*####"
        L"    ****##     F    ##****    "
        L"####*######## ## ########*####"
        L"####*######## ## ########*####"
        L"  ##**********##**********##  "
        L"####*#####*########*#####*####"
        L"####*#####*########*#####*####"
        L"##******      P       ******##"
        L"##*####*#####*##*#####*####*##"
        L"##*####*#####*##*#####*####*##"
        L"##*####*##****##****##*####*##"
        L"##*####*##*########*##*####*##"
        L"##0####*##*########*##*####0##"
        L"##**************************##"
        L"##############################"
        L"##############################"
    },
    {
        L"##############################"
        L"##############################"
        L"##************##************##"
        L"##*##*#######*##*#######*##*##"
        L"##*##*#######*##*#######*##*##"
        L"##0##******##*##*##******##0##"
        L"##*####*##*##****##*##*####*##"
        L"##*####*##*##*##*##*##*####*##"
        L"##******##*##*##*##*##******##"
        L"####*#####****##****#####*####"
        L"####*##### ######## #####*####"
        L"  ##****## ######## ##****##  "
        L"  ## ##*##          ##*## ##  "
        L"#### ##*## ###--### ##*## ####"
        L"#### ##*## ###HH### ##*## ####"
        L"     ##*   ##CBAD##   *##     "
        L"########## ######## ##########"
        L"########## ######## ##########"
        L"     ##*       F      *##     "
        L"#### ##*## ######## ##*## ####"
        L"#### ##*## ######## ##*## ####"
        L"  ##****##****##****##****##  "
        L"  ##*#####*##*##*##*#####*##  "
        L"  ##*#####*##*##*##*#####*##  "
        L"  ##*******## P  ##*******##  "
        L"####*##*#####*##*#####*##*####"
        L"####*##*#####*##*#####*##*####"
        L"##***##*******##*******##***##"
        L"##0####*##*########*##*####0##"
        L"##*####*##*########*##*####*##"
        L"##******##**********##******##"
        L"##############################"
        L"##############################"
    },
    {
        L"##############################"
        L"##############################"
        L"##**************************##"
        L"##*##########*##*##########*##"
        L"##0##########*##*##########0##"
        L"##******##****##****##******##"
        L"##*####*##*########*##*####*##"
        L"##*####*##*########*##*####*##"
        L"##*##********************##*##"
        L"##*##*#### ######## ####*##*##"
        L"##*##*#### ######## ####*##*##"
        L"##******## ######## ##******##"
        L"##*####*##          ##*####*##"
        L"##*####*## ###--### ##*####*##"
        L"##***##*   ###HH###   *##***##"
        L"####*##*## ##CBAD## ##*##*####"
        L"####*##*## ######## ##*##*####"
        L"  ##*##*## ######## ##*##*##  "
        L"  ##****##     F    ##****##  "
        L"  ##*##### ######## #####*##  "
        L"  ##*##### ######## #####*##  "
        L"  ##**********##**********##  "
        L"####*########*##*########*####"
        L"####*########*##*########*####"
        L"    ****##    P     ##****    "
        L"####*##*##*########*##*##*####"
        L"####*##*##*########*##*##*####"
        L"##***##*******##*******##***##"
        L"##*#######*##*##*##*#######*##"
        L"##0#######*##*##*##*#######0##"
        L"##*********##****##*********##"
        L"##############################"
        L"##############################"
    },
    {
        L"##############################"
        L"##############################"
        L"##*********##****##*********##"
        L"##*#######*##*##*##*#######*##"
        L"##0#######*##*##*##*#######0##"
        L"##*##*********##*********##*##"
        L"##*##*##*####*##*####*##*##*##"
        L"##****##*####*##*####*##****##"
        L"#####*##*####*##*####*##*#####"
        L"#####*##**************##*#####"
        L"  ****#### ######## ####****  "
        L"##*##*#### ######## ####*##*##"
        L"##*##***              ***##*##"
        L"##*####*## ###--### ##*####*##"
        L"##*####*## ###HH### ##*####*##"
        L"##******## ##CBAD## ##******##"
        L"##*##*#### ######## ####*##*##"
        L"##*##*#### ######## ####*##*##"
        L"##*##***       F      ***##*##"
        L"##*####*##### ## #####*####*##"
        L"##*####*##### ## #####*####*##"
        L"##******##****##****##******##"
        L"####*##*##*########*##*##*####"
        L"####*##*##*########*##*##*####"
        L"##0**##       P        ##**0##"
        L"##*####*#####*##*#####*####*##"
        L"##*####*#####*##*#####*####*##"
        L"##******##****##****##******##"
        L"##*####*##*########*##*####*##"
        L"##*####*##*########*##*####*##"
        L"##******##**********##******##"
        L"##############################"
        L"##############################"
    },
};

static int32_t current_health;
static uint32_t current_coin_count_max;
static uint32_t current_coin_count;
static uint32_t current_score;
static uint32_t current_combo;
static uint32_t current_round;
static uint32_t current_fruit_count;
static Vector2UInt32 current_fruit_position;
static int32_t fruit_spawn_coroutine_index;
static uint32_t level_high_scores[LEVEL_COUNT_MAX];

static Coroutine coroutines[16];

static Pacman obj_pacman;
static Ghost obj_ghosts[GHOST_COUNT];
static GhostMode ghost_mode;
static bool obj_ghost_is_flashing[GHOST_COUNT];

errno_t pacman(void)
{
    pacman_commence();

main_menu:
    while (pacman_main_menu())
    {
level_start:
        wprintf(ANSI_CURSOR_DISABLE());
        game_map_initialize_level();

round_start:
        game_map_initialize_round();

round_reset:
        game_map_initialize_actors();
        game_draw_map();

        clock_t current = clock(), previous = current;
        uint32_t control;

        while (true)
        {
            if (game_pacman_check_coins())
                goto round_start;

            long double delta_time = (long double)(current - previous) / CLOCKS_PER_SEC;

            game_fruit_cycle();
            game_pacman_move(delta_time, &current_coin_count);
            game_ghosts_move(delta_time);
            game_draw_top_bar();

            if (game_pacman_check_collision())
                CONDITIONAL_GOTO(current_health >= 0, round_reset, level_start);

            if (control = game_control_flow())
                CONDITIONAL_GOTO(control == 1, level_start, main_menu);

            previous = current;
            current = clock();
            program_coroutine_trigger_all(current);
            _sleep(2);
        }
    }

    pacman_terminate();

    return 0;
}

int32_t console_scan_input(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char type;
    int32_t variable_count = 0, c = getchar();

    if (c == EOF)
        return 0;

    while (true)
    {
        while (c == ' ' || c == '\t')
            c = getchar();

        if (c == '\n')
            return variable_count;

        ++variable_count;

        if (!(type = *format++))
            break;

        switch (type)
        {
            uint32_t length;
            uint32_t ui;
            char chr;

            case 'u':
                if ((ui = c - '0') > 9u)
                {
                    wprintf(INVALID_NUMBER_FORMAT);
                    goto error;
                }

                for (length = 1; (c = getchar()) - '0' <= 9u; length += (ui != 0))
                    ui = ui * 10 + c - '0';

                if (c != ' ' && c != '\t' && c != '\n')
                {
                    wprintf(INVALID_NUMBER_FORMAT);
                    goto error;
                }

                if (length > 6)
                {
                    wprintf(ARGUMENT_TOO_LONG);
                    goto error;
                }

                *va_arg(args, uint32_t*) = ui;
                break;

            case 'c':
                chr = c;
                if (c != 26 && (c = getchar()) != ' ' && c != '\t' && c != '\n')
                {
                    wprintf(ARGUMENT_TOO_LONG);
                    goto error;
                }

                *va_arg(args, char*) = chr;
                break;

            case 'O':
                chr = c;
                if ((ui = c - '0') > 9u)
                {
                    if (c != 26 && (c = getchar()) != ' ' && c != '\t' && c != '\n')
                    {
                        wprintf(ARGUMENT_TOO_LONG);
                        goto error;
                    }

                    *va_arg(args, uint32_t*) = chr;
                    *va_arg(args, uint32_t*) = -1;
                    break;
                }

                for (length = 1; (c = getchar()) - '0' <= 9u; length += (ui != 0))
                    ui = ui * 10 + c - '0';

                if (c != ' ' && c != '\t' && c != '\n')
                {
                    wprintf(INVALID_NUMBER_FORMAT);
                    goto error;
                }

                if (length > 6)
                {
                    wprintf(ARGUMENT_TOO_LONG);
                    goto error;
                }

                *va_arg(args, uint32_t*) = -1;
                *va_arg(args, uint32_t*) = ui;
                break;

            case 'l':
                while (c != '\n' && c != 26)
                    c = getchar();
                break;

            default:
                wprintf(UNKNOWN_CONVERSION_SPECIFIER);
                goto error;
        }
    }

    if (c != '\n')
    {
        wprintf(TOO_MANY_ARGUMENTS);
error:
        while (c != '\n' && c != 26)
            c = getchar();

        va_end(args);
        return -variable_count;
    }

    va_end(args);
    return variable_count;
}

int32_t program_coroutine_register(long double duration, Routine routine)
{
    Coroutine coroutine = { .start_time = clock(), duration, routine };

    for (uint32_t i = 0; i < sizeof coroutines / sizeof * coroutines; ++i)
        if (coroutines[i].routine == NULL)
        {
            coroutines[i] = coroutine;
            return i;
        }

    return -1;
}

void program_coroutine_unregister(uint32_t index)
{
    coroutines[index].routine = NULL;
}

void program_coroutine_trigger_all(clock_t now)
{
    for (uint32_t i = 0; i < sizeof coroutines / sizeof * coroutines; ++i)
        if (coroutines[i].routine != NULL && (long double)(now - coroutines[i].start_time) / CLOCKS_PER_SEC >= coroutines[i].duration)
        {
            coroutines[i].routine();
            program_coroutine_unregister(i);
        }
}

void program_coroutine_unregister_all(void)
{
    for (uint32_t i = 0; i < sizeof coroutines / sizeof * coroutines; ++i)
        program_coroutine_unregister(i);
}

void pacman_commence(void)
{
    wprintf(ANSI_TEXT_WRAP_ENABLE() ANSI_CURSOR_DISABLE());

    for (uint32_t bar = 0; bar <= 23 + 21 + 25; ++bar, _sleep(25))
        wprintf(ANSI_CURSOR_POSITION_RESET() L"\n"
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ 🍊═════════════════════════════════🍓 ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                                   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                                   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║   " ANSI_SET_FG_MD(FG_PACMAN, ANSI_MD_SLOW_BLINK, L"   %.*s%.*s%.*s   "),   L"   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║   " ANSI_SET_FG_MD(FG_PACMAN, ANSI_MD_SLOW_BLINK, L"    %.*s%.*s%.*s    "), L"   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║   " ANSI_SET_FG_MD(FG_PACMAN, ANSI_MD_SLOW_BLINK, L"  %.*s%.*s%.*s  "),     L"   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                                   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                                   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ 🍎═════════════════════════════════🍒 ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" L"\n"),
            (bar >= 0) ? (bar > 23) ? 0 : (23 - bar) / 2 : 11, L"           ",
            (bar >= 0) ? (bar > 23) ? 23 : bar : 0, L"Welcome, fellow player!",
            (bar >= 0) ? (bar > 23) ? 0 : (24 - bar) / 2 : 12, L"            ",
            (bar >= 23) ? (bar > 23 + 21) ? 0 : (23 + 21 - bar) / 2 : 10, L"          ",
            (bar >= 23) ? (bar > 23 + 21) ? 21 : bar - 23 : 0, L"We hope you enjoy it.",
            (bar >= 23) ? (bar > 23 + 21) ? 0 : (24 + 21 - bar) / 2 : 11, L"           ",
            (bar >= 23 + 21) ? (bar > 23 + 21 + 25) ? 0 : (23 + 21 + 25 - bar) / 2 : 12, L"            ",
            (bar >= 23 + 21) ? (bar > 23 + 21 + 25) ? 25 : bar - 23 - 21 : 0, L"Press enter to proceed...",
            (bar >= 23 + 21) ? (bar > 23 + 21 + 25) ? 0 : (24 + 21 + 25 - bar) / 2 : 13, L"             ");

    wprintf(ANSI_CURSOR_ENABLE() L"\n"
        CONSOLE_ISSUE(L"If you notice strange bugs with texts, please make sure the size of your console is big enough.") L"\n"
        L"\n"
        CONSOLE_PROMPT(L""));

    console_scan_input("l");
}

void pacman_loading_screen(void)
{
    wprintf(ANSI_RESET_ALL() ANSI_CURSOR_POSITION_RESET() ANSI_CLEAR_FORWARD() ANSI_CURSOR_DISABLE());

    for (uint32_t bar = 0; bar <= 42 + 5 * 5; ++bar, _sleep(8))
        wprintf(ANSI_CURSOR_POSITION_RESET() L"\n"
            ANSI_SET_BG(ANSI_BG_BLACK,
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ╔══════════════════════════════════════════════╗ ┃" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║  ", ANSI_SET_FG(FG_PACMAN, L"%-42.*s"), L"  ║ ┃" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║  ", ANSI_SET_FG(FG_PACMAN, L"%-42.*s"), L"  ║ ┃" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║  ", ANSI_SET_FG(FG_PACMAN, L"%-42.*s"), L"  ║ ┃" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║  ", ANSI_SET_FG(FG_PACMAN, L"%-42.*s"), L"  ║ ┃" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║  ", ANSI_SET_FG(FG_PACMAN, L"%-42.*s"), L"  ║ ┃" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ╚══════════════════════════════════════════════╝ ┃" L"\n"),
                L"    ", ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" L"\n"))
            L"\n" 
            L"    " L"%.*s" L"%s    👻" ANSI_CLEAR_LINE_FORWARD(),
            bar >= 0 * 5 ? bar >= 42 + 0 * 5 ? 42 : bar - 0 * 5 : 0, L" ____                  __  __                ",
            bar >= 1 * 5 ? bar >= 42 + 1 * 5 ? 42 : bar - 1 * 5 : 0, L"|  _ \\ __ _  ___      |  \\/  | __ _ _ __   ",
            bar >= 2 * 5 ? bar >= 42 + 2 * 5 ? 42 : bar - 2 * 5 : 0, L"| |_) / _` |/ __| ___ | |\\/| |/ _` | '_ \\  ",
            bar >= 3 * 5 ? bar >= 42 + 3 * 5 ? 42 : bar - 3 * 5 : 0, L"|  __/ (_| | (__ |___|| |  | | (_| | | | |   ",
            bar >= 4 * 5 ? bar >= 42 + 4 * 5 ? 42 : bar - 4 * 5 : 0, L"|_|   \\__,_|\\___|     |_|  |_|\\__,_|_| |_|",
            bar > 67 ? 0 : 42 - ((bar * 42 / 67) / 2) * 2, L"🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸🔸",
            (bar & 3) < 2 ? L"😮" : L"😋");

    _sleep(200);
}

bool pacman_main_menu(void)
{
    pacman_loading_screen();

    wprintf(ANSI_CURSOR_ENABLE() ANSI_CURSOR_MOVE_BEGINNING_OF_PREVIOUS() ANSI_CLEAR_FORWARD() L"\n"
        CONSOLE_INFO(
            L"Menu options:" L"\n",
            L" " CONSOLE_OPTION(L"`S`") L" - Start the level." L"\n",
            L" " CONSOLE_OPTION(L"`L`") L" - Choose the level." L"\n",
            L" " CONSOLE_OPTION(L"`H`") L" - Show highest scores." L"\n",
            L" " CONSOLE_OPTION(L"`Q`") L" - Quit the game." L"\n"
            L"\n")
        CONSOLE_ISSUE(L"Enter one of the menu options from above.")
        ANSI_CURSOR_POSITION_SAVE());

prompt:
    wprintf(ANSI_CURSOR_POSITION_RESTORE() ANSI_CLEAR_FORWARD() L"\n" CONSOLE_PROMPT(L""));

    while (true)
    {
        char op;
        int32_t code = console_scan_input("c", &op);

        if (code <= 0)
        {
            if (code == 0)
                wprintf(TOO_FEW_ARGUMENTS);

            continue;
        }

        switch (op | 32)
        {
            case 's':
                return true;
            case 'l':
                pacman_scan_level();
                goto prompt;
            case 'h':
                pacman_show_highest_scores();
                goto prompt;
            case 'q':
                return false;
            default:
                wprintf(INVALID_OPTION);
        }
    }
}

void pacman_scan_level(void)
{
    wprintf(L"\n" 
        CONSOLE_INFO(
            L"Current level: " CONSOLE_NUMBER(L"%c") L"\n",
            L"Unlocked levels: " CONSOLE_NUMBER(L"%u") L"\n",
            L" " CONSOLE_OPTION(L"` `") L" - Cancel" L"\n",
            L" " CONSOLE_OPTION(L"`R`") L" - Let the program decide randomly" L"\n",
            L" " CONSOLE_OPTION(L"`n`") L" - Select one of the unlocked levels, within [" CONSOLE_NUMBER(WCS(LEVEL_COUNT_MIN)) L", " CONSOLE_NUMBER(L"%u") L"]" L"\n"
            L"\n")
        CONSOLE_ISSUE(L"Enter one of the options from above." L"\n")
        CONSOLE_PROMPT(L""),
        current_level < LEVEL_COUNT_MIN ? L'R' : (wchar_t)(L'0' + current_level), unlocked_levels, unlocked_levels);

    while (true)
    {
        uint32_t option, level;
        int32_t code = console_scan_input("O", &option, &level);

        if (code <= 0)
        {
            if (code == 0)
                return;

            continue;
        }

        if (level == -1)
            switch (option | 32)
            {
                case 'r':
                    current_level = 0;
                    return;
                default:
                    wprintf(INVALID_OPTION);
                    continue;
            }

        if (unlocked_levels < level && level <= LEVEL_COUNT_MAX)
        {
            wprintf(LEVEL_NOT_AVAILABLE, (level - 2) * 5000, level - 1);
            continue;
        }

        if (level < LEVEL_COUNT_MIN || LEVEL_COUNT_MAX < level)
        {
            wprintf(VALUE_OUT_OF_RANGE);
            continue;
        }

        current_level = level;
        return;
    }
}

void pacman_show_highest_scores(void)
{
    const wchar_t *const roman_numbers[LEVEL_COUNT_MAX] = { L"I:", L"II:", L"III:", L"IV:", L"V:" };

    wprintf(L"\n" CONSOLE_INFO(L"Highest scores for each level: " L"\n"));

    for (uint32_t i = 0; i < unlocked_levels; ++i)
        wprintf(CONSOLE_INFO(L" %-4s ") CONSOLE_NUMBER(L"%u") L"\n", roman_numbers[i], level_high_scores[i]);

    for (uint32_t i = unlocked_levels; i < LEVEL_COUNT_MAX; ++i)
        wprintf(CONSOLE_INFO(L" %-4s ") CONSOLE_OPTION(L"-") L"\n", roman_numbers[i]);

    wprintf(L"\n" CONSOLE_PROMPT(L""));

    console_scan_input("l");
}

void pacman_terminate(void)
{
    wprintf(ANSI_CURSOR_POSITION_RESET() ANSI_CLEAR_FORWARD() ANSI_CURSOR_DISABLE());

    for (uint32_t bar = 0; bar <= 19; ++bar, _sleep(25))
        wprintf(ANSI_CURSOR_POSITION_RESET() L"\n"
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ 🍊═══════════════════════════🍓 ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                             ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                             ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║   " ANSI_SET_FG_MD(FG_PACMAN, ANSI_MD_SLOW_BLINK, L"  %.*s%.*s%.*s  "),     L"   ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                             ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ ║                             ║ ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┃ 🍎═══════════════════════════🍒 ┃" L"\n")
            L"    " ANSI_SET_FG_BG(FG_BORDER_1, BG_GREY_1, L"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" L"\n"),
            (19 - bar) / 2, L"         ",
            bar, L"Thanks for playing!",
            (20 - bar) / 2, L"          ");
}

void game_map_initialize_level(void)
{
    current_health = 3;
    current_score = 0;
    current_round = 0;

    if (current_level < LEVEL_COUNT_MIN)
    {
        if (unlocked_levels == LEVEL_COUNT_MIN)
            current_level = -1;
        else
        {
            uint32_t random_level;

            do
            {
                random_level = ~RANDOM_INT(unlocked_levels);
            }
            while (random_level == current_level);

            current_level = random_level;
        }
    }
}

void game_map_initialize_round(void)
{
    uint32_t level = abs(current_level);
    const wchar_t *const map = level_maps[level - 1];
    current_coin_count_max = 0;
    current_coin_count = 0;
    current_fruit_count = 1;
    ++current_round;

    for (uint32_t i = 0; i < MAP_SIZE_X * MAP_SIZE_Y; ++i)
    {
        switch (map[i])
        {
            case L' ':
                tiles[i] = TILE_EMPTY;
                break;
            case L'H':
                tiles[i] = TILE_HOME;
                break;
            case L'#':
                tiles[i] = TILE_WALL;
                break;
            case L'-':
                tiles[i] = TILE_DOOR;
                break;
            case L'P':
                obj_pacman.home = (Vector2LongDouble) { i % MAP_SIZE_X + 0.5, i / MAP_SIZE_X + 0.5 };
                tiles[i] = TILE_EMPTY;
                break;
            case L'A':
                obj_ghosts[0].home = (Vector2LongDouble) { i % MAP_SIZE_X + 0.5, i / MAP_SIZE_X + 0.5 };
                tiles[i] = TILE_HOME;
                break;
            case L'B':
                obj_ghosts[1].home = (Vector2LongDouble) { i % MAP_SIZE_X + 0.5, i / MAP_SIZE_X + 0.5 };
                tiles[i] = TILE_HOME;
                break;
            case L'C':
                obj_ghosts[2].home = (Vector2LongDouble) { i % MAP_SIZE_X + 0.5, i / MAP_SIZE_X + 0.5 };
                tiles[i] = TILE_HOME;
                break;
            case L'D':
                obj_ghosts[3].home = (Vector2LongDouble) { i % MAP_SIZE_X + 0.5, i / MAP_SIZE_X + 0.5 };
                tiles[i] = TILE_HOME;
                break;
            case L'F':
                current_fruit_position = (Vector2UInt32) { i % MAP_SIZE_X, i / MAP_SIZE_X };
                tiles[i] = TILE_EMPTY;
                break;
            case L'*':
                tiles[i] = TILE_COIN;
                ++current_coin_count_max;
                break;
            case L'0':
                tiles[i] = TILE_POWERUP;
                ++current_coin_count_max;
                break;
            default:
                tiles[i] = TILE_ERROR;
                break;
        }
    }
}

void game_map_initialize_actors(void)
{
    obj_pacman.position = obj_pacman.home;
    obj_pacman.direction = (Vector2Int32) { 0, 0 };
    obj_pacman.speed = 5.0;

    game_fruit_despawn();

    for (uint32_t i = 0; i < GHOST_COUNT; ++i)
    {
        obj_ghosts[i].position = obj_ghosts[i].home;
        obj_ghosts[i].old_tile = (Vector2LongDouble) { -1.0, -1.0 };
        obj_ghosts[i].state = GHOST_STATE_NORMAL;
    }

    ghost_mode = GHOST_MODE_SCATTER;
    program_coroutine_register(15.0, &game_ghost_mode_alternate);
}

void game_save_progress(void)
{
    uint32_t level = abs(current_level);

    if (current_score > level_high_scores[level - 1])
        level_high_scores[level - 1] = current_score;

    if (unlocked_levels < LEVEL_COUNT_MAX && level == unlocked_levels && current_score >= (level - 2) * 5000)
        ++unlocked_levels;
}

void game_draw_symbol(uint32_t x, uint32_t y, const wchar_t *const symbol)
{
    wprintf(ANSI_CURSOR_POSITION_SET(%i, %i) L"%s", y + 4, x * 2 + 6, symbol);
}

void game_draw_tile(uint32_t x, uint32_t y)
{
    const wchar_t *symbol;

    switch (tiles[y * MAP_SIZE_X + x])
    {
        case TILE_EMPTY:
        case TILE_HOME:
            symbol = L"  ";
            break;

        case TILE_WALL:
            static const wchar_t *const wall_tiles[] =
            {
                L"  ", L"╹╹", L"╺━", L"┗┻",
                L"╻╻", L"┃┃", L"┏┳", L"┣╋",
                L"━╸", L"┻┛", L"━━", L"┻┻",
                L"┳┓", L"╋┫", L"┳┳", L"╋╋"
            };
    
            uint32_t tile_index = 0;
    
#pragma warning(push)
#pragma warning(disable: 6385)
            if (y - 1 < MAP_SIZE_Y && tiles[(y - 1) * MAP_SIZE_X + x] == TILE_WALL)
                tile_index |= 1;
            if (x + 1 < MAP_SIZE_X && tiles[y * MAP_SIZE_X + (x + 1)] == TILE_WALL)
                tile_index |= 2;
            if (y + 1 < MAP_SIZE_Y && tiles[(y + 1) * MAP_SIZE_X + x] == TILE_WALL)
                tile_index |= 4;
            if (x - 1 < MAP_SIZE_X && tiles[y * MAP_SIZE_X + (x - 1)] == TILE_WALL)
                tile_index |= 8;
#pragma warning(pop)

            symbol = wall_tiles[tile_index];
            break;

        case TILE_DOOR:
            static wchar_t door_tile[41];
            swprintf(door_tile, 40, ANSI_CREATE_ATTR(FG_DOOR) L"＿" ANSI_CREATE_ATTR(L"%s"), level_map_foreground_colors[abs(current_level) - 1]);
            symbol = door_tile;
            break;

        case TILE_COIN:
            symbol = L"🔸";
            break;
            
        case TILE_POWERUP:
            symbol = L"🪙";
            break;

        case TILE_FRUIT_CHERRY:
            symbol = L"🍒";
            break;

        case TILE_FRUIT_STRAWBERRY:
            symbol = L"🍓";
            break;

        case TILE_FRUIT_ORANGE:
            symbol = L"🍊";
            break;

        case TILE_FRUIT_APPLE:
            symbol = L"🍎";
            break;

        case TILE_FRUIT_MELON:
            symbol = L"🍈";
            break;
            
        case TILE_FRUIT_GALAXIAN:
            symbol = L"🏮";
            break;
            
        case TILE_FRUIT_BELL:
            symbol = L"🔔";
            break;
            
        case TILE_FRUIT_KEY:
            symbol = L"🔑";
            break;

        default:
            symbol = L"XX";
    }

    game_draw_symbol(x, y, symbol);
}

void game_draw_map(void)
{
    wprintf(ANSI_RESET_ALL() ANSI_CURSOR_POSITION_RESET() ANSI_CLEAR_FORWARD() ANSI_CURSOR_DISABLE() L"\n\n");

    for (uint32_t y = 0; y < MAP_SIZE_Y; ++y)
    {
        wprintf(L" " L"\n" ANSI_RESET_ALL() L"    " ANSI_CREATE_ATTRS(L"%s", L"%s") L" ",
            level_map_foreground_colors[abs(current_level) - 1], level_map_background_colors[abs(current_level) - 1]);

        for (uint32_t x = 0; x < MAP_SIZE_X; ++x)
            game_draw_tile(x, y);
    }

    wprintf(L" " L"\n");
}

void game_draw_top_bar(void)
{
    wprintf(ANSI_CURSOR_POSITION_RESET() ANSI_RESET_ALL() L"\n"
        L"    " L"Level: %-8u" L"Round: %-8u" L"Score: %-8u" L"           " L"%.*s" L"%.*s" ANSI_CREATE_ATTRS(L"%s", L"%s"),
        abs(current_level), current_round, current_score, current_health * 2, L"😋😋😋", (3 - current_health) * 2, L"💀💀💀",
        level_map_foreground_colors[abs(current_level) - 1], level_map_background_colors[abs(current_level) - 1]);
}

static inline long double get_difficulty_gradient(void)
{
    return current_round >= 20 ? 1.0 : current_round / 20.0;
}

void game_pacman_move(long double delta_time, uint32_t *current_coin_count)
{
    if ((int32_t)obj_pacman.position.x != 0)
    {
        Tile tile_left = tiles[(int32_t)obj_pacman.position.y * MAP_SIZE_X + (int32_t)(obj_pacman.position.x - 0.5)];

        if (tile_left == TILE_WALL || tile_left == TILE_DOOR)
            obj_pacman.position.x = (int32_t)(obj_pacman.position.x + 1) - 0.5;
    }

    if ((int32_t)obj_pacman.position.x != MAP_SIZE_X - 1)
    {
        Tile tile_right = tiles[(int32_t)obj_pacman.position.y * MAP_SIZE_X + (int32_t)(obj_pacman.position.x + 0.5)];

        if (tile_right == TILE_WALL || tile_right == TILE_DOOR)
            obj_pacman.position.x = (int32_t)(obj_pacman.position.x + 1) - 0.5;
    }

    if ((int32_t)obj_pacman.position.y != 0)
    {
        Tile up = tiles[(int32_t)(obj_pacman.position.y - 0.5) * MAP_SIZE_X + (int32_t)obj_pacman.position.x];

        if (up == TILE_WALL || up == TILE_DOOR)
            obj_pacman.position.y = (int32_t)(obj_pacman.position.y + 1) - 0.5;
    }

    if ((int32_t)obj_pacman.position.y != MAP_SIZE_Y - 1)
    {
        Tile down = tiles[(int32_t)(obj_pacman.position.y + 0.5) * MAP_SIZE_X + (int32_t)obj_pacman.position.x];

        if (down == TILE_WALL || down == TILE_DOOR)
            obj_pacman.position.y = (int32_t)(obj_pacman.position.y + 1) - 0.5;
    }

    Vector2Int32 direction = obj_pacman.direction;

    if (GetKeyState(VK_LEFT) < 0 || GetKeyState('A') < 0)
        direction = (Vector2Int32) { -1, 00 };
    else if (GetKeyState(VK_RIGHT) < 0 || GetKeyState('D') < 0)
        direction = (Vector2Int32) { +1, 00 };
    else if (GetKeyState(VK_UP) < 0 || GetKeyState('W') < 0)
        direction = (Vector2Int32) { 00, -1 };
    else if (GetKeyState(VK_DOWN) < 0 || GetKeyState('S') < 0)
        direction = (Vector2Int32) { 00, +1 };

    if (direction.x != 0 ?
        ((int32_t)obj_pacman.position.x != 0 && (int32_t)obj_pacman.position.x != MAP_SIZE_X - 1) :
        ((int32_t)obj_pacman.position.y != 0 && (int32_t)obj_pacman.position.y != MAP_SIZE_Y - 1))
    {
        Tile tile = tiles[((int32_t)obj_pacman.position.y + direction.y) * MAP_SIZE_X + ((int32_t)obj_pacman.position.x + direction.x)];

        if (tile == TILE_WALL || tile == TILE_DOOR)
            direction = obj_pacman.direction;
    }

    long double delta_distance = obj_pacman.speed * delta_time;

    Vector2LongDouble position = (Vector2LongDouble)
    {
        obj_pacman.position.x + direction.x * delta_distance,
        obj_pacman.position.y + direction.y * delta_distance
    };

    if (position.x < 0.0)
        position.x += MAP_SIZE_X;
    else if (position.x >= MAP_SIZE_X)
        position.x -= MAP_SIZE_X;

    if (position.y < 0.0)
        position.y += MAP_SIZE_Y;
    else if (position.y >= MAP_SIZE_Y)
        position.y -= MAP_SIZE_Y;

#pragma warning(push)
#pragma warning(disable: 6385)
#pragma warning(disable: 6386)
    switch (tiles[(int32_t)position.y * MAP_SIZE_X + (int32_t)position.x])
    {
        case TILE_WALL:
        case TILE_DOOR:
            goto draw_pacman;
        case TILE_COIN:
            current_score += 10;
            ++*current_coin_count;
            break;
        case TILE_POWERUP:
            current_score += 50;
            ++*current_coin_count;
            current_combo = 0;
            game_ghosts_frighten();
            break;
        case TILE_FRUIT_CHERRY:
            current_score += 100;
            break;
        case TILE_FRUIT_STRAWBERRY:
            current_score += 300;
            break;
        case TILE_FRUIT_ORANGE:
            current_score += 500;
            break;
        case TILE_FRUIT_APPLE:
            current_score += 700;
            break;
        case TILE_FRUIT_MELON:
            current_score += 1000;
            break;
        case TILE_FRUIT_GALAXIAN:
            current_score += 2000;
            break;
        case TILE_FRUIT_BELL:
            current_score += 3000;
            break;
        case TILE_FRUIT_KEY:
            current_score += 5000;
            break;
    }

    tiles[(int32_t)position.y * MAP_SIZE_X + (int32_t)position.x] = TILE_EMPTY;
    game_draw_tile((int32_t)obj_pacman.position.x, (int32_t)obj_pacman.position.y);
    obj_pacman.position = position;
    obj_pacman.direction = direction;
draw_pacman:
    game_draw_symbol((int32_t)obj_pacman.position.x, (int32_t)obj_pacman.position.y,
        (((uint32_t)(obj_pacman.position.x * 4) - 1) & 2) || (((uint32_t)(obj_pacman.position.y * 4) - 1) & 2) ? L"😮" : L"😋");
#pragma warning(pop)
}

bool game_pacman_check_coins(void)
{
    if (current_coin_count == current_coin_count_max)
    {
        game_close();
        _sleep(1000);
        return true;
    }

    return false;
}

bool game_pacman_check_collision(void)
{
    for (uint32_t i = 0; i < GHOST_COUNT; ++i)
        if ((int32_t)obj_pacman.position.x == (int32_t)obj_ghosts[i].position.x &&
            (int32_t)obj_pacman.position.y == (int32_t)obj_ghosts[i].position.y)
            switch (obj_ghosts[i].state)
            {
                case GHOST_STATE_NORMAL:
                    --current_health;
                    game_close();
                    _sleep(2000);
                    return true;

                case GHOST_STATE_FRIGHTENED:
                    current_score += 200 * (1 << current_combo);
                    ++current_combo;
                    obj_ghosts[i].state = GHOST_STATE_EATEN;
                    break;
            }

    return false;
}

static Vector2LongDouble game_get_target_a(void)
{
    return obj_pacman.position;
}

static Vector2LongDouble game_get_target_b(void)
{
    return (Vector2LongDouble) { obj_pacman.position.x + obj_pacman.direction.x * 4, obj_pacman.position.y + obj_pacman.direction.y * 4 };
}

static Vector2LongDouble game_get_target_c(void)
{
    Vector2LongDouble offset = { obj_pacman.position.x + obj_pacman.direction.x * 2, obj_pacman.position.y + obj_pacman.direction.y * 2 };

    return (Vector2LongDouble) { offset.x * 2 - obj_ghosts[0].position.x, offset.y * 2 - obj_ghosts[0].position.y };
}

static Vector2LongDouble game_get_target_d(void)
{
    long double x = obj_ghosts[3].position.x - obj_pacman.position.x;
    long double y = obj_ghosts[3].position.y - obj_pacman.position.y;

    return x * x + y * y <= 64.0 ? (Vector2LongDouble) { 28.0, 35.0 } : obj_pacman.position;
}

void game_ghosts_move(long double delta_time)
{
    static const wchar_t *const ghost_emojis[GHOST_COUNT] = { L"🔴", L"🟣", L"🔵", L"🟠" };

    static const Vector2LongDouble targets[GHOST_COUNT] =
    {
        { 26.0, -3.0 },
        {  4.0, -3.0 },
        {  0.0, 35.0 },
        { 28.0, 35.0 }
    };

    static Vector2LongDouble(*get_target[GHOST_COUNT])(void) =
    {
        &game_get_target_a,
        &game_get_target_b,
        &game_get_target_c,
        &game_get_target_d
    };

    for (uint32_t i = 0; i < GHOST_COUNT; ++i)
    {
        if ((int32_t)obj_ghosts[i].position.x != 0 &&
            tiles[(int32_t)obj_ghosts[i].position.y * MAP_SIZE_X + (int32_t)(obj_ghosts[i].position.x - 0.5)] == TILE_WALL)
            obj_ghosts[i].position.x = (int32_t)(obj_ghosts[i].position.x + 1) - 0.5;

        if ((int32_t)obj_ghosts[i].position.x != MAP_SIZE_X - 1 &&
            tiles[(int32_t)obj_ghosts[i].position.y * MAP_SIZE_X + (int32_t)(obj_ghosts[i].position.x + 0.5)] == TILE_WALL)
            obj_ghosts[i].position.x = (int32_t)(obj_ghosts[i].position.x + 1) - 0.5;

        if ((int32_t)obj_ghosts[i].position.y != 0 &&
            tiles[(int32_t)(obj_ghosts[i].position.y - 0.5) * MAP_SIZE_X + (int32_t)obj_ghosts[i].position.x] == TILE_WALL)
            obj_ghosts[i].position.y = (int32_t)(obj_ghosts[i].position.y + 1) - 0.5;

        if ((int32_t)obj_ghosts[i].position.y != MAP_SIZE_Y - 1 &&
            tiles[(int32_t)(obj_ghosts[i].position.y + 0.5) * MAP_SIZE_X + (int32_t)obj_ghosts[i].position.x] == TILE_WALL)
            obj_ghosts[i].position.y = (int32_t)(obj_ghosts[i].position.y + 1) - 0.5;

        Vector2LongDouble target;
        long double speed;

        if (obj_ghosts[i].state == GHOST_STATE_NORMAL)
        {
            target = ghost_mode == GHOST_MODE_SCATTER ? targets[i] : get_target[i]();
            speed = ghost_mode == GHOST_MODE_SCATTER ? 4.25 + get_difficulty_gradient() : 4.5 + 1.5 * get_difficulty_gradient();
        }
        else if (obj_ghosts[i].state == GHOST_STATE_FRIGHTENED)
        {
            uint32_t random_int = RANDOM_INT(4);
            int32_t sign = 1 - ((random_int & 1) << 1);
            Vector2Int32 random_direction = { sign * (random_int >> 1), sign * (1 - (random_int >> 1)) };
            target = (Vector2LongDouble) { obj_ghosts[i].position.x + random_direction.x, obj_ghosts[i].position.y + random_direction.y };
            speed = 3.25 + 1.75 * get_difficulty_gradient();
        }
        else
        {
            target = obj_ghosts[i].home;
            speed = 15.0;
        }

#pragma warning(push)
#pragma warning(disable: 6385)
        long double walked_distance_x = obj_ghosts[i].position.x - obj_ghosts[i].old_tile.x;
        long double walked_distance_y = obj_ghosts[i].position.y - obj_ghosts[i].old_tile.y;

        if (walked_distance_x * walked_distance_x + walked_distance_y * walked_distance_y >= 0.99)
        {
            Vector2Int32 direction = (Vector2Int32) { -obj_ghosts[i].direction.x, -obj_ghosts[i].direction.y };
            double long distance_to_target_min = INFINITY;

            Tile tile_midde = tiles[(int32_t)obj_ghosts[i].position.y * MAP_SIZE_X + (int32_t)obj_ghosts[i].position.x];
            Tile tile_left  = tiles[(int32_t)obj_ghosts[i].position.y * MAP_SIZE_X + (int32_t)(obj_ghosts[i].position.x - 1.0)];
            Tile tile_right = tiles[(int32_t)obj_ghosts[i].position.y * MAP_SIZE_X + (int32_t)(obj_ghosts[i].position.x + 1.0)];
            Tile tile_down  = tiles[(int32_t)(obj_ghosts[i].position.y + 1.0) * MAP_SIZE_X + (int32_t)obj_ghosts[i].position.x];
            Tile tile_up    = tiles[(int32_t)(obj_ghosts[i].position.y - 1.0) * MAP_SIZE_X + (int32_t)obj_ghosts[i].position.x];

            if (((int32_t)obj_ghosts[i].position.x == 0 ||
                 (tile_left != TILE_WALL && (obj_ghosts[i].state == GHOST_STATE_EATEN || tile_midde == TILE_HOME || tile_left != TILE_DOOR))) &&
                ((int32_t)obj_ghosts[i].old_tile.x + 1 != (int32_t)obj_ghosts[i].position.x ||
                 (int32_t)obj_ghosts[i].old_tile.y     != (int32_t)obj_ghosts[i].position.y))
            {
                double long x = obj_ghosts[i].position.x - 1.0 - target.x;
                double long y = obj_ghosts[i].position.y - target.y;
                double long distance_to_target = x * x + y * y;

                if (distance_to_target <= distance_to_target_min)
                {
                    distance_to_target_min = distance_to_target;
                    direction = (Vector2Int32) { -1, 00 };
                }
            }

            if (((int32_t)obj_ghosts[i].position.y == MAP_SIZE_Y - 1 ||
                 (tile_down != TILE_WALL && (obj_ghosts[i].state == GHOST_STATE_EATEN || tile_midde == TILE_HOME || tile_down != TILE_DOOR))) &&
                ((int32_t)obj_ghosts[i].old_tile.x     != (int32_t)obj_ghosts[i].position.x ||
                 (int32_t)obj_ghosts[i].old_tile.y - 1 != (int32_t)obj_ghosts[i].position.y))
            {
                double long x = obj_ghosts[i].position.x - target.x;
                double long y = obj_ghosts[i].position.y + 1.0 - target.y;
                double long distance_to_target = x * x + y * y;

                if (distance_to_target <= distance_to_target_min)
                {
                    distance_to_target_min = distance_to_target;
                    direction = (Vector2Int32) { 00, +1 };
                }
            }

            if (((int32_t)obj_ghosts[i].position.x == MAP_SIZE_X - 1 ||
                 (tile_right != TILE_WALL && (obj_ghosts[i].state == GHOST_STATE_EATEN || tile_midde == TILE_HOME || tile_right != TILE_DOOR))) &&
                ((int32_t)obj_ghosts[i].old_tile.x - 1 != (int32_t)obj_ghosts[i].position.x ||
                 (int32_t)obj_ghosts[i].old_tile.y     != (int32_t)obj_ghosts[i].position.y))
            {
                double long x = obj_ghosts[i].position.x + 1.0 - target.x;
                double long y = obj_ghosts[i].position.y - target.y;
                double long distance_to_target = x * x + y * y;

                if (distance_to_target <= distance_to_target_min)
                {
                    distance_to_target_min = distance_to_target;
                    direction = (Vector2Int32) { +1, 00 };
                }
            }

            if (((int32_t)obj_ghosts[i].position.y == 0 ||
                 (tile_up != TILE_WALL && (obj_ghosts[i].state == GHOST_STATE_EATEN || tile_midde == TILE_HOME || tile_up != TILE_DOOR))) &&
                ((int32_t)obj_ghosts[i].old_tile.x     != (int32_t)obj_ghosts[i].position.x ||
                 (int32_t)obj_ghosts[i].old_tile.y + 1 != (int32_t)obj_ghosts[i].position.y))
            {
                double long x = obj_ghosts[i].position.x - target.x;
                double long y = obj_ghosts[i].position.y - 1.0 - target.y;
                double long distance_to_target = x * x + y * y;

                if (distance_to_target <= distance_to_target_min)
                {
                    distance_to_target_min = distance_to_target;
                    direction = (Vector2Int32) { 00, -1 };
                }
            }

            obj_ghosts[i].direction = direction;
            obj_ghosts[i].old_tile = (Vector2LongDouble) { (int32_t)obj_ghosts[i].position.x + 0.5, (int32_t)obj_ghosts[i].position.y + 0.5 };
        }
#pragma warning(pop)

        long double delta_distance = speed * delta_time;

        Vector2LongDouble position = (Vector2LongDouble)
        {
            obj_ghosts[i].position.x + obj_ghosts[i].direction.x * delta_distance,
            obj_ghosts[i].position.y + obj_ghosts[i].direction.y * delta_distance
        };

        if (position.x < 0.0)
        {
            position.x += MAP_SIZE_X;
            obj_ghosts[i].old_tile.x = (int32_t)obj_ghosts[i].position.x + 0.5;
        }
        else if (position.x >= MAP_SIZE_X)
        {
            position.x -= MAP_SIZE_X;
            obj_ghosts[i].old_tile.x = (int32_t)obj_ghosts[i].position.x + 0.5;
        }

        if (position.y < 0.0)
        {
            position.y += MAP_SIZE_Y;
            obj_ghosts[i].old_tile.y = (int32_t)obj_ghosts[i].position.y + 0.5;
        }
        else if (position.y >= MAP_SIZE_Y)
        {
            position.y -= MAP_SIZE_Y;
            obj_ghosts[i].old_tile.y = (int32_t)obj_ghosts[i].position.y + 0.5;
        }

        if (obj_ghosts[i].state == GHOST_STATE_EATEN &&
            (int32_t)obj_ghosts[i].position.x == (int32_t)target.x &&
            (int32_t)obj_ghosts[i].position.y == (int32_t)target.y)
            obj_ghosts[i].state = GHOST_STATE_NORMAL;

        game_draw_tile((int32_t)obj_ghosts[i].position.x, (int32_t)obj_ghosts[i].position.y);
        obj_ghosts[i].position = position;
        game_draw_symbol((int32_t)obj_ghosts[i].position.x, (int32_t)obj_ghosts[i].position.y,
            obj_ghosts[i].state == GHOST_STATE_FRIGHTENED ?
                (obj_ghost_is_flashing[i] ? ((((uint32_t)(obj_ghosts[i].position.x * 4) - 1) & 2) || (((uint32_t)(obj_ghosts[i].position.y * 4) - 1) & 2) ? L"🌑" : L"🧿") : L"🧿") :
            obj_ghosts[i].state == GHOST_STATE_EATEN ? L"👀" :
            ghost_emojis[i]);
    }
}

void game_ghost_reverse_direction(uint32_t index)
{
    obj_ghosts[index].old_tile = (Vector2LongDouble) { (int32_t)obj_ghosts[index].position.x + 0.5 + obj_ghosts[index].direction.x, (int32_t)obj_ghosts[index].position.y + 0.5 + obj_ghosts[index].direction.y };
    obj_ghosts[index].direction = (Vector2Int32) { -obj_ghosts[index].direction.x, -obj_ghosts[index].direction.y };
}

void game_ghost_mode_alternate(void)
{
    for (uint32_t i = 0; i < GHOST_COUNT; ++i)
        if (obj_ghosts[i].state == GHOST_STATE_NORMAL)
            game_ghost_reverse_direction(i);

    if (ghost_mode == GHOST_MODE_SCATTER)
    {
        ghost_mode = GHOST_MODE_CHASE;
        program_coroutine_register(15.0 - 5.0 * get_difficulty_gradient(), &game_ghost_mode_alternate);
    }
    else
    {
        ghost_mode = GHOST_MODE_SCATTER;
        program_coroutine_register(15.0 + 5.0 * get_difficulty_gradient(), &game_ghost_mode_alternate);
    }
}

static void game_ghost_frighten_a(void)
{
    if (obj_ghosts[0].state == GHOST_STATE_FRIGHTENED)
        obj_ghosts[0].state = GHOST_STATE_NORMAL;
}

static void game_ghost_frighten_b(void)
{
    if (obj_ghosts[1].state == GHOST_STATE_FRIGHTENED)
        obj_ghosts[1].state = GHOST_STATE_NORMAL;
}

static void game_ghost_frighten_c(void)
{
    if (obj_ghosts[2].state == GHOST_STATE_FRIGHTENED)
        obj_ghosts[2].state = GHOST_STATE_NORMAL;
}

static void game_ghost_frighten_d(void)
{
    if (obj_ghosts[3].state == GHOST_STATE_FRIGHTENED)
        obj_ghosts[3].state = GHOST_STATE_NORMAL;
}

static void game_ghost_set_flashing_a(void)
{
    obj_ghost_is_flashing[0] = true;
}

static void game_ghost_set_flashing_b(void)
{
    obj_ghost_is_flashing[1] = true;
}

static void game_ghost_set_flashing_c(void)
{
    obj_ghost_is_flashing[2] = true;
}

static void game_ghost_set_flashing_d(void)
{
    obj_ghost_is_flashing[3] = true;
}

void game_ghosts_frighten(void)
{
    static void (*frighten[GHOST_COUNT])(void) =
    {
        &game_ghost_frighten_a,
        &game_ghost_frighten_b,
        &game_ghost_frighten_c,
        &game_ghost_frighten_d
    };

    static void (*set_flash[GHOST_COUNT])(void) =
    {
        &game_ghost_set_flashing_a,
        &game_ghost_set_flashing_b,
        &game_ghost_set_flashing_c,
        &game_ghost_set_flashing_d
    };

    static int32_t frighten_coroutine_indexes[GHOST_COUNT];
    static int32_t is_flashing_coroutine_indexes[GHOST_COUNT];

    for (uint32_t i = 0; i < GHOST_COUNT; ++i)
        if (obj_ghosts[i].state != GHOST_STATE_EATEN)
        {
            if (obj_ghosts[i].state == GHOST_STATE_FRIGHTENED)
            {
                program_coroutine_unregister(frighten_coroutine_indexes[i]);
                program_coroutine_unregister(is_flashing_coroutine_indexes[i]);
            }
            else
                game_ghost_reverse_direction(i);

            obj_ghosts[i].state = GHOST_STATE_FRIGHTENED;
            obj_ghost_is_flashing[i] = false;
            frighten_coroutine_indexes[i] = program_coroutine_register(12.0 - 6.0 * get_difficulty_gradient(), frighten[i]);
            is_flashing_coroutine_indexes[i] = program_coroutine_register(9.0 - 4.5 * get_difficulty_gradient(), set_flash[i]);
        }
}

void game_fruit_cycle(void)
{
    long double g = get_difficulty_gradient();
    uint32_t c = current_fruit_count;

    if (current_coin_count >= 200 * c / ((uint32_t)(2 * g) + 2))
    {
        ++current_fruit_count;
        game_fruit_spawn();
    }
}

void game_fruit_spawn(void)
{
    if (fruit_spawn_coroutine_index == -1)
    {
        tiles[current_fruit_position.y * MAP_SIZE_X + current_fruit_position.x] =
            current_round > 12 ? TILE_FRUIT_KEY :
            current_round > 10 ? TILE_FRUIT_BELL :
            current_round >  8 ? TILE_FRUIT_GALAXIAN :
            current_round >  6 ? TILE_FRUIT_MELON :
            current_round >  4 ? TILE_FRUIT_APPLE :
            current_round >  2 ? TILE_FRUIT_ORANGE :
            current_round >  1 ? TILE_FRUIT_STRAWBERRY :
                                 TILE_FRUIT_CHERRY;

        game_draw_tile(current_fruit_position.x, current_fruit_position.y);
        fruit_spawn_coroutine_index = program_coroutine_register(15.0 - 8.0 * get_difficulty_gradient(), &game_fruit_despawn);
    }
}

void game_fruit_despawn(void)
{
    if ((uint32_t)(tiles[current_fruit_position.y * MAP_SIZE_X + current_fruit_position.x] - TILE_FRUIT_CHERRY) < TILE_FRUIT_KEY)
    {
        tiles[current_fruit_position.y * MAP_SIZE_X + current_fruit_position.x] = TILE_EMPTY;
        game_draw_tile(current_fruit_position.x, current_fruit_position.y);
    }

    fruit_spawn_coroutine_index = -1;
}

uint32_t game_control_flow(void)
{
    if (GetKeyState(VK_SHIFT) < 0)
    {
        if (GetKeyState('R') < 0)
        {
            game_close();
            _sleep(500);
            return 1;
        }

        if (GetKeyState(VK_ESCAPE) < 0)
        {
            game_close();
            _sleep(500);
            return 2;
        }
    }

    return 0;
}

void game_close(void)
{
    game_save_progress();
    program_coroutine_unregister_all();
}

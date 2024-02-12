#ifndef __ANSI_H__
#define __ANSI_H__

#include "macro_utils.h"

#define ESC     L"\033"
#define CSI ESC L"["

#define ANSI_ATTR_RESET       L"0"

#define ANSI_MD_BOLD          L"1"
#define ANSI_MD_FAINT         L"2"
#define ANSI_MD_ITALIC        L"3"
#define ANSI_MD_UNDERLINE     L"4"
#define ANSI_MD_SLOW_BLINK    L"5"
#define ANSI_MD_REVERSE_VIDEO L"7"
#define ANSI_MD_CONCEAL       L"8"
#define ANSI_MD_STRIKE        L"9"

#define ANSI_FG_BLACK        L"30"
#define ANSI_FG_RED          L"31"
#define ANSI_FG_GREEN        L"32"
#define ANSI_FG_YELLOW       L"33"
#define ANSI_FG_BLUE         L"34"
#define ANSI_FG_MAGENTA      L"35"
#define ANSI_FG_CYAN         L"36"
#define ANSI_FG_WHITE        L"37"
#define ANSI_FG_INDEX(n)     L"38;5;" WCS(n)
#define ANSI_FG_RGB(r, g, b) L"38;2;" WCS(r) L";" WCS(g) L";" WCS(b)
#define ANSI_FG_DEFAULT      L"39"

#define ANSI_BG_BLACK        L"40"
#define ANSI_BG_RED          L"41"
#define ANSI_BG_GREEN        L"42"
#define ANSI_BG_YELLOW       L"43"
#define ANSI_BG_BLUE         L"44"
#define ANSI_BG_MAGENTA      L"45"
#define ANSI_BG_CYAN         L"46"
#define ANSI_BG_WHITE        L"47"
#define ANSI_BG_INDEX(n)     L"48;5;" WCS(n)
#define ANSI_BG_RGB(r, g, b) L"48;2;" WCS(r) L";" WCS(g) L";" WCS(b)
#define ANSI_BG_DEFAULT      L"49"

#define ANSI_TEXT_WRAP_ENABLE()  CSI L"=7h"
#define ANSI_TEXT_WRAP_DISABLE() CSI L"=7l"

#define ANSI_CURSOR_ENABLE()  CSI L"?25h"
#define ANSI_CURSOR_DISABLE() CSI L"?25l"

#define ANSI_CURSOR_POSITION_RESET()   CSI                    L"H"
#define ANSI_CURSOR_POSITION_SET(n, m) CSI WCS(n) L";" WCS(n) L"H"
#define ANSI_CURSOR_POSITION_SAVE()    ESC L"7"
#define ANSI_CURSOR_POSITION_RESTORE() ESC L"8"

#define ANSI_CURSOR_MOVE_UP()        CSI        L"A"
#define ANSI_CURSOR_MOVE_UP_BY(n)    CSI WCS(n) L"A"
#define ANSI_CURSOR_MOVE_DOWN()      CSI        L"B"
#define ANSI_CURSOR_MOVE_DOWN_BY(n)  CSI WCS(n) L"B"
#define ANSI_CURSOR_MOVE_RIGHT()     CSI        L"C"
#define ANSI_CURSOR_MOVE_RIGHT_BY(n) CSI WCS(n) L"C"
#define ANSI_CURSOR_MOVE_LEFT()      CSI        L"D"
#define ANSI_CURSOR_MOVE_LEFT_BY(n)  CSI WCS(n) L"D"

#define ANSI_CURSOR_MOVE_BEGINNING_OF_NEXT()         CSI        L"E"
#define ANSI_CURSOR_MOVE_BEGINNING_OF_NEXT_BY(n)     CSI WCS(n) L"E"
#define ANSI_CURSOR_MOVE_BEGINNING_OF_PREVIOUS()     CSI        L"F"
#define ANSI_CURSOR_MOVE_BEGINNING_OF_PREVIOUS_BY(n) CSI WCS(n) L"F"

#define ANSI_CLEAR_FORWARD()  CSI L"J"
#define ANSI_CLEAR_BACKWARD() CSI L"1J"
#define ANSI_CLEAR_ALL()      CSI L"2J"

#define ANSI_CLEAR_LINE_FORWARD()  CSI L"K"
#define ANSI_CLEAR_LINE_BACKWARD() CSI L"1K"
#define ANSI_CLEAR_LINE()          CSI L"2K"

#define ANSI_REVERSE_VIDEO_ENABLE()  CSI L"?5h"
#define ANSI_REVERSE_VIDEO_DISABLE() CSI L"?5l"

#define __ANSI_ATTR_DISABLE_MARKDOWN(markdown) L"2" markdown

#define ANSI_CREATE_ATTR(attr) CSI attr L"m"
#define ANSI_CREATE_ATTRS(attr, ...) CSI JOIN(L";", attr, ## __VA_ARGS__) L"m"

#define ANSI_SET_MD(markdown, ...)                                ANSI_CREATE_ATTR(markdown)                          JOIN(ANSI_CREATE_ATTR(markdown), __VA_ARGS__)                          ANSI_CREATE_ATTR(__ANSI_ATTR_DISABLE_MARKDOWN(markdown))
#define ANSI_SET_BG(background, ...)                              ANSI_CREATE_ATTR(background)                        JOIN(ANSI_CREATE_ATTR(background), __VA_ARGS__)                        ANSI_CREATE_ATTR(ANSI_BG_DEFAULT)
#define ANSI_SET_FG(foreground, ...)                              ANSI_CREATE_ATTR(foreground)                        JOIN(ANSI_CREATE_ATTR(foreground), __VA_ARGS__)                        ANSI_CREATE_ATTR(ANSI_FG_DEFAULT)
#define ANSI_SET_FG_BG(foreground, background, ...)               ANSI_CREATE_ATTRS(foreground, background)           JOIN(ANSI_CREATE_ATTRS(foreground, background), __VA_ARGS__)           ANSI_CREATE_ATTRS(ANSI_FG_DEFAULT, ANSI_BG_DEFAULT)
#define ANSI_SET_BG_MD(background, markdown, ...)                 ANSI_CREATE_ATTRS(background, markdown)             JOIN(ANSI_CREATE_ATTRS(background, markdown), __VA_ARGS__)             ANSI_CREATE_ATTRS(ANSI_BG_DEFAULT, __ANSI_ATTR_DISABLE_MARKDOWN(markdown))
#define ANSI_SET_FG_MD(foreground, markdown, ...)                 ANSI_CREATE_ATTRS(foreground, markdown)             JOIN(ANSI_CREATE_ATTRS(foreground, markdown), __VA_ARGS__)             ANSI_CREATE_ATTRS(ANSI_FG_DEFAULT, __ANSI_ATTR_DISABLE_MARKDOWN(markdown))
#define ANSI_SET_FG_BG_MD(foreground, background, markdown, ...)  ANSI_CREATE_ATTRS(foreground, background, markdown) JOIN(ANSI_CREATE_ATTRS(foreground, background, markdown), __VA_ARGS__) ANSI_CREATE_ATTRS(ANSI_FG_DEFAULT, ANSI_BG_DEFAULT, __ANSI_ATTR_DISABLE_MARKDOWN(markdown))

#define ANSI_RESET_CL()  ANSI_CREATE_ATTRS(ANSI_FG_DEFAULT, ANSI_BG_DEFAULT)
#define ANSI_RESET_MD(markdown)  ANSI_CREATE_ATTR(__ANSI_ATTR_DISABLE_MARKDOWN(markdown))
#define ANSI_RESET_ALL() ANSI_CREATE_ATTR(ANSI_ATTR_RESET)

#endif

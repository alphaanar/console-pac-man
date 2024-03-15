#ifndef __COMMONDEFS_H__
#define __COMMONDEFS_H__

#include <stdint.h>
#include <stdlib.h>

#define RANDOM_INT(value) (rand() % (value))

#define CONDITIONAL_GOTO(condition, layer_true, layer_false) do { if (condition) goto layer_true; goto layer_false; } while (false)

typedef struct _Vector2UInt32
{
    uint32_t x;
    uint32_t y;
} Vector2UInt32;

typedef struct _Vector2Int32
{
    int32_t x;
    int32_t y;
} Vector2Int32;

typedef struct _Vector2LongDouble
{
    long double x;
    long double y;
} Vector2LongDouble;

#endif

#include "pacman.h"
#include <fcntl.h>
#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

int main()
{
#if !defined(_WIN32) && !defined(_WIN64)
    /*
        Just as any other developer might do in this scenario, we explicitly mark the game
        as Windows-exclusive because we prefer not to invest effort in making it portable.

         - Regards.
    */

    return -1;
#endif

    if (!SetConsoleTitle(L"PacMan"))
        return 1;

    uint64_t seed = (uint64_t)time(NULL);
    srand((uint32_t)(seed ^ (seed >> 32)));

    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1)
        return 2;

    errno_t err = pacman();

    return err;
}

#include <iostream>

#include <gp/gp.h>
#include "game.h"
#include "defs.h"

int main(int argc, char* argv[])
{
    GPLib* gp = gpInit(WINDOW_WIDTH, WINDOW_HEIGHT, "Tower++", GP_VSYNC_ON);

    while (gpWindowShouldClose(gp) == false)
    {
        Game g(gp);
        g.gameRun();
    }
    gpShutdown(gp);
    return 0;
}
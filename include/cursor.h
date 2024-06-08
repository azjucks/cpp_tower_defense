#pragma once

#include <gp/gp.h>

#include "defs.h"
#include "object.h"

class Cursor : public Object
{
public:
    GPColor color;
    Cursor() { color = GP_CWHITE; }
    void updateCursor(GPLib* gp);
    void drawCursor(GPLib* gp, GPTexture texture, GPRect src);
};
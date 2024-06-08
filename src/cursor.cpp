#include "cursor.h"

void Cursor::updateCursor(GPLib* gp)
{
    GPVector2 mouse = gpMousePosition(gp);
    m_pos = {mouse.x - RENDER_SIZE/2, mouse.y - RENDER_SIZE/2};
}

void Cursor::drawCursor(GPLib* gp, GPTexture texture, GPRect src)
{
    GPRect dest = {m_pos.x, m_pos.y, RENDER_SIZE, RENDER_SIZE};

    gpDrawTextureEx2(gp, texture, src, dest, color);
}
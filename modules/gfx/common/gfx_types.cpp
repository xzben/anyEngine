#include "gfx_types.h"

#include "base/Texture.h"

BEGIN_GFX_NAMESPACE
void DrawSurface::addRef() {
    if (texture) {
        texture->addRef();
    }
}
void DrawSurface::delRef() {
    if (texture) {
        texture->delRef();
    }
}
END_GFX_NAMESPACE
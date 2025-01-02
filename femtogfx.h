#ifndef FEMTOGFX_H
#define FEMTOGFX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    size_t width;
    size_t height;
    size_t *pixels;
} FemtogfxCanvas;

typedef struct {
    size_t x, y;
} FemtogfxVec2;

typedef struct {
    size_t x, y;
    size_t w, h;
} FemtogfxRectangle;

FemtogfxCanvas femtogfx_create(size_t width, size_t height);
void femtogfx_destroy(FemtogfxCanvas canvas);
void femtogfx_fill(FemtogfxCanvas canvas, bool color);
void femtogfx_invert(FemtogfxCanvas canvas);
void femtogfx_set_pixel(FemtogfxCanvas canvas, bool color, size_t x, size_t y);
bool femtogfx_get_pixel(FemtogfxCanvas canvas, size_t x, size_t y);
void femtogfx_draw_line(FemtogfxCanvas canvas, bool color, FemtogfxVec2 p0, FemtogfxVec2 p1);
void femtogfx_draw_horizontal_line(FemtogfxCanvas canvas, bool color, FemtogfxVec2 point, size_t width);
void femtogfx_draw_rectangle_edges(FemtogfxCanvas canvas, bool color, FemtogfxRectangle rectangle);
void femtogfx_draw_rectangle(FemtogfxCanvas canvas, bool color, FemtogfxRectangle rectangle);
void femtogfx_draw_char(FemtogfxCanvas canvas, bool color, char c, FemtogfxVec2 rectangle);
void femtogfx_draw_string(FemtogfxCanvas canvas, bool color, char *string, FemtogfxVec2 rectangle);

#endif /* FEMTOGFX_H */

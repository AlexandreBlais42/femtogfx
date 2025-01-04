#ifndef FEMTOGFX_H
#define FEMTOGFX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define FEMTOGFX_WORD_SIZE (sizeof(uintptr_t) * 8)
#define FEMTOGFX_CANVAS_SIZE(width, height) ((width * height + FEMTOGFX_WORD_SIZE - 1) / FEMTOGFX_WORD_SIZE)

typedef struct {
    uintptr_t width;
    uintptr_t height;
    uintptr_t *pixels;
} FemtogfxCanvas;

typedef struct {
    uintptr_t x, y;
} FemtogfxVec2;

typedef struct {
    uintptr_t x, y;
    uintptr_t w, h;
} FemtogfxRectangle;

FemtogfxCanvas femtogfx_create(uintptr_t width, size_t height);
void femtogfx_destroy(FemtogfxCanvas canvas);
void femtogfx_fill(FemtogfxCanvas canvas, bool color);
void femtogfx_invert(FemtogfxCanvas canvas);
void femtogfx_set_pixel(FemtogfxCanvas canvas, bool color, uintptr_t x, size_t y);
bool femtogfx_get_pixel(FemtogfxCanvas canvas, uintptr_t x, size_t y);
void femtogfx_draw_line(FemtogfxCanvas canvas, bool color, FemtogfxVec2 p0, FemtogfxVec2 p1);
void femtogfx_draw_horizontal_line(FemtogfxCanvas canvas, bool color, FemtogfxVec2 point, uintptr_t width);
void femtogfx_draw_rectangle_edges(FemtogfxCanvas canvas, bool color, FemtogfxRectangle rectangle);
void femtogfx_draw_rectangle(FemtogfxCanvas canvas, bool color, FemtogfxRectangle rectangle);
void femtogfx_draw_char(FemtogfxCanvas canvas, bool color, char c, FemtogfxVec2 rectangle);
void femtogfx_draw_string(FemtogfxCanvas canvas, bool color, char *string, FemtogfxVec2 rectangle);

#endif /* FEMTOGFX_H */

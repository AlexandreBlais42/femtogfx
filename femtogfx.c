#include "femtogfx.h"

#include <stdlib.h>

#define bitsize (sizeof(size_t) * 8)
#define canvas_size(canvas) ((canvas.width * canvas.height + bitsize - 1) / bitsize)
#define get_byte_index(canvas, x, y) ((canvas.width * y + x) / bitsize)
#define get_bit_index(canvas, x, y) ((canvas.width * y + x) % bitsize)
#define get_bit_mask(shift) ((size_t) 1 << shift);
#define all_bits_set (~((size_t) 0))

static void femtogfx_memset(void *ptr, uint8_t value, size_t length) {
    uint8_t *p = ptr;
    while (length--) {
        *p = value;
        p++;
    }
}

FemtogfxCanvas femtogfx_create(size_t width, size_t height) {
    return (FemtogfxCanvas) {
        .width = width,
        .height = height,
        .pixels = (size_t *) calloc((width * height + bitsize - 1) / bitsize, sizeof(size_t)),
    };
}

void femtogfx_destroy(FemtogfxCanvas canvas) {
    free(canvas.pixels);
}

inline void femtogfx_fill(FemtogfxCanvas canvas, bool color) {
    femtogfx_memset(canvas.pixels, color ? 0xFF : 0x00, canvas_size(canvas) * sizeof(size_t));
}

inline void femtogfx_invert(FemtogfxCanvas canvas) {
    for (size_t i = 0 ; i < canvas_size(canvas) ; i++) {
        canvas.pixels[i] = ~canvas.pixels[i];
    }
}

inline void femtogfx_set_pixel(FemtogfxCanvas canvas, bool color, size_t x, size_t y) {
    const size_t offset = get_bit_index(canvas, x, y);
    canvas.pixels[get_byte_index(canvas, x, y)] &= ~get_bit_mask(offset);
    canvas.pixels[get_byte_index(canvas, x, y)] |= (size_t) color << offset;
}

inline bool femtogfx_get_pixel(FemtogfxCanvas canvas, size_t x, size_t y) {
    return canvas.pixels[get_byte_index(canvas, x, y)] & get_bit_mask(get_bit_index(canvas, x, y));
}

inline void femtogfx_draw_line(FemtogfxCanvas canvas, bool color, FemtogfxVec2 p0, FemtogfxVec2 p1) {
    ssize_t dx = p1.x - p0.x;
    dx *= dx < 0 ? -1 : 1;
    ssize_t sx = p0.x < p1.x ? 1 : -1;
    ssize_t dy = p1.y - p0.y;
    dy *= dy < 0 ? 1 : -1;
    ssize_t sy = p0.y < p1.y ? 1 : -1;
    ssize_t error = dx + dy;

    while (true) {
        femtogfx_set_pixel(canvas, color, p0.x, p0.y);
        if (p0.x == p1.x && p0.y == p1.y) {
            break;
        }
        if (error * 2 >= dy) {
            error += dy;
            p0.x += sx;
        }
        if (error * 2 <= dx) {
            error += dx;
            p0.y += sy;
        }
    }
}

inline void femtogfx_draw_horizontal_line(FemtogfxCanvas canvas, bool color, FemtogfxVec2 point, size_t width) {
    size_t start_byte_index = get_byte_index(canvas, point.x, point.y);
    size_t start_byte_mask = all_bits_set << get_bit_index(canvas, point.x, point.y);

    size_t end_byte_index = get_byte_index(canvas, point.x + width, point.y);
    size_t end_byte_mask = all_bits_set >> get_bit_index(canvas, point.x + width, point.y);

    if (start_byte_index == end_byte_index) {
        if (color) {
            canvas.pixels[start_byte_index] |= start_byte_mask & end_byte_mask;
        } else {
            canvas.pixels[start_byte_index] &= ~(start_byte_mask & end_byte_mask);
        }
        return;
    }

    if (color) {
        canvas.pixels[start_byte_index] |= start_byte_mask;
        canvas.pixels[end_byte_index] |= end_byte_mask;
        for (size_t i = start_byte_index + 1 ; i < end_byte_index ; i++) {
            canvas.pixels[i] = all_bits_set;
        }
    } else {
        canvas.pixels[start_byte_index] &= ~start_byte_mask;
        canvas.pixels[end_byte_index] &= ~end_byte_mask;
        for (size_t i = start_byte_index + 1 ; i < end_byte_index ; i++) {
            canvas.pixels[i] = 0;
        }
    }
}

inline void femtogfx_draw_rectangle_edges(FemtogfxCanvas canvas, bool color, FemtogfxRectangle rectangle) {
    for (size_t y = rectangle.y ; y < rectangle.y + rectangle.h ; y++) {
        femtogfx_set_pixel(canvas, color, rectangle.x, y);
        femtogfx_set_pixel(canvas, color, rectangle.x + rectangle.w, y);
    }
    femtogfx_draw_horizontal_line(canvas, color, (FemtogfxVec2) {.x = rectangle.x, .y = rectangle.y}, rectangle.w);
    femtogfx_draw_horizontal_line(canvas, color, (FemtogfxVec2) {.x = rectangle.x, .y = rectangle.y + rectangle.h}, rectangle.w);
}

inline void femtogfx_draw_rectangle(FemtogfxCanvas canvas, bool color, FemtogfxRectangle rectangle) {
    for (size_t y = rectangle.y ; y < rectangle.y + rectangle.h ; y++) {
        femtogfx_draw_horizontal_line(canvas, color, (FemtogfxVec2) {.x = rectangle.x, .y = y}, rectangle.w);
    }
}

#if defined(sevenlinedigital_font)
#include "fonts/sevenlinedigital_font.h"
#elif defined(acme_5_outlines_font)
#include "fonts/acme_5_outlines_font.h"
#elif defined(aztech_font)
#include "fonts/aztech_font.h"
#elif defined(Blokus_font)
#include "fonts/Blokus_font.h"
#elif defined(BMplain_font)
#include "fonts/BMplain_font.h"
#elif defined(BMSPA_font)
#include "fonts/BMSPA_font.h"
#elif defined(bubblesstandard_font)
#include "fonts/bubblesstandard_font.h"
#elif defined(Commo)
#include "fonts/Commo-Monospaced_font.h"
#elif defined(crackers_font)
#include "fonts/crackers_font.h"
#elif defined(font)
#include "fonts/font.h"
#elif defined(formplex12_font)
#include "fonts/formplex12_font.h"
#elif defined(haiku_font)
#include "fonts/haiku_font.h"
#elif defined(HISKYF21_font)
#include "fonts/HISKYF21_font.h"
#elif defined(homespun_font)
#include "fonts/homespun_font.h"
#elif defined(HUNTER_font)
#include "fonts/HUNTER_font.h"
#elif defined(m38_font)
#include "fonts/m38_font.h"
#elif defined(Minimum)
#include "fonts/Minimum+1_font.h"
#elif defined(Minimum_font)
#include "fonts/Minimum_font.h"
#elif defined(pzim3x5_font)
#include "fonts/pzim3x5_font.h"
#elif defined(Raumsond_font)
#include "fonts/Raumsond_font.h"
#elif defined(renew_font)
#include "fonts/renew_font.h"
#elif defined(sloth_font)
#include "fonts/sloth_font.h"
#elif defined(SUPERDIG_font)
#include "fonts/SUPERDIG_font.h"
#elif defined(tama_mini02_font)
#include "fonts/tama_mini02_font.h"
#elif defined(zxpix_font)
#include "fonts/zxpix_font.h"
#else
#include "fonts/tama_mini02_font.h"
#endif

inline void femtogfx_draw_char(FemtogfxCanvas canvas, bool color, char c, FemtogfxVec2 rectangle) {
    for (uint8_t dx = 0 ; dx < FEMTOGFX_FONT_WIDTH ; dx++) {
        for (uint8_t dy = 0 ; dy < FEMTOGFX_FONT_HEIGHT ; dy++) {
            if (font[(uint8_t) c][dx] & (1 << dy)) {
                femtogfx_set_pixel(canvas, color, rectangle.x + dx, rectangle.y + dy);
            }
        }
    }
}

inline void femtogfx_draw_string(FemtogfxCanvas canvas, bool color, char *string, FemtogfxVec2 rectangle) {
    while (true) {
        switch (*string) {
            case '\0':
                return;

            case '\r':
            case '\n':
                rectangle.x = 0;
                rectangle.y += FEMTOGFX_FONT_HEIGHT;
                break;

            default:
                if (rectangle.x + FEMTOGFX_FONT_WIDTH > canvas.width) {
                    rectangle.x = 0;
                    rectangle.y += FEMTOGFX_FONT_HEIGHT;
                }
                femtogfx_draw_char(canvas, color, *string - ' ', rectangle);
                rectangle.x += FEMTOGFX_FONT_WIDTH;
                break;
        }
        string++;
    }
}

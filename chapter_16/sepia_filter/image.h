#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>

struct pixel {
    uint8_t b, g, r;
};

// start of pixels is top left corner
struct image {
    uint64_t width, height;
    struct pixel* data;
};

struct image create(uint64_t width, uint64_t height);
struct image duplicate(const struct image* const src);
void image_free(struct image* const img);

void sepia_c_inplace(struct image* const img);
void sepia_asm_inplace(struct image* const img);

#endif

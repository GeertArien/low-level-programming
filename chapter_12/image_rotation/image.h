#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>

struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint64_t width, height;
    struct pixel* data;
};

struct image create(uint64_t width, uint64_t height);
void image_free(struct image* const img);
struct image rotate(const struct image* const source);

#endif

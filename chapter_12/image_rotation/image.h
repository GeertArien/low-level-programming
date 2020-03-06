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
void image_free(struct image* const img);

struct image rotate(const struct image* const src, const float degrees);
struct image blur(const struct image* const src);
struct image dilate(const struct image* const src);
struct image erode(const struct image* const src);

#endif

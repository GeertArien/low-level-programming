#include "image.h"
#include <malloc.h>

struct image create(uint64_t width, uint64_t height) {
    struct image img;
    img.height = height;
    img.width = width;
    img.data = (struct pixel*) malloc(width * height * sizeof(struct pixel));
    return img;
}

void image_free(struct image* const img) {
    if (img->data != NULL) {
        free(img->data);
    }
}

struct image rotate(const struct image* const source) {
    size_t i, j;
    const size_t last_j = source->height - 1;
    struct image img = create(source->height, source->width);

    for (i = 0; i < img.height; ++i) {
        const size_t offset = i * img.width;
        for (j = 0; j < img.width; ++j) {
            const size_t offset_src = (last_j - j) * source->width;
            img.data[offset + j] = source->data[offset_src + i];
        }
    }

    return img;
}

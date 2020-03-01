#include "image.h"
#include <string.h>
#include <malloc.h>

typedef void (pixel_fn)(struct pixel* const, const struct pixel* const);

static void max(struct pixel* const target, const struct pixel* const other) {
    if (target->b < other->b) target->b = other->b;
    if (target->g < other->g) target->g = other->g;
    if (target->r < other->r) target->r = other->r;
}

static void min(struct pixel* const target, const struct pixel* const other) {
    if (target->b > other->b) target->b = other->b;
    if (target->g > other->g) target->g = other->g;
    if (target->r > other->r) target->r = other->r;
}

static void add(struct pixel* const target, const struct pixel* const val) {
    target->b += val->b;
    target->g += val->g;
    target->r += val->r;
}

static void divide(struct pixel* const target, const uint8_t div) {
    target->b = target->b / div;
    target->g = target->g / div;
    target->r = target->r / div;
}

static void add_divided(struct pixel* const target, const struct pixel* const val) {
    struct pixel cpy = *val;
    divide(&cpy, 9);
    add(target, &cpy);
}

static void copy_borders(struct image* const img, const struct image* const src) {
    size_t i;
    size_t offset;

    memcpy(img->data, src->data, img->width * sizeof(struct pixel));
    offset = (img->height - 1) * img->width;
    memcpy(img->data + offset, src->data + offset, img->width * sizeof(struct pixel));

    for (i = 0; i < img->height ; ++i) {
        offset = i * img->width;
        img->data[offset] = src->data[offset];
        img->data[offset + img->width - 1] = src->data[offset + img->width - 1];
    }
}

static struct image window_algorithm(const struct image* const src, const struct pixel init_value, pixel_fn* fn) {
    size_t i, j;
    struct image img = create(src->width, src->height);

    copy_borders(&img, src);

    for (i = 1; i < img.height - 1; ++i) {
        size_t offset = i * img.width;
        const size_t offset_min = offset - img.width;
        const size_t offset_plus = offset + img.width;
        for (j = 1; j < img.width - 1; ++j) {
            struct pixel value = init_value;
            fn(&value, src->data + offset_min + j - 1);
            fn(&value, src->data + offset_min + j);
            fn(&value, src->data + offset_min + j + 1);
            fn(&value, src->data + offset + j - 1);
            fn(&value, src->data + offset + j);
            fn(&value, src->data + offset + j + 1);
            fn(&value, src->data + offset_plus + j - 1);
            fn(&value, src->data + offset_plus + j);
            fn(&value, src->data + offset_plus + j + 1);
            img.data[offset + j] = value;
        }
    }

    return img;
}

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

struct image rotate(const struct image* const src) {
    size_t i, j;
    const size_t last_j = src->height - 1;
    struct image img = create(src->height, src->width);

    for (i = 0; i < img.height; ++i) {
        const size_t offset = i * img.width;
        for (j = 0; j < img.width; ++j) {
            const size_t offset_src = (last_j - j) * src->width;
            img.data[offset + j] = src->data[offset_src + i];
        }
    }

    return img;
}

struct image blur(const struct image* const src) {
    return window_algorithm(src, (struct pixel) { 0, 0, 0 }, &add_divided);
}

struct image dilate(const struct image* const src) {
    return window_algorithm(src, (struct pixel) { 255, 255, 255 }, &min);
}

struct image erode(const struct image* const src) {
    return window_algorithm(src, (struct pixel) { 0, 0, 0 }, &max);
}

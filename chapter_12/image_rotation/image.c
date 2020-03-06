#include "image.h"
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

#define M_PI   3.14159265358979323846264338327950288

typedef void (pixel_fn)(struct pixel* const, const struct pixel* const);

static void pixel_max(struct pixel* const target, const struct pixel* const other) {
    if (target->b < other->b) target->b = other->b;
    if (target->g < other->g) target->g = other->g;
    if (target->r < other->r) target->r = other->r;
}

static void pixel_min(struct pixel* const target, const struct pixel* const other) {
    if (target->b > other->b) target->b = other->b;
    if (target->g > other->g) target->g = other->g;
    if (target->r > other->r) target->r = other->r;
}

static void pixel_add(struct pixel* const target, const struct pixel* const val) {
    target->b += val->b;
    target->g += val->g;
    target->r += val->r;
}

static void pixel_divide(struct pixel* const target, const uint8_t div) {
    target->b = target->b / div;
    target->g = target->g / div;
    target->r = target->r / div;
}

static void pixel_add_divided(struct pixel* const target, const struct pixel* const val) {
    struct pixel cpy = *val;
    pixel_divide(&cpy, 9);
    pixel_add(target, &cpy);
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
    size_t i, j, k;
    struct image img = create(src->width, src->height);

    copy_borders(&img, src);

    for (i = 1; i < img.height - 1; ++i) {
        const size_t offset = i * img.width;
        const size_t offset_min = offset - img.width;
        const size_t offset_plus = offset + img.width;

        for (j = 0; j < img.width - 2; ++j) {
            struct pixel value = init_value;

            for (k = 0; k < 3; ++k) {
                const size_t offset_x = j + k;
                fn(&value, src->data + offset_min + offset_x);
                fn(&value, src->data + offset + offset_x);
                fn(&value, src->data + offset_plus + offset_x);
            }
            
            img.data[offset + j + 1] = value;
        }
    }

    return img;
}

static int min(const int lhs, const int rhs) {
    if (lhs < rhs) {
        return lhs;
    }
    else {
        return rhs;
    }
}

static int max(const int lhs, const int rhs) {
    if (lhs > rhs) {
        return lhs;
    }
    else {
        return rhs;
    }
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

struct image rotate(const struct image* const src, const float degrees) {
    size_t i, j;
    float radians, sin, cos;
    float midx, midy, src_midx, src_midy, src_width_f, src_height_f;
    struct image img;
    float corners_x[] = {0, 0, src->width, src->width}; 
    float corners_y[] = {0, src->height, src->height, 0};
    int min_x = INT_MAX;
    int max_x = INT_MIN;
    int min_y = INT_MAX;
    int max_y = INT_MIN;

    radians = fmodf(degrees, 360.) * M_PI / 180.;
    sin = sinf(-radians);
    cos = cosf(-radians);

    for (i = 0; i < 4; ++i) {
        const int x_rot = cos * corners_x[i] - sin * corners_y[i];
        const int y_rot = sin * corners_x[i] + cos * corners_y[i];
        min_x = min(min_x, x_rot);
        max_x = max(max_x, x_rot);
        min_y = min(min_y, y_rot);
        max_y = max(max_y, y_rot);
    }

    img = create(abs(max_x - min_x), abs(max_y - min_y));
    memset(img.data, 0, img.width * img.height * sizeof(struct pixel));

    midx = (float) img.width / 2.;
    midy = (float) img.height / 2.;
    src_width_f = (float) src->width;
    src_height_f = (float) src->height;
    src_midx = src_width_f / 2.;
    src_midy = src_height_f / 2.;
    

    for (i = 0; i < img.height; ++i) {
        const size_t offset = i * img.width;
        const float y = (float) i - midy;
        for (j = 0; j < img.width; ++j) {
            const float x = (float) j - midx;
            const float x_rot = cos * x - sin * y;
            const float y_rot = sin * x + cos * y;
            const float rot_i = x_rot + src_midx;
            const float rot_j = y_rot + src_midy;
            
            if (rot_i > 0. && rot_j > 0. && rot_i < src_width_f && rot_j < src_height_f) {
                const size_t offset_rot = (size_t) rot_j * src->width;
                img.data[offset + j] = src->data[offset_rot + (size_t) rot_i];
            }
        }
    }

    return img;
}

struct image blur(const struct image* const src) {
    return window_algorithm(src, (struct pixel) { 0, 0, 0 }, &pixel_add_divided);
}

struct image dilate(const struct image* const src) {
    return window_algorithm(src, (struct pixel) { 255, 255, 255 }, &pixel_min);
}

struct image erode(const struct image* const src) {
    return window_algorithm(src, (struct pixel) { 0, 0, 0 }, &pixel_max);
}

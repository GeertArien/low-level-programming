#include "image.h"
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

void sse(float[static 12], const float[static 12]);

float const byte_to_float[] = {
    0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f,
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f,
    20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f,
    30.f, 31.f, 32.f, 33.f, 34.f, 35.f, 36.f, 37.f, 38.f, 39.f,
    40.f, 41.f, 42.f, 43.f, 44.f, 45.f, 46.f, 47.f, 48.f, 49.f,
    50.f, 51.f, 52.f, 53.f, 54.f, 55.f, 56.f, 57.f, 58.f, 59.f,
    60.f, 61.f, 62.f, 63.f, 64.f, 65.f, 66.f, 67.f, 68.f, 69.f,
    70.f, 71.f, 72.f, 73.f, 74.f, 75.f, 76.f, 77.f, 78.f, 79.f,
    80.f, 81.f, 82.f, 83.f, 84.f, 85.f, 86.f, 87.f, 88.f, 89.f,
    90.f, 91.f, 92.f, 93.f, 94.f, 95.f, 96.f, 97.f, 98.f, 99.f,
    100.f, 101.f, 102.f, 103.f, 104.f, 105.f, 106.f, 107.f, 108.f, 109.f,
    110.f, 111.f, 112.f, 113.f, 114.f, 115.f, 116.f, 117.f, 118.f, 119.f,
    120.f, 121.f, 122.f, 123.f, 124.f, 125.f, 126.f, 127.f, 128.f, 129.f,
    130.f, 131.f, 132.f, 133.f, 134.f, 135.f, 136.f, 137.f, 138.f, 139.f,
    140.f, 141.f, 142.f, 143.f, 144.f, 145.f, 146.f, 147.f, 148.f, 149.f,
    150.f, 151.f, 152.f, 153.f, 154.f, 155.f, 156.f, 157.f, 158.f, 159.f,
    160.f, 161.f, 162.f, 163.f, 164.f, 165.f, 166.f, 167.f, 168.f, 169.f,
    170.f, 171.f, 172.f, 173.f, 174.f, 175.f, 176.f, 177.f, 178.f, 179.f,
    180.f, 181.f, 182.f, 183.f, 184.f, 185.f, 186.f, 187.f, 188.f, 189.f,
    190.f, 191.f, 192.f, 193.f, 194.f, 195.f, 196.f, 197.f, 198.f, 199.f,
    200.f, 201.f, 202.f, 203.f, 204.f, 205.f, 206.f, 207.f, 208.f, 209.f,
    210.f, 211.f, 212.f, 213.f, 214.f, 215.f, 216.f, 217.f, 218.f, 219.f,
    220.f, 221.f, 222.f, 223.f, 224.f, 225.f, 226.f, 227.f, 228.f, 229.f,
    230.f, 231.f, 232.f, 233.f, 234.f, 235.f, 236.f, 237.f, 238.f, 239.f,
    240.f, 241.f, 242.f, 243.f, 244.f, 245.f, 246.f, 247.f, 248.f, 249.f,
    250.f, 251.f, 252.f, 253.f, 254.f, 255.f
};

struct image create(uint64_t width, uint64_t height) {
    struct image img;
    img.height = height;
    img.width = width;
    img.data = (struct pixel*) malloc(width * height * sizeof(struct pixel));
    return img;
}

struct image duplicate(const struct image* const src) {
    size_t i, j;
    struct image img = create(src->width, src->height);

    for (i = 0; i < img.height; ++i) {
        const size_t offset_y = i * img.width;
        for (j = 0; j < img.width; ++j) {
            const size_t offset = offset_y + j;
            img.data[offset] = src->data[offset];
        }
    }

    return img;
}

void image_free(struct image* const img) {
    if (img->data != NULL) {
        free(img->data);
    }
}

static unsigned char sat(const uint64_t x) {
    if (x < 256) {
        return x;
    }
    return 255;
}

static void sepia_one(struct pixel* const pixel) {
    static const float c[3][3] = {
        { .393f, .769f, .189f },
        { .349f, .686f, .168f },
        { .272f, .543f, .131f }
    };

    struct pixel const old = *pixel;

    pixel->r = sat(old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]);
    pixel->g = sat(old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]);
    pixel->b = sat(old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]);
}

void sepia_c_inplace(struct image* const img) {
    uint32_t x, y;
    
    for (y = 0; y < img->height; ++y) {
        const size_t offset = y * img->width;
        for (x = 0; x < img->width; ++x) {
            sepia_one(img->data + offset + x);
        }
    }
}

void sepia_asm_inplace(struct image* const img) {
    uint32_t x, y;

    static const float c[12] = {
        .393f, .349f, .272f, 0.f,
        .769f, .686f, .543f, 0.f,
        .189f, .168f, .131f, 0.f
    };
    
    for (y = 0; y < img->height; ++y) {
        const size_t offset = y * img->width;
        for (x = 0; x < img->width; ++x) {
            struct pixel* p = img->data + offset + x;

            const float r = byte_to_float[p->r];
            const float g = byte_to_float[p->g];
            const float b = byte_to_float[p->b];

            float pix[12] = {
                r, r, r, 0.f,
                g, g, g, 0.f,
                b, b, b, 0.f
            };

            sse(pix, c);

            p->r = sat(pix[0]);
            p->g = sat(pix[1]);
            p->b = sat(pix[2]);
        }
    }
}

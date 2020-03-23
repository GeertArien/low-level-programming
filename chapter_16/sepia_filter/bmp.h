#ifndef BMP_H_
#define BMP_H_

#include <stdint.h>
#include <stdio.h>
#include "image.h"

struct __attribute__((packed))
bmp_header
{
    uint16_t bfType;
    uint32_t bfFileSize;
    uint32_t bfReserved;
    uint32_t bfDataOffset;
    
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPixelsPerMeter;
    uint32_t biYPixelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

enum read_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
};

enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR
};

enum read_status from_bmp(FILE* in, struct image* const img);
enum write_status to_bmp(FILE* out, const struct image* const img);

#endif

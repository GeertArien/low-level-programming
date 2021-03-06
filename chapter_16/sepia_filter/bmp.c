#include "bmp.h"
#include <malloc.h>

static void print(const struct bmp_header* const header) {
    puts("file header:");
    printf("type: \t%x\n", header->bfType);
    printf("filesize: \t%d\n", header->bfFileSize);
    printf("data offset: \t%d\n", header->bfDataOffset);
    puts("info header:");
    printf("info header size: \t%d\n", header->biSize);
    printf("width: \t%d\n", header->biWidth);
    printf("height: \t%d\n", header->biHeight);
    printf("planes: \t%d\n", header->biPlanes);
    printf("bit count: \t%d\n", header->biBitCount);
    printf("compression: \t%d\n", header->biCompression);
    printf("image size: \t%d\n", header->biSizeImage);
    printf("x ppm: \t%d\n", header->biXPixelsPerMeter);
    printf("y ppm: \t%d\n", header->biYPixelsPerMeter);
    printf("clr used: \t%d\n", header->biClrUsed);
    printf("clr important: \t%d\n", header->biClrImportant);
}

static struct bmp_header create_header(const struct image* const img) {
    return (struct bmp_header) { 
        .bfType = 0x4d42, 
        .bfDataOffset = sizeof(struct bmp_header),
        .biSize = sizeof(struct bmp_header) - 14,
        .biWidth = img->width,
        .biHeight = img->height,
        .biPlanes = 1,
        .biBitCount = sizeof(struct pixel) * 8 
    };
}

enum read_status from_bmp(FILE* in, struct image* const img) {
    struct bmp_header header;
    size_t i, j;
    long padding;
    
    if (fread(&header, sizeof(header), 1, in) < 1) {
        return READ_INVALID_HEADER;
    }

    if (header.bfType != 0x4d42) {
        return READ_INVALID_SIGNATURE;
    }

    *img = create(header.biWidth, header.biHeight);
    
    fseek(in, (long) header.bfDataOffset, SEEK_SET);

    padding = (img->width * sizeof(struct pixel)) % 4;

    if (padding != 0) {
        padding = 4 - padding;
    }
    
    // start of bmp data is bottom left corner
    i = img->height;
    do {
        --i;
        long offset = i * img->width;
        for (j = 0; j < img->width; ++j) {
            if (fread(img->data + offset + j, sizeof(struct pixel), 1, in) < 1) {
                return READ_INVALID_BITS;
            }
        }
        fseek(in, padding, SEEK_CUR);
    } while (i != 0);

    return READ_OK;
}

enum write_status to_bmp(FILE* out, const struct image* const img) {
    int i, j;
    long padding;
    uint32_t f_size;
    struct bmp_header header;
    char pad_value = 0;

    padding = (img->width * sizeof(struct pixel)) % 4;

    if (padding != 0) {
        padding = 4 - padding; 
    }

    f_size = sizeof(struct bmp_header) + (img->height * img->width * sizeof(struct pixel)) + padding;

    header = create_header(img);
    header.bfFileSize = f_size;

    if (fwrite(&header, sizeof(struct bmp_header), 1, out) < 1) {
        return WRITE_ERROR;
    }

    // start of bmp data is bottom left corner
    i = img->height;
    do {
        --i;
        long offset = i * img->width;
        for (j = 0; j < img->width; ++j) {
            if (fwrite(img->data + offset + j, sizeof(struct pixel), 1, out) < 1) {
                return READ_INVALID_BITS;
            }
        }
        fwrite(&pad_value, 1, padding, out);
    } while (i != 0);

    return WRITE_OK;
}



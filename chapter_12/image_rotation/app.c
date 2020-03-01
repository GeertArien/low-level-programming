#include "stdio.h"
#include "image.h"
#include "bmp.h"

static void print_read_error(const enum read_status status) {
    switch (status) {
        case READ_INVALID_BITS:
            fprintf(stderr, "BMP reader: invalid bit.\n");
            break;
        case READ_INVALID_HEADER:
            fprintf(stderr, "BMP reader: invalid header.\n");
            break;
        case READ_INVALID_SIGNATURE:
            fprintf(stderr, "BMP reader: invalid signature.\n");
            break;
        case READ_OK:
            break;
    }
}

static void print_write_error(const enum write_status status) {
    if (status == WRITE_ERROR) {
        fprintf(stderr, "BMP writer: write error.\n");
    }
}

int main(void) {
    enum read_status r_status;
    enum write_status w_status;
    struct image img;
    struct image rotated_img;
    FILE* file = fopen("test.bmp", "r");

    if (file == NULL) {
        puts("Unable to open file");
        fclose(file);
    }

    r_status = from_bmp(file, &img);
    fclose(file);

    if (r_status != READ_OK) {
        print_read_error(r_status);
        return 1;
    }

    rotated_img = rotate(&img);

    file = fopen("rotated.bmp", "w");
    if (file == NULL) {
        puts("Unable to open file");
        fclose(file);
    }

    w_status = to_bmp(file, &rotated_img);
    fclose(file);

    image_free(&rotated_img);
    image_free(&img);

    if (w_status != WRITE_OK) {
        print_write_error(w_status);
        return 1;
    }

    return 0;
}


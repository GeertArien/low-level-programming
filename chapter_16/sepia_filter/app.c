#include <stdio.h>
#include "image.h"
#include "bmp.h"

static const char* input_file = "fam.bmp";
static const char* sepia_file = "sepia.bmp";

typedef struct image (image_function)(const struct image* const src);

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

static void test_sepia(const char* file_name, struct image* const src) {
    FILE* file;
    enum write_status w_status;
    
    sepia_c_inplace(src);

    file = fopen(file_name, "w");
    if (file == NULL) {
        puts("Unable to open file");
        //image_free(&img);
        return;
    }

    w_status = to_bmp(file, src);
    fclose(file);
    //image_free(&img);
    
    if (w_status != WRITE_OK) {
        print_write_error(w_status);
    }
}

int main(void) {
    enum read_status r_status;
    struct image img;
    FILE* file = fopen(input_file, "r");

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

    test_sepia(sepia_file, &img);
   
    image_free(&img);

    return 0;
}


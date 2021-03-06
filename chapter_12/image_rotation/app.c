#include <stdio.h>
#include "image.h"
#include "bmp.h"

static const char* input_file = "test.bmp";
static const char* blurred_file = "blurred.bmp";
static const char* dilated_file = "dilated.bmp";
static const char* eroded_file = "eroded.bmp";
static const char* rotate_180_file = "rotated_180.bmp";
static const char* rotate_450_file = "rotated_450.bmp";
static const char* rotate_223_file = "rotated_223.bmp";

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

static void test(const char* file_name, const struct image* const src, image_function* fn) {
    FILE* file;
    enum write_status w_status;
    struct image img;
    
    img = fn(src);

    file = fopen(file_name, "w");
    if (file == NULL) {
        puts("Unable to open file");
        image_free(&img);
        return;
    }

    w_status = to_bmp(file, &img);
    fclose(file);
    image_free(&img);
    
    if (w_status != WRITE_OK) {
        print_write_error(w_status);
    }
}

static void test_rotation(const char* file_name, const struct image* const src, const float degrees) {
    FILE* file;
    enum write_status w_status;
    struct image img;
    
    img = rotate(src, degrees);

    file = fopen(file_name, "w");
    if (file == NULL) {
        puts("Unable to open file");
        image_free(&img);
        return;
    }

    w_status = to_bmp(file, &img);
    fclose(file);
    image_free(&img);
    
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

    test(blurred_file, &img, blur);
    test(dilated_file, &img, dilate);
    test(eroded_file, &img, erode);
    test_rotation(rotate_180_file, &img, 180.);
    test_rotation(rotate_450_file, &img, 450.);
    test_rotation(rotate_223_file, &img, 223.);
   
    image_free(&img);

    return 0;
}


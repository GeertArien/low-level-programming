#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include "image.h"
#include "bmp.h"

static const char* input_file = "fam.bmp";
static const char* sepia_c_file = "sepia_c.bmp";
static const char* sepia_asm_file = "sepia_asm.bmp";

typedef void (sepia_fn)(struct image* const img);

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

static void test_sepia(const char* file_name, const struct image* const src, sepia_fn* sepia) {
    struct rusage r;
    struct timeval start, end;
    long res;
    FILE* file;
    enum write_status w_status;
    struct image img = duplicate(src);
    
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;
    
    sepia(&img);

    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;

    res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec -start.tv_usec;

    printf("Time elapsed in microseconds: %ld\n", res);

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

    puts("Image sepia filter without SSE.");
    test_sepia(sepia_c_file, &img, &sepia_c_inplace);

    puts("Image sepia filter with SSE.");
    test_sepia(sepia_asm_file, &img, &sepia_asm_inplace);
   
    image_free(&img);

    return 0;
}


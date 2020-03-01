#include "image.h"
#include <malloc.h>

void image_free(struct image* const img) {
    if (img->data != NULL) {
        free(img->data);
    }
}

#include <stdio.h>
#include "bmp8.h"

int main() {
    t_bmp8 *img = bmp8_loadImage("images/barbara_gray.bmp");
    if (!img) return 1;

    bmp8_printInfo(img);
    bmp8_negative(img);
    bmp8_saveImage("images/barbara_gray_negative.bmp", img);

    bmp8_free(img);
    return 0;
}

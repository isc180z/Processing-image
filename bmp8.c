#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = malloc(sizeof(t_bmp8));
    fread(img->header, sizeof(unsigned char), 54, file);
    img->width = *(unsigned int*)&img->header[18];
    img->height = *(unsigned int*)&img->header[22];
    img->colorDepth = *(unsigned int*)&img->header[28];
    img->dataSize = *(unsigned int*)&img->header[34];

    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    img->data = (unsigned char*)malloc(img->dataSize * sizeof(unsigned char));
    fread(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    return img;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur : impossible d'écrire dans le fichier %s\n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
}

void bmp8_free(t_bmp8 *img) {
    free(img->data);
    free(img);
}

void bmp8_printInfo(t_bmp8 *img) {
    printf("Image Info:\n");
    printf("Largeur: %u\nHauteur: %u\nProfondeur: %u bits\nTaille: %u octets\n",
           img->width, img->height, img->colorDepth, img->dataSize);
}

void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + value;
        img->data[i] = (newValue > 255) ? 255 : (newValue < 0) ? 0 : newValue;
    }
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    int n = kernelSize / 2;
    unsigned char *newData = malloc(img->dataSize);

    for (unsigned int y = n; y < img->height - n; y++) {
        for (unsigned int x = n; x < img->width - n; x++) {
            float sum = 0;
            for (int ky = -n; ky <= n; ky++) {
                for (int kx = -n; kx <= n; kx++) {
                    int pixelIndex = (y + ky) * img->width + (x + kx);
                    sum += img->data[pixelIndex] * kernel[ky + n][kx + n];
                }
            }
            newData[y * img->width + x] = (sum > 255) ? 255 : (sum < 0) ? 0 : (unsigned char)sum;
        }
    }

    free(img->data);
    img->data = newData;
}



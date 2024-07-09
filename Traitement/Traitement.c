#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "Traitement.h"

// Fonction pour allouer de la mémoire pour une image PGM
PGMImage *allocatePGMImage(int width, int height) {
    PGMImage *image = (PGMImage *)malloc(sizeof(PGMImage));
    image->width = width;
    image->height = height;
    image->pixels = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        image->pixels[i] = (int *)malloc(width * sizeof(int));
    }
    return image;
}

// Fonction pour libérer la mémoire allouée pour une image PGM
void freePGMImage(PGMImage *image) {
    for (int i = 0; i < image->height; i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}

// Fonction pour lire une image PGM à partir d'un fichier
PGMImage *readPGMImage(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }
    char magic_number[3];
    fscanf(file, "%2s", magic_number);
    if (magic_number[0] != 'P' || magic_number[1] != '5') {
        fprintf(stderr, "Format d'image non pris en charge (uniquement PGM P5)");
        fclose(file);
        return NULL;
    }
    int width, height, max_gray_value;
    fscanf(file, "%d %d %d", &width, &height, &max_gray_value);
    fgetc(file); // Pour consommer le caractère de fin de ligne
    PGMImage *image = allocatePGMImage(width, height);
    image->max_gray_value = max_gray_value;
    // Lire les pixels de l'image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image->pixels[i][j] = fgetc(file);
        }
    }
    fclose(file);
    return image;
}

// Fonction pour améliorer le contraste de l'image
PGMImage *enhanceContrast(const PGMImage *image) {
    int min_value = MAX_GRAY_LEVEL;
    int max_value = 0;
    // Trouver la valeur minimale et maximale des pixels
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int pixel_value = image->pixels[i][j];
            if (pixel_value < min_value) {
                min_value = pixel_value;
            }
            if (pixel_value > max_value) {
                max_value = pixel_value;
            }
        }
    }

    // Créer une nouvelle image pour stocker le résultat
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    // Appliquer la transformation linéaire pour étirer le contraste
    float slope = (float)(MAX_GRAY_LEVEL - 1) / (max_value - min_value);
    float intercept = -slope * min_value;
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int new_value = slope * image->pixels[i][j] + intercept;
            if (new_value < 0) {
                new_value = 0;
            } else if (new_value > MAX_GRAY_LEVEL - 1) {
                new_value = MAX_GRAY_LEVEL - 1;
            }
            result_image->pixels[i][j] = new_value;
        }
    }
    return result_image;
}


// Fonction pour appliquer une transformation linéaire à l'image
PGMImage *linearTransformation(const PGMImage *image, float slope, float intercept) {
    // Créer une nouvelle image pour stocker le résultat
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int new_value = slope * image->pixels[i][j] + intercept;
            if (new_value < 0) {
                new_value = 0;
            } else if (new_value > MAX_GRAY_LEVEL - 1) {
                new_value = MAX_GRAY_LEVEL - 1;
            }
            result_image->pixels[i][j] = new_value;
        }
    }
    return result_image;
}

//fonction pour appliquer la luminance sur la photo
PGMImage *luminance(const PGMImage *image, double scale_factor) {
    // Créer une nouvelle image pour stocker le résultat
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int new_value = (int)(image->pixels[i][j] * scale_factor);
            if (new_value < 0) {
                new_value = 0;
            } else if (new_value > MAX_GRAY_LEVEL - 1) {
                new_value = MAX_GRAY_LEVEL - 1;
            }
            result_image->pixels[i][j] = new_value;
        }
    }
    return result_image;
}

// Fonction pour appliquer une transformation linéaire par morceau à l'image
PGMImage *piecewiseLinearTransformation(const PGMImage *image, int threshold) {
    // Créer une nouvelle image pour stocker le résultat
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            if (image->pixels[i][j] < threshold) {
                result_image->pixels[i][j] = (int)(0.5 * image->pixels[i][j]);
            } else {
                result_image->pixels[i][j] = (int)(1.5 * image->pixels[i][j]);
            }
        }
    }
    return result_image;
}

// Fonction pour appliquer une transformation non-linéaire à l'image
PGMImage *nonlinearTransformation(const PGMImage *image) {
    // Créer une nouvelle image pour stocker le résultat
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            result_image->pixels[i][j] = MAX_GRAY_LEVEL - 1 - image->pixels[i][j];
        }
    }
    return result_image;
}

// Fonction pour appliquer une transformation linéaire avec saturation à l'image
PGMImage *linearTransformationWithSaturation(const PGMImage *image, float slope, float intercept) {
    // Créer une nouvelle image pour stocker le résultat
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int new_value = slope * image->pixels[i][j] + intercept;
            if (new_value < 0) {
                new_value = 0;
            } else if (new_value > MAX_GRAY_LEVEL - 1) {
                new_value = MAX_GRAY_LEVEL - 1;
            }
            result_image->pixels[i][j] = new_value;
        }
    }
    return result_image;
}

// Fonction pour égaliser l'histogramme de l'image
PGMImage *histogramEqualization(const PGMImage *image) {
    // Créer une nouvelle image pour stocker le résultat
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    int histogram[MAX_GRAY_LEVEL] = {0};
    // Calculer l'histogramme
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            histogram[image->pixels[i][j]]++;
        }
    }
    // Calculer la fonction de distribution cumulative
    int cdf[MAX_GRAY_LEVEL] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < MAX_GRAY_LEVEL; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }
    // Égaliser l'histogramme
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int new_value = (int)(((float)cdf[image->pixels[i][j]] / (image->width * image->height)) * (MAX_GRAY_LEVEL - 1));
            result_image->pixels[i][j] = new_value;
        }
    }
    return result_image;
}

// Fonction pour sauvegarder une image PGM dans un fichier
void savePGMImage(const char *filename, const PGMImage *image) {
    FILE *file = fopen(filename, "wb"); // Ouvrir en mode binaire
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        return;
    }
    // Trouver la valeur minimale et maximale des pixels dans l'image
    int min_value = image->pixels[0][0];
    int max_value = image->pixels[0][0];
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            if (image->pixels[i][j] < min_value) {
                min_value = image->pixels[i][j];
            }
            if (image->pixels[i][j] > max_value) {
                max_value = image->pixels[i][j];
            }
        }
    }
    // Écrire l'en-tête de l'image PGM
    fprintf(file, "P5\n%d %d\n%d\n", image->width, image->height, 255);
    // Rééchelonner et écrire les pixels de l'image
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int scaled_value = (int)(((double)(image->pixels[i][j] - min_value) / (max_value - min_value)) * 255.0);
            fputc((unsigned char)scaled_value, file);
        }
    }
    fclose(file);
}

/*
    ici commence image 2
*/

// Fonction pour vérifier si deux images ont les mêmes dimensions
int sameDimensions(const PGMImage *image1, const PGMImage *image2) {
    return image1->width == image2->width && image1->height == image2->height;
}

// Fonction pour appliquer l'opération logique ET entre deux images
PGMImage *logicalAND(const PGMImage *image1, const PGMImage *image2) {
    if (!sameDimensions(image1, image2)) {
        fprintf(stderr, "Les dimensions des images ne correspondent pas pour l'opération AND.\n");
        return NULL;
    }

    PGMImage *result_image = allocatePGMImage(image1->width, image1->height);
    for (int i = 0; i < image1->height; i++) {
        for (int j = 0; j < image1->width; j++) {
            result_image->pixels[i][j] = image1->pixels[i][j] & image2->pixels[i][j];
        }
    }
    return result_image;
}

// Fonction pour appliquer l'opération logique OU entre deux images
PGMImage *logicalOR(const PGMImage *image1, const PGMImage *image2) {
    if (!sameDimensions(image1, image2)) {
        fprintf(stderr, "Les dimensions des images ne correspondent pas pour l'opération OR.\n");
        return NULL;
    }

    PGMImage *result_image = allocatePGMImage(image1->width, image1->height);
    for (int i = 0; i < image1->height; i++) {
        for (int j = 0; j < image1->width; j++) {
            result_image->pixels[i][j] = image1->pixels[i][j] | image2->pixels[i][j];
        }
    }
    return result_image;
}

// Fonction pour additionner deux images
PGMImage *addImages(const PGMImage *image1, const PGMImage *image2) {
    if (!sameDimensions(image1, image2)) {
        fprintf(stderr, "Les dimensions des images ne correspondent pas pour l'opération d'addition.\n");
        return NULL;
    }

    PGMImage *result_image = allocatePGMImage(image1->width, image1->height);
    for (int i = 0; i < image1->height; i++) {
        for (int j = 0; j < image1->width; j++) {
            int sum = image1->pixels[i][j] + image2->pixels[i][j];
            result_image->pixels[i][j] = (sum > 255) ? 255 : sum;
        }
    }
    return result_image;
}

// Fonction pour soustraire deux images
PGMImage *subtractImages(const PGMImage *image1, const PGMImage *image2) {
    if (!sameDimensions(image1, image2)) {
        fprintf(stderr, "Les dimensions des images ne correspondent pas pour l'opération de soustraction.\n");
        return NULL;
    }

    PGMImage *result_image = allocatePGMImage(image1->width, image1->height);
    for (int i = 0; i < image1->height; i++) {
        for (int j = 0; j < image1->width; j++) {
            int diff = image1->pixels[i][j] - image2->pixels[i][j];
            result_image->pixels[i][j] = (diff < 0) ? 0 : diff;
        }
    }
    return result_image;
}

// Fonction pour multiplier une image par un facteur
PGMImage *multiplyImage(const PGMImage *image, float factor) {
    PGMImage *result_image = allocatePGMImage(image->width, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int product = image->pixels[i][j] * factor;
            result_image->pixels[i][j] = (product > 255) ? 255 : product;
        }
    }
    return result_image;
}

// Fonction pour appliquer l'interpolation du plus proche voisin à une image
PGMImage *nearestNeighborInterpolation(const PGMImage *image, int new_width, int new_height) {
    PGMImage *result_image = allocatePGMImage(new_width, new_height);

    float x_ratio = (float)image->width / new_width;
    float y_ratio = (float)image->height / new_height;

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            int px = (int)(j * x_ratio);
            int py = (int)(i * y_ratio);
            result_image->pixels[i][j] = image->pixels[py][px];
        }
    }
    return result_image;
}

// Fonction pour appliquer l'interpolation bilinéaire à une image
PGMImage *bilinearInterpolation(const PGMImage *image, int new_width, int new_height) {
    PGMImage *result_image = allocatePGMImage(new_width, new_height);

    float x_ratio = (float)(image->width - 1) / new_width;
    float y_ratio = (float)(image->height - 1) / new_height;
    float x_diff, y_diff;
    int x, y;
    int index;

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            x = (int)(x_ratio * j);
            y = (int)(y_ratio * i);
            x_diff = (x_ratio * j) - x;
            y_diff = (y_ratio * i) - y;
            index = y * image->width + x;

            int A = image->pixels[y][x];
            int B = image->pixels[y][x + 1];
            int C = image->pixels[y + 1][x];
            int D = image->pixels[y + 1][x + 1];

            result_image->pixels[i][j] = (int)(
                A * (1 - x_diff) * (1 - y_diff) +
                B * x_diff * (1 - y_diff) +
                C * y_diff * (1 - x_diff) +
                D * x_diff * y_diff
            );
        }
    }
    return result_image;
}

// Fonction pour appliquer l'interpolation bicubique à une image
PGMImage *bicubicInterpolation(const PGMImage *image, int new_width, int new_height) {
    // Cette fonction est complexe à implémenter entièrement ici.
    // Nous utiliserons une version simplifiée.
    PGMImage *result_image = allocatePGMImage(new_width, new_height);

    // Coefficients pour l'interpolation bicubique
    float x_ratio = (float)(image->width - 1) / new_width;
    float y_ratio = (float)(image->height - 1) / new_height;
    int x, y;
    float x_diff, y_diff;
    int index;

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            x = (int)(x_ratio * j);
            y = (int)(y_ratio * i);
            x_diff = (x_ratio * j) - x;
            y_diff = (y_ratio * i) - y;
            index = y * image->width + x;

            // Calculer la valeur interpolée
            float sum = 0.0;
            for (int m = -1; m <= 2; m++) {
                for (int n = -1; n <= 2; n++) {
                    int px = x + n;
                    int py = y + m;
                    if (px >= 0 && px < image->width && py >= 0 && py < image->height) {
                        float coeff = (1.0 - abs(x_diff - n)) * (1.0 - abs(y_diff - m));
                        sum += coeff * image->pixels[py][px];
                    }
                }
            }

            result_image->pixels[i][j] = (int)sum;
        }
    }
    return result_image;
}

/*
    image 3 commence
*/
// Fonction pour afficher une image PGM
void printPGMImage(PGMImage *image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            printf("%d ", image->pixels[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour écrire une image PGM dans un fichier
void writePGMImage(const char *filename, const PGMImage *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Cannot open file to write");
        return;
    }

    fprintf(file, "P5\n%d %d\n%d\n", image->width, image->height, image->max_value);

    for (int i = 0; i < image->height; i++) {
        fwrite(image->pixels[i], sizeof(unsigned char), image->width, file);
    }

    fclose(file);
}

// Fonctions de transformation d'image et de filtrage
void pointTransform(PGMImage *image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            image->pixels[i][j] = 255 - image->pixels[i][j]; // Inversion des couleurs
        }
    }
}

//transformation local
void localTransform(PGMImage *image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            image->pixels[i][j] = (image->pixels[i][j] + 10) % 256; // Exemple: ajout de 10
        }
    }
}

//transformation global
void globalTransform(PGMImage *image) {
    int sum = 0;
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            sum += image->pixels[i][j];
        }
    }
    int mean = sum / (image->height * image->width);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            image->pixels[i][j] = mean;
        }
    }
}

//fonction pour appliquer la convolution sur une image en pgm
void convolution(PGMImage *image, int kernel[3][3]) {
    PGMImage *result = allocatePGMImage(image->width, image->height);
    for (int i = 1; i < image->height - 1; i++) {
        for (int j = 1; j < image->width - 1; j++) {
            int sum = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    sum += image->pixels[i + k][j + l] * kernel[k + 1][l + 1];
                }
            }
            result->pixels[i][j] = sum;
        }
    }
    for (int i = 1; i < image->height - 1; i++) {
        for (int j = 1; j < image->width - 1; j++) {
            image->pixels[i][j] = result->pixels[i][j];
        }
    }
    freePGMImage(result);
}

//filtre moyen pour la convolution
void meanFilter(PGMImage *image) {
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    convolution(image, kernel);
}

//filtre gausian
void gaussianFilter(PGMImage *image) {
    int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    convolution(image, kernel);
}

//filtre median
void medianFilter(PGMImage *image) {
    int temp[9];
    PGMImage *result = allocatePGMImage(image->width, image->height);

    for (int i = 1; i < image->height - 1; i++) {
        for (int j = 1; j < image->width - 1; j++) {
            int k = 0;
            for (int u = -1; u <= 1; u++) {
                for (int v = -1; v <= 1; v++) {
                    temp[k++] = image->pixels[i + u][j + v];
                }
            }
            // Sorting the array
            for (int m = 0; m < 9 - 1; m++) {
                for (int n = m + 1; n < 9; n++) {
                    if (temp[m] > temp[n]) {
                        int swap = temp[m];
                        temp[m] = temp[n];
                        temp[n] = swap;
                    }
                }
            }
            result->pixels[i][j] = temp[4]; // Median value
        }
    }

    for (int i = 1; i < image->height - 1; i++) {
        for (int j = 1; j < image->width - 1; j++) {
            image->pixels[i][j] = result->pixels[i][j];
        }
    }
    freePGMImage(result);
}

//filtre passe haut
void highPassFilter(PGMImage *image) {
    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };
    convolution(image, kernel);
}

//filtre passe bas
void lowPassFilter(PGMImage *image) {
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    convolution(image, kernel);
}

//appliquer les transformation
void applyAndSaveTransform(PGMImage *image, void (*transform)(PGMImage *), const char *outputFilename) {
    PGMImage *transformedImage = allocatePGMImage(image->width, image->height);
    
    // Copier les pixels de l'image originale
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            transformedImage->pixels[i][j] = image->pixels[i][j];
        }
    }
    
    // Appliquer la transformation
    transform(transformedImage);
    
    // Sauvegarder l'image transformée
    savePGMImage(outputFilename, transformedImage);
    
    // Libérer l'image transformée
    freePGMImage(transformedImage);
}

//fonction pour afficher le menu
void printMenu() {
    printf("\nMenu de transformation d'image \n");
    printf("1. Application des transformations de base \n");
    printf("2. Operation logique sur l'image \n");
    printf("3. Transformation de fourier \n");
    printf("4. Detection des contours \n");
    printf("5. Binarisation\n");
    printf("0. Quitter\n");
}

/*
    image4 commence
*/

// Fonction pour détecter les contours en utilisant l'algorithme de Sobel
PGMImage *detectEdges(const PGMImage *image) {
    int width = image->width;
    int height = image->height;

    PGMImage *edges = allocatePGMImage(width, height);

    int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumX = 0;
            int sumY = 0;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sumX += gx[i + 1][j + 1] * image->pixels[y + i][x + j];
                    sumY += gy[i + 1][j + 1] * image->pixels[y + i][x + j];
                }
            }

            int magnitude = (int)sqrt(sumX * sumX + sumY * sumY);
            if (magnitude > 255) {
                magnitude = 255;
            }
            edges->pixels[y][x] = (unsigned char)magnitude;
        }
    }
    return edges;
}

// Fonction pour la segmentation d'images en utilisant le seuillage
PGMImage *segmentImage(const PGMImage *image, unsigned char threshold) {
    int width = image->width;
    int height = image->height;

    PGMImage *segmented = allocatePGMImage(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            segmented->pixels[y][x] = (image->pixels[y][x] >= threshold) ? 255 : 0;
        }
    }

    return segmented;
}

// Fonction de detection de contours en utilisant le seuillage global
void globalThresholding(PGMImage *input, PGMImage *output, unsigned char threshold) {
    for (int y = 0; y < input->height; y++) {
        for (int x = 0; x < input->width; x++) {
            if (input->pixels[y][x] >= threshold) {
                output->pixels[y][x] = 255;
            } else {
                output->pixels[y][x] = 0;
            }
        }
    }
}

// Détection de contours par l'opérateur de Prewitt
PGMImage *prewittEdgeDetection(const PGMImage *image) {
    int width = image->width;
    int height = image->height;

    PGMImage *edges = allocatePGMImage(width, height);

    int prewitt_x[3][3] = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };

    int prewitt_y[3][3] = {
        {-1, -1, -1},
        { 0,  0,  0},
        { 1,  1,  1}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int gx = 0;
            int gy = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    gx += prewitt_x[ky + 1][kx + 1] * image->pixels[y + ky][x + kx];
                    gy += prewitt_y[ky + 1][kx + 1] * image->pixels[y + ky][x + kx];
                }
            }
            edges->pixels[y][x] = (unsigned char)(sqrt(gx * gx + gy * gy) / sqrt(2));
        }
    }

    return edges;
}

//detection de contours en utilisant le Seuillage adaptatif
PGMImage *adaptiveThresholding(const PGMImage *image, int windowSize, double C) {
    int width = image->width;
    int height = image->height;

    PGMImage *output = allocatePGMImage(width, height);
    int halfWindowSize = windowSize / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sum = 0, count = 0;
            for (int ky = -halfWindowSize; ky <= halfWindowSize; ky++) {
                for (int kx = -halfWindowSize; kx <= halfWindowSize; kx++) {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        sum += image->pixels[ny][nx];
                        count++;
                    }
                }
            }
            double mean = (double)sum / count;
            output->pixels[y][x] = (image->pixels[y][x] > mean - C) ? 255 : 0;
        }
    }

    return output;
}

// Segmentation par k-means
PGMImage *kMeansSegmentation(const PGMImage *image, int k) {
    int width = image->width;
    int height = image->height;

    PGMImage *output = allocatePGMImage(width, height);
    int *clusters = (int *)malloc(width * height * sizeof(int));
    double *means = (double *)malloc(k * sizeof(double));
    int *counts = (int *)malloc(k * sizeof(int));

    for (int i = 0; i < k; i++) {
        means[i] = (double)(i * 255 / (k - 1));
    }

    for (int iter = 0; iter < 100; iter++) {
        for (int i = 0; i < k; i++) {
            counts[i] = 0;
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int minIndex = 0;
                double minDist = fabs(image->pixels[y][x] - means[0]);
                for (int i = 1; i < k; i++) {
                    double dist = fabs(image->pixels[y][x] - means[i]);
                    if (dist < minDist) {
                        minDist = dist;
                        minIndex = i;
                    }
                }
                clusters[y * width + x] = minIndex;
                counts[minIndex]++;
            }
        }

        for (int i = 0; i < k; i++) {
            means[i] = 0;
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                means[clusters[y * width + x]] += image->pixels[y][x];
            }
        }

        for (int i = 0; i < k; i++) {
            if (counts[i] > 0) {
                means[i] /= counts[i];
            }
        }
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            output->pixels[y][x] = (unsigned char)(means[clusters[y * width + x]]);
        }
    }

    free(clusters);
    free(means);
    free(counts);
    return output;
}

/*
    image 5 commence
*/

//allocation des image binaire
BinaryImage* allocateBinaryImage(int width, int height) {
    BinaryImage *image = (BinaryImage *)malloc(sizeof(BinaryImage));
    if (image == NULL) {
        printf("Error: Unable to allocate memory for BinaryImage structure.\n");
        exit(EXIT_FAILURE);
    }
    image->width = width;
    image->height = height;
    image->pixels = (unsigned char **)malloc(height * sizeof(unsigned char *));
    if (image->pixels == NULL) {
        printf("Error: Unable to allocate memory for pixels.\n");
        exit(EXIT_FAILURE);
    }
    image->pixels[0] = (unsigned char *)calloc(width * height, sizeof(unsigned char));
    if (image->pixels[0] == NULL) {
        printf("Error: Unable to allocate memory for pixel data.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < height; i++) {
        image->pixels[i] = image->pixels[0] + i * width;
    }
    return image;
}

//fonction pour liberer les image binaire
void freeBinaryImage(BinaryImage *image) {
    if (image != NULL) {
        if (image->pixels != NULL) {
            if (image->pixels[0] != NULL) {
                free(image->pixels[0]);
            }
            free(image->pixels);
        }
        free(image);
    }
}

//fonction de lecture et transformation en image binaire
void readPGMImagebin(const char *filename, BinaryImage *image) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Unable to open file for reading.\n");
        exit(EXIT_FAILURE);
    }

    // Read PGM header
    char format[3];
    int maxval;
    fscanf(file, "%2s", format);
    if (strcmp(format, "P5") != 0) {
        printf("Error: Not a valid PGM file (must be P5 format).\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d %d %d", &image->width, &image->height, &maxval);
    fgetc(file);  // Consume the newline character after maxval

    // Print image dimensions
    printf("Image dimensions: %d x %d\n", image->width, image->height);

    // Allocate memory for pixels if necessary
    if (image->pixels == NULL) {
        image->pixels = (unsigned char **)malloc(image->height * sizeof(unsigned char *));
        image->pixels[0] = (unsigned char *)calloc(image->width * image->height, sizeof(unsigned char));
        for (int i = 1; i < image->height; i++) {
            image->pixels[i] = image->pixels[0] + i * image->width;
        }
    } else {
        // Check if we need to reallocate
        if (image->width != WIDTH || image->height != HEIGHT) {
            free(image->pixels[0]);
            free(image->pixels);
            image->pixels = (unsigned char **)malloc(image->height * sizeof(unsigned char *));
            image->pixels[0] = (unsigned char *)calloc(image->width * image->height, sizeof(unsigned char));
            for (int i = 1; i < image->height; i++) {
                image->pixels[i] = image->pixels[0] + i * image->width;
            }
        }
    }

    // Read pixel data
    size_t read = fread(image->pixels[0], sizeof(unsigned char), image->width * image->height, file);
    if (read != image->width * image->height) {
        printf("Error: Unable to read pixel data.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Print the first 10x10 pixels for debugging
    printf("First 10x10 pixels of the image:\n");
    for (int y = 0; y < 10 && y < image->height; y++) {
        for (int x = 0; x < 10 && x < image->width; x++) {
            printf("%3d ", image->pixels[y][x]);
        }
        printf("\n");
    }
    printf("\n");

    fclose(file);
}

//ecriture image binaire
void writeBinaryImage(const char *filename, BinaryImage *image) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        exit(EXIT_FAILURE);
    }

    // Write PBM header
    fprintf(file, "P4\n%d %d\n", image->width, image->height);
    
    // Write image data
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x += 8) {
            unsigned char byte = 0;
            for (int bit = 0; bit < 8; bit++) {
                if (x + bit < image->width && image->pixels[y][x + bit] == 255) {
                    byte |= (1 << (7 - bit));
                }
            }
            fputc(byte, file);
        }
    }

    fclose(file);
}

//fonction de binarization
void binarizeImage(BinaryImage *image, unsigned char threshold) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            if(image->pixels[y][x] > threshold){
                image->pixels[y][x]=255;    
            }else{
                image->pixels[y][x] =0;
            }
        }
    }
}

// Connexité-4 et Connexité-8
bool isConnected4(BinaryImage *image, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    return (dx + dy == 1);
}

bool isConnected8(BinaryImage *image, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    return (dx <= 1 && dy <= 1);
}

// Distance de Manhattan (D4) et Distance de l'échiquier (D8)
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int chessboardDistance(int x1, int y1, int x2, int y2) {
    return fmax(abs(x1 - x2), abs(y1 - y2));
}

// Etiquetage des composantes connexes
void labelConnectedComponents(BinaryImage *image) {
    int label = 1;
    int *labels = (int *)calloc(image->width * image->height, sizeof(int));
    if (labels == NULL) {
        printf("Error: Unable to allocate memory for labels.\n");
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            if (image->pixels[y][x] == 255 && labels[y * image->width + x] == 0) {
                // New component found, label it
                labels[y * image->width + x] = label;
                label++;
            }
        }
    }
    // Write labels back to image (for demonstration)
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            image->pixels[y][x] = labels[y * image->width + x] > 0 ? 255 : 0;
        }
    }
    free(labels);
}

// Opérateurs morphologiques

//fonction d'errosion
BinaryImage *binaryErosion(BinaryImage *inputImage, int structuringElement[3][3]) {
    binarizeImage(inputImage, THRESHOLD);
    BinaryImage *resultImage = allocateBinaryImage(inputImage->width, inputImage->height);

    for (int i = 1; i < inputImage->height - 1; i++) {
        for (int j = 1; j < inputImage->width - 1; j++) {
            resultImage->pixels[i][j] = 0;

            if (inputImage->pixels[i][j] == 1) {
                bool isEroded = false;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        if (k == 0 && l == 0) continue;

                        int neighborRow = i + k;
                        int neighborCol = j + l;

                        if (neighborRow < 0 || neighborRow >= inputImage->height ||
                            neighborCol < 0 || neighborCol >= inputImage->width) {
                            isEroded = true;
                            break;
                        }

                        if (inputImage->pixels[neighborRow][neighborCol] != 1) {
                            isEroded = true;
                            break;
                        }
                    }
                    if (isEroded) break;
                    if (!isEroded) {
                        resultImage->pixels[i][j] = 1;
                    }
                }
            }
        }
    }
    return resultImage;
}

//fonction pour effectuer la dilation sur l'image
BinaryImage *binaryDilation(BinaryImage *inputImage, int structuringElement[3][3]) {
    binarizeImage(inputImage, THRESHOLD);
    BinaryImage *resultImage = allocateBinaryImage(inputImage->width, inputImage->height);

    for (int i = 1; i < inputImage->height - 1; i++) {
        for (int j = 1; j < inputImage->width - 1; j++) {
            resultImage->pixels[i][j] = 0; // Initialiser à 0

            // Vérifier si un voisin est un objet (valeur 1)
            bool isDilated = false;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    if (k == 0 && l == 0) continue; // Ignorer le pixel central

                    int neighborRow = i + k;
                    int neighborCol = j + l;

                    if (neighborRow < 0 || neighborRow >= inputImage->height ||
                        neighborCol < 0 || neighborCol >= inputImage->width) {
                        continue; // Ignorer les pixels hors limites
                    }

                    if (inputImage->pixels[neighborRow][neighborCol] == 255) {
                        isDilated = true;
                        break ; // Sortir des boucles si un
                     }
                }
            }
            // Si un voisin est un objet, dilater le pixel
            if (isDilated) {
                resultImage->pixels[i][j] = 0;
            }
        }
    }

    return resultImage;
}

//fonction eroder
void erode(BinaryImage *image, BinaryImage *output) {
    for (int y = 1; y < image->height - 1; y++) {
        for (int x = 1; x < image->width - 1; x++) {
            if (image->pixels[y][x] == 255) {
                bool erodePixel = false;
                for (int j = -1; j <= 1 && !erodePixel; j++) {
                    for (int i = -1; i <= 1 && !erodePixel; i++) {
                        if (image->pixels[y + j][x + i] == 0) {
                            erodePixel = true;
                        }
                    }
                }
                output->pixels[y][x] = erodePixel ? 0 : 255;
            } else {
                output->pixels[y][x] = 0;
            }
        }
    }
}

//fonction de dilatation
void dilate(BinaryImage *image, BinaryImage *output) {
    for (int y = 1; y < image->height - 1; y++) {
        for (int x = 1; x < image->width - 1; x++) {
            if (image->pixels[y][x] == 255) {
                for (int j = -1; j <= 1; j++) {
                    for (int i = -1; i <= 1; i++) {
                        output->pixels[y + j][x + i] = 255;
                    }
                }
            }
        }
    }
}

//fonction pour affiner les contours de notre image
void findContours(BinaryImage *image, BinaryImage *output) {
    BinaryImage *temp = allocateBinaryImage(image->width, image->height);
    erode(image, temp);
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            output->pixels[y][x] = image->pixels[y][x] - temp->pixels[y][x];
        }
    }
    freeBinaryImage(temp);
}
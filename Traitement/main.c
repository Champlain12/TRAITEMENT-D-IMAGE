#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "Traitement.h"
//#include "Traitement.c"

int main(){
    int choice;
    PGMImage *image = readPGMImage("arbre.pgm");
    if (!image) {
        fprintf(stderr, "Erreur lors de la lecture de l'image.\n");
        return 1;
    }
    PGMImage *image1 = readPGMImage("arbre.pgm");
    if (!image1) {
        freePGMImage(image1);
        return 1;
    }

    while (1)
    {
        printMenu();
        printf("Entrez votre choix: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            int hist=1;
            int result;
            PGMImage *luni= luminance( image,2);
            // Appliquer les transformations sur l'image 1
            PGMImage *enhanced_image1 = enhanceContrast(image);
            PGMImage *linear_transformed_image1 = linearTransformation(image, 1.2, 10);
            PGMImage *linear_saturation_transformed_image1 = linearTransformationWithSaturation(image, 1.2, 10);
            PGMImage *piecewise_linear_transformed_image1 = piecewiseLinearTransformation(image, 128);
            PGMImage *nonlinear_transformed_image1 = nonlinearTransformation(image);
            PGMImage *equalized_image1 = histogramEqualization(image);
            
            //sauvegarde des image pour les operation
            savePGMImage("images1/luninamce.pgm",luni);
            savePGMImage("images1/contraste.pgm", enhanced_image1);
            savePGMImage("images1/transform_linear.pgm", linear_transformed_image1);
            savePGMImage("images1/linear_saturation.pgm", linear_saturation_transformed_image1);
            savePGMImage("images1/piecewise_linear.pgm", piecewise_linear_transformed_image1);
            savePGMImage("images1/nonlinear_transformed.pgm", nonlinear_transformed_image1);
            savePGMImage("images1/equalized_image.pgm", equalized_image1);
            
            //liberation de la memoire
            freePGMImage(luni);
            freePGMImage(enhanced_image1);
            freePGMImage(linear_transformed_image1);
            freePGMImage(linear_saturation_transformed_image1);
            freePGMImage(piecewise_linear_transformed_image1);
            freePGMImage(nonlinear_transformed_image1);
            freePGMImage(equalized_image1);
            break;
        case 2:
            // Appliquer les transformations
            PGMImage *and_image = logicalAND(image, image1);
            PGMImage *or_image = logicalOR(image, image1);
            PGMImage *added_image = addImages(image, image1);
            PGMImage *subtracted_image = subtractImages(image, image1);
            PGMImage *multiplied_image = multiplyImage(image, 1.5);
            PGMImage *nn_interpolated_image = nearestNeighborInterpolation(image, image->width * 2, image->height * 2);
            PGMImage *bilinear_interpolated_image = bilinearInterpolation(image, image->width * 2, image->height * 2);
            PGMImage *bicubic_interpolated_image = bicubicInterpolation(image, image->width * 2, image->height * 2);

            // Sauvegarder les résultats
            savePGMImage("images2/and_image.pgm", and_image);
            savePGMImage("images2/or_image.pgm", or_image);
            savePGMImage("images2/added_image.pgm", added_image);
            savePGMImage("images2/subtracted_image.pgm", subtracted_image);
            savePGMImage("images2/multiplied_image.pgm", multiplied_image);
            savePGMImage("images2/nn_interpolated_image.pgm", nn_interpolated_image);
            savePGMImage("images2/bilinear_interpolated_image.pgm", bilinear_interpolated_image);
            savePGMImage("images2/bicubic_interpolated_image.pgm", bicubic_interpolated_image);

            // Libérer la mémoire
            freePGMImage(image);
            freePGMImage(image1);
            freePGMImage(and_image);
            freePGMImage(or_image);
            freePGMImage(added_image);
            freePGMImage(subtracted_image);
            freePGMImage(multiplied_image);
            freePGMImage(nn_interpolated_image);
            freePGMImage(bilinear_interpolated_image);
            freePGMImage(bicubic_interpolated_image);
            break;
        case 3:
            applyAndSaveTransform(image, pointTransform, "images3/output_pointTransform.pgm");
            applyAndSaveTransform(image, localTransform, "images3/output_localTransform.pgm");
            applyAndSaveTransform(image, globalTransform, "images3/output_globalTransform.pgm");
            applyAndSaveTransform(image, convolution, "images3/output_convolution.pgm");
            applyAndSaveTransform(image, meanFilter, "images3/output_meanFilter.pgm");
            applyAndSaveTransform(image, gaussianFilter, "images3/output_gaussianFilter.pgm");
            applyAndSaveTransform(image, medianFilter, "images3/output_medianFilter.pgm");
            applyAndSaveTransform(image, highPassFilter, "images3/output_highPassFilter.pgm");
            applyAndSaveTransform(image, lowPassFilter, "images3/output_lowPassFilter.pgm");
            break;
        case 4:
            // Détection des contours par Sobel
            PGMImage *sobelEdges = detectEdges(image);
            writePGMImage("images4/sobel_edges.pgm", sobelEdges);
            
            // Détection des contours par Prewitt
            PGMImage *prewittEdges = prewittEdgeDetection(image);
            writePGMImage("images4/prewitt_edges.pgm", prewittEdges);
            
            // Segmentation de l'image avec un seuil global
            unsigned char threshold = 128;
            PGMImage *segmented = allocatePGMImage(image->width, image->height);
            globalThresholding(image, segmented, threshold);
            writePGMImage("images4/global_thresholded.pgm", segmented);

            // Segmentation de l'image avec un seuillage adaptatif
            int windowSize = 15;
            double C = 10.0;
            PGMImage *adaptiveSegmented = adaptiveThresholding(image, windowSize, C);
            writePGMImage("images4/adaptive_thresholded.pgm", adaptiveSegmented);

            // Segmentation de l'image par k-means
            int k = 3;
            PGMImage *kmeansSegmented = kMeansSegmentation(image, k);
            writePGMImage("images4/kmeans_segmented.pgm", kmeansSegmented);

            // Libération de la mémoire
            freePGMImage(image);
            freePGMImage(sobelEdges);
            freePGMImage(prewittEdges);
            freePGMImage(segmented);
            freePGMImage(adaptiveSegmented);
            freePGMImage(kmeansSegmented);
            break;
        case 5:
            BinaryImage *image2 = allocateBinaryImage(WIDTH, HEIGHT);
            readPGMImagebin("fem.pgm", image2);

            binarizeImage(image2, THRESHOLD);

            BinaryImage *outputImage1 = allocateBinaryImage(image2->width, image2->height);
            findContours(image2, outputImage1);
            writeBinaryImage("images5/binaire.pbm", outputImage1);

            int structuring_element[3][3]={
                {1, 1, 1},
                {1, 1, 1},
                {1, 1, 1}
            };

            BinaryImage* resulterode = binaryErosion(outputImage1, structuring_element);
            if(!resulterode){
                freeBinaryImage(image2);
                return 1;
            }
            writeBinaryImage("images5/erode.pbm", outputImage1);

            BinaryImage *resultdilate = binaryDilation(outputImage1,structuring_element);
            if (!resultdilate) {
                freeBinaryImage(image2);
                return 1;
            }
            writeBinaryImage("images5/dilate.pbm", outputImage1);

            //ouverture sur l'image
            BinaryImage *resulte=binaryErosion(outputImage1, structuring_element);
            BinaryImage *resultouvert=binaryDilation(resulte,structuring_element);
            writeBinaryImage("images5/ouverture.pbm", outputImage1);

            //fermeture de l'image
            BinaryImage *resulttt=binaryDilation(image2,structuring_element);
            BinaryImage *resultferme=binaryErosion(resulttt, structuring_element);
            writeBinaryImage("images5/fermeture.pbm", outputImage1);

            freeBinaryImage(image2);
            freeBinaryImage(outputImage1);
            break;
        
        case 0:
            printf("Exiting...\n");
            freePGMImage(image);
            exit(0);
            break;
        default:
            printf("Choix invalide!\n");
            break;
        }
    }

    return 0;
}
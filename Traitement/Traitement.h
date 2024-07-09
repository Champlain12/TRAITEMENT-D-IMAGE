#ifndef TRAITEMENT
#define TRAITEMENT
#define MAX_GRAY_LEVEL 256
#define MAX_HIST_HEIGHT 50
#define WIDTH 256
#define HEIGHT 256
#define THRESHOLD 127

// Structure pour stocker les données d'image PGM
typedef struct {
    int width;
    int height;
    int max_gray_value;
    int min_gray_value;
    int max_value;
    int **pixels;
} PGMImage;

typedef struct {
    int width;
    int height;
    unsigned char **pixels;
} BinaryImage;

PGMImage *allocatePGMImage(int width, int height);
void freePGMImage(PGMImage *image);
PGMImage *readPGMImage(const char *filename);
PGMImage *enhanceContrast(const PGMImage *image);
PGMImage *linearTransformation(const PGMImage *image, float slope, float intercept);
PGMImage *luminance(const PGMImage *image, double scale_factor);
PGMImage *piecewiseLinearTransformation(const PGMImage *image, int threshold);
PGMImage *nonlinearTransformation(const PGMImage *image);
PGMImage *linearTransformationWithSaturation(const PGMImage *image, float slope, float intercept);
PGMImage *histogramEqualization(const PGMImage *image);
void savePGMImage(const char *filename, const PGMImage *image);
/*
    image 2 commence
*/
int sameDimensions(const PGMImage *image1, const PGMImage *image2);
PGMImage *logicalAND(const PGMImage *image1, const PGMImage *image2);
PGMImage *logicalOR(const PGMImage *image1, const PGMImage *image2);
PGMImage *addImages(const PGMImage *image1, const PGMImage *image2);
PGMImage *subtractImages(const PGMImage *image1, const PGMImage *image2);
PGMImage *multiplyImage(const PGMImage *image, float factor);
PGMImage *nearestNeighborInterpolation(const PGMImage *image, int new_width, int new_height);
PGMImage *bilinearInterpolation(const PGMImage *image, int new_width, int new_height);
PGMImage *bicubicInterpolation(const PGMImage *image, int new_width, int new_height);

/*
    image3 commence
*/
void printPGMImage(PGMImage *image);
void pointTransform(PGMImage *image);
void localTransform(PGMImage *image);
void globalTransform(PGMImage *image);
void convolution(PGMImage *image, int kernel[3][3]);
void meanFilter(PGMImage *image);
void gaussianFilter(PGMImage *image);
void medianFilter(PGMImage *image);
void highPassFilter(PGMImage *image);
void lowPassFilter(PGMImage *image);
void applyAndSaveTransform(PGMImage *image, void (*transform)(PGMImage *), const char *outputFilename);
void printMenu();
/*
    image 4 commence
*/
PGMImage *detectEdges(const PGMImage *image);
PGMImage *segmentImage(const PGMImage *image, unsigned char threshold);
void globalThresholding(PGMImage *input, PGMImage *output, unsigned char threshold);
PGMImage *prewittEdgeDetection(const PGMImage *image);
PGMImage *adaptiveThresholding(const PGMImage *image, int windowSize, double C);
PGMImage *kMeansSegmentation(const PGMImage *image, int k);
void writePGMImage(const char *filename, const PGMImage *image);

/*
    image5 commence;
*/

BinaryImage* allocateBinaryImage(int width, int height);
void freeBinaryImage(BinaryImage *image);
void readPGMImagebin(const char *filename, BinaryImage *image);
void writeBinaryImage(const char *filename, BinaryImage *image);
void binarizeImage(BinaryImage *image, unsigned char threshold);
bool isConnected4(BinaryImage *image, int x1, int y1, int x2, int y2);
bool isConnected8(BinaryImage *image, int x1, int y1, int x2, int y2);
int manhattanDistance(int x1, int y1, int x2, int y2);
int chessboardDistance(int x1, int y1, int x2, int y2);
void labelConnectedComponents(BinaryImage *image);
BinaryImage *binaryErosion(BinaryImage *inputImage, int structuringElement[3][3]);
BinaryImage *binaryDilation(BinaryImage *inputImage, int structuringElement[3][3]);
void erode(BinaryImage *image, BinaryImage *output);
void dilate(BinaryImage *image, BinaryImage *output);
void findContours(BinaryImage *image, BinaryImage *output);
#endif
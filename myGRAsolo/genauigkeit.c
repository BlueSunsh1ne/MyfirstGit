#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <math.h>
#include <nmmintrin.h>

typedef struct {
     int width, height;
     uint8_t *data;
} PGMImage;

#define RGB_COMPONENT_COLOR 255

static PGMImage* readPGM(const char *filename)
{
         char buff[16];
         PGMImage *img;
         FILE *fp;
         int c, rgb_comp_color;

         //open PPM file for reading
         fp = fopen(filename, "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }

         //read image format
         if (!fgets(buff, sizeof(buff), fp)) {
              perror(filename);
              exit(1);
         }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '5') {
         fprintf(stderr, "Invalid image format (must be 'P5')\n");
         exit(1);
    }

    //alloc memory from image
    img = (PGMImage *) malloc(sizeof(PGMImage));
    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
         c = getc(fp);
    }
    ungetc(c, fp);


    //read image size information
    if (fscanf(fp, "%d %d", &img->width, &img->height) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n') ;

    //memory allocation for pixel data
    img->data = (uint8_t*) malloc(img->width * img->height);

    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

     
    //read pixel data from file
    if (fread(img->data, 1, img->width * img->height, fp) != img->width * img->height) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         exit(1);
    }

    fclose(fp);
    return img;
}

void cmpPGMfiles(PGMImage* image1, PGMImage* image2){
    int width = image1->width;
    int height = image1->height;
    int numberPixels = height*width;

    uint8_t* pixelptr1 = image1->data;
    uint8_t* pixelptr2 = image2->data;

    int totaldelta =0;

    for(int i=0;i<100;i++){
        int tmp;
        tmp = pixelptr1[i]-pixelptr2[i];
        if(tmp<0){tmp = -tmp;}
        if(tmp !=0){
        printf("At pixel Number %i the first image has value %i and the second image has value %i. The differnce is %i \n",i,pixelptr1[i], pixelptr2[i], tmp);
        totaldelta += tmp;
        }
    }
    printf("The total difference is: %i \n", totaldelta);

}


void main(){

    PGMImage* image1;
    char* inputfile1 = "outputfile1.pgm";
    image1 = readPGM(inputfile1);

    uint8_t* pixelptr1 = image1->data;

    // for(int i=0;i<50;i++){
    //      printf("The %i.Pixel has value %i \n", i, pixelptr1[i]);
   //  }

    PGMImage* image2;
    char* inputfile2 = "netpbmOutput.pgm";
    image2 = readPGM(inputfile2);



   cmpPGMfiles(image1, image2);
}
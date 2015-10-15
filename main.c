#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct Pix
{
  unsigned char R;
  unsigned char G;
  unsigned char B;
  //unsigned char L;
  //int BW;
}Pix;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BitMap
{
    short Signature;
    long Reserved1;
    long Reserved2;
    long DataOffSet;

    long Size;
    long Width;
    long Height;
    short Planes;
    short BitsPerPixel;
    long Compression;
    long SizeImage;
    long XPixelsPreMeter;
    long YPixelsPreMeter;
    long ColorsUsed;
    long ColorsImportant;
    struct Pix *pixels
}BitMap;
#pragma pack(pop)

void threshold(unsigned char *r,unsigned char *g, unsigned char *b){

    int luminosity = (int)(0.2126 * *r + 0.7152 * *g + 0.0722 * *b);
    if (luminosity > 200){
        *r = 255;
        *g = 255;
        *b = 255;
    }else {
        *r = 0;
        *g = 0;
        *b = 0;
    }

}

void grayscale(unsigned char *r,unsigned char *g, unsigned char *b){

    int luminosity = (int)(0.2126 * *r + 0.7152 * *g + 0.0722 * *b);

    *r = luminosity;
    *g = luminosity;
    *b = luminosity;


}

void sepia(unsigned char *r,unsigned char *g, unsigned char *b){

    unsigned char rOut = 0;
    unsigned char gOut = 0;
    unsigned char bOut = 0;

    rOut = (int)((*r * 0.393) + (*g * 0.769) + (*b * 0.189));
    gOut = (int)((*r * 0.349) + (*g * 0.686) + (*b * 0.168));
    bOut = (int)((*r * 0.272) + (*g * 0.534) + (*b * 0.131));

    if (rOut > 255){
        *r = 255;
    } else{
        *r = rOut;
    }

    if (gOut > 255){
        *g = 255;
    } else{
        *g = gOut;
    }

    if (bOut > 255){
        *b = 255;
    } else{
        *b = bOut;
    }


}

void borderDetector(Pix *pixel, BitMap *pBmp){

    int rOut = 0;
    int gOut = 0;
    int bOut = 0;

    int i = 0;
    int j = 0;

    int luminosity = 0;
    int luminosity2 = 0;
/*
    for(i = 1; i < pBmp->Height; ++i){
        for(j = i; j < pBmp->Width; ++j){
            luminosity = (int)(0.2126 * *pBmp.pixels[i-1, j-1]->R + 0.7152 * *pBmp.pixels[i-1, j-1]->G + 0.0722 * *pBmp.pixels[i-1, j-1]->B);
            luminosity2 = (int)(0.2126 * *pBmp->pixels[i, j]->R + 0.7152 * *pBmp->pixels[i, j]->G + 0.0722 * *pBmp->pixels[i, j]->B);

            if (luminosity - luminosity2 > 30){
                rOut = 255;
                gOut = 255;
                bOut = 255;
            } else {
                rOut = 0;
                gOut = 0;
                bOut = 0;
            }
        }
    }
*/
}



int main(int argc, char **argv){

    unsigned long int i=0;
    unsigned long int S=0;

    int filterChoice = 0;

    struct BitMap source_info;
    struct Pix source_pix;

    FILE *fp;
    FILE *Dfp;

    printf("Choose filter: \n1: Threshold\n2: Grayscale\n3: Sepia\n4: BorderDetector\n");
    scanf("%d", &filterChoice);


    if(!(fp=fopen("duck.bmp","rb"))){
        printf(" can not open file");
        exit(-1);
    }


    Dfp=fopen("out.bmp","wb");

    fread(&source_info, sizeof(BitMap),1,fp);

    S=source_info.Width*source_info.Height;
    Pix* pixels =  (struct Pix *) malloc(sizeof(struct Pix)*S);


    for(i=1;i<=S;i++){
        fread(&source_pix,sizeof(struct Pix),1,fp);
        pixels[i-1] = source_pix;
    }

    fwrite(&source_info, sizeof(BitMap),1,Dfp);


    if (filterChoice == 1){
        for(i=1;i<=S;i++){
                threshold(&pixels[i-1].R, &pixels[i-1].G, &pixels[i-1].B);
                fwrite(&pixels[i-1],sizeof(struct Pix),1,Dfp);
            }
    }else if(filterChoice == 2){
            for(i=1;i<=S;i++){
                grayscale(&pixels[i-1].R, &pixels[i-1].G, &pixels[i-1].B);
                fwrite(&pixels[i-1],sizeof(struct Pix),1,Dfp);
            }
        }else if(filterChoice == 3){
                for(i=1;i<=S;i++){
                    sepia(&pixels[i-1].R, &pixels[i-1].G, &pixels[i-1].B);
                    fwrite(&pixels[i-1],sizeof(struct Pix),1,Dfp);
                }
            }else{
                for(i=1;i<=S;i++){
                    fwrite(&pixels[i-1],sizeof(struct Pix),1,Dfp);
                }
            }



    fclose(fp);
    fclose(Dfp);
    return 0;
}



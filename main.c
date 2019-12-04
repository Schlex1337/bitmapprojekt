#include <stdio.h>
#include <stdlib.h>

#pragma pack(2)
typedef struct
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned int bfReserverd;
    unsigned int bfOffBits;
}bmpheader;

#pragma pack(2)
typedef struct
{
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizelmange;
    unsigned int biXPelsPerMeter;
    unsigned int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
}bmpinfo;

typedef struct
{
    unsigned char cB;
    unsigned char cG;
    unsigned char cR;
} bmpcolor;

void printheader(bmpheader *tbmpfh)
{
    printf("Type: %d \n", tbmpfh->bfType);
    printf("Size:%d \n", tbmpfh->bfSize);
    printf("Reserved:%d \n", tbmpfh->bfReserverd);
    printf("OffBits: %d \n", tbmpfh->bfOffBits);
}

void printinfo(bmpinfo *tbmpif)
{
    printf("Size: %d \n", tbmpif->biSize);
    printf("Width: %d \n", tbmpif->biWidth);
    printf("Height:%d \n", tbmpif->biHeight);
    printf("Planes: %d \n", tbmpif->biPlanes);
    printf("BitCount: %d \n", tbmpif->biBitCount);
    printf("Compression: %d \n", tbmpif->biCompression);
    printf("SizeImage: %d \n", tbmpif->biSizelmange);
    printf("XPelsPerMeter: %d \n", tbmpif->biXPelsPerMeter);
    printf("YPelsPerMeter: %d \n", tbmpif->biYPelsPerMeter);
    printf("Clr Used:%d \n", tbmpif->biClrUsed);
    printf("Clr IMPORTANT: %d \n", tbmpif->biClrImportant);
}



int main(int argc, char *argv[])
{
    int iX , iY, iL;
    bmpheader tbmpfh;
    bmpinfo tbmpif;
    bmpcolor **tcolortab;

    FILE *fBmpdatei = fopen(argv[1],"rb");
    FILE *fuellerneu = fopen("fuellerneu.bmp","wb");

    fread(&tbmpfh,sizeof(bmpheader),1,fBmpdatei);
    fread(&tbmpif,sizeof(bmpinfo),1,fBmpdatei);

    tcolortab = (bmpcolor**)malloc(tbmpif.biHeight*sizeof(bmpcolor*));

    for(iL=0; iL<tbmpif.biHeight;iL++)
    {
        tcolortab[iL] = (bmpcolor*)malloc(tbmpif.biWidth*sizeof(bmpcolor));
    }

    for(iX=0 ; iX<tbmpif.biHeight; iX++)
    {
        for(iY=0; iY<tbmpif.biWidth; iY++)
        {
            fread(&tcolortab[iX][iY],sizeof(bmpcolor),1,fBmpdatei);
        }
        fseek(fBmpdatei,tbmpif.biWidth%4,SEEK_CUR);
    }
    fwrite(&tbmpfh,sizeof(bmpheader),1,fuellerneu);
    fwrite(&tbmpif,sizeof(bmpinfo),1,fuellerneu);


    for(iX=0 ; iX<tbmpif.biHeight; iX++)
    {
       for(iY=0; iY<tbmpif.biWidth; iY++)
        {
            fwrite(&tcolortab[iX][iY],sizeof(bmpcolor),1,fuellerneu);
        }


        if(4-tbmpif.biWidth%4 != 0)
        {
            for(iL=0; iL<4-tbmpif.biWidth%4; iL++)
            {
                fputc(0,fuellerneu);
            }

        }


    }

    printheader(&tbmpfh);
    printf("\n");
    printinfo(&tbmpif);


    fclose(fuellerneu);
    fclose(argv[1]);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(2)
typedef struct
{
    unsigned short biType;
    unsigned int biSize;
    unsigned int biReserverd;
    unsigned int biOffBits;
}bmpheader;

#pragma pack(2)
typedef struct // Bitmapinfoheader
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

typedef struct // Farbtabelle
{
    unsigned char cB;
    unsigned char cG;
    unsigned char cR;
} bmpcolor;

void printheadinfo(bmpheader *bhead,bmpinfo *binfo) // Bitmapinfoheader auf die Konsole ausprinten
{
    printf("Type: %d \n", bhead->biType);
    printf("Size:%d \n", bhead->biSize);
    printf("Reserved:%d \n", bhead->biReserverd);
    printf("OffBits: %d \n", bhead->biOffBits);
    printf("\n");
    printf("Size: %d \n", binfo->biSize);
    printf("Width: %d \n", binfo->biWidth);
    printf("Height:%d \n", binfo->biHeight);
    printf("Planes: %d \n", binfo->biPlanes);
    printf("BitCount: %d \n", binfo->biBitCount);
    printf("Compression: %d \n", binfo->biCompression);
    printf("SizeImage: %d \n", binfo->biSizelmange);
    printf("XPelsPerMeter: %d \n", binfo->biXPelsPerMeter);
    printf("YPelsPerMeter: %d \n", binfo->biYPelsPerMeter);
    printf("Clr Used:%d \n", binfo->biClrUsed);
    printf("Clr IMPORTANT: %d \n", binfo->biClrImportant);
}

void einlesen(FILE *fBmpdatei,bmpheader *bhead,bmpinfo *binfo)
{
    fread(bhead,sizeof(bmpheader),1,fBmpdatei); // Bitmapheader einlesen in die Struktur fBmpdatei
    fread(binfo,sizeof(bmpinfo),1,fBmpdatei); // Bitmapinfoheader einlesen in die Struktur fBmpdatei
}

void auslesen(FILE *neu,bmpheader *bhead,bmpinfo *binfo)
{
    fwrite(bhead,sizeof(bmpheader),1,neu); // Bitmapheader in die neue Datei reinschreiben
    fwrite(binfo,sizeof(bmpinfo),1,neu); // Bitmapinfoheader in die neue Datei reinschreiben
}

void greyscale(bmpcolor **tcolortab,int height, int width)
{
    int iP;
    for(int iX=0; iX<height; iX++) // Graustufe
    {
       for(int iY=0; iY<width; iY++)
        {
            iP = tcolortab[iX][iY].cR*0.299+tcolortab[iX][iY].cG*0.587+tcolortab[iX][iY].cB*0.114;
            tcolortab[iX][iY].cB = iP;
            tcolortab[iX][iY].cG = iP;
            tcolortab[iX][iY].cR = iP;
        }
    }
}

void tabelleneu(FILE *neu, int height, int width, bmpcolor **tcolortab)
{
    for(int iX=0; iX<height; iX++) // Farbtabelle in die neue Datei reinschreiben
        {
           for(int iY=0; iY<width; iY++)
            {
                fwrite(&tcolortab[iX][iY],sizeof(bmpcolor),1,neu);
            }

            if(width%4 != 0)
            {
                for(int iL=0; iL<4-width%4; iL++)
                {
                    fputc(0,neu);
                }
            }
        }
}
void tabellealt(FILE *neu, int height, int width, bmpcolor **tcolortab, FILE *fBmpdatei)
{
     for(int iX=0 ; iX<height; iX++) // Farbtabelle einlesen
        {
            for(int iY=0; iY<width; iY++)
            {
                fread(&tcolortab[iX][iY],sizeof(bmpcolor),1,fBmpdatei);
            }
            fseek(fBmpdatei,width%4,SEEK_CUR);
        }
}

/*
void speicher(int height, int width, bmpcolor ***tcolortab)
{

    tcolortab = (bmpcolor**)malloc(height*sizeof(bmpcolor*)); // Speicher reservieren f�r die erste Dimension
    for(int iL=0; iL<height;iL++) // Speicher reservieren f�r die zweite Dimension
    {
        tcolortab[iL] = (bmpcolor*)malloc(width*sizeof(bmpcolor));
    }
}*/


int main(int argc, char *argv[])
{
    int iX , iY, iL, iLZ;

    char neustring[] = "neu.bmp";
    char fakestring[20];
    char cseek[] = ".";

    bmpheader bhead;
    bmpinfo binfo;
    bmpcolor **tcolortab;
    FILE *fBmpdatei;
    FILE *neu;


    for(iLZ=1; iLZ<=3; iLZ++)
    {


        printf("::::::::::::::::::::::::%d::::::::::::::::",iLZ);
        fBmpdatei = fopen(argv[iLZ],"rb"); // Alte Datei �ffnen

        printf("%s \n",argv[iLZ]);
        strcpy(fakestring,argv[iLZ]);
        strtok(fakestring,cseek); // Zeichen suchen und ab dem Zeichen zerteilen
        strcat(fakestring,neustring); // Zeichen ersetzen
        printf("%s \n",fakestring);

        neu = fopen(fakestring,"wb"); // neue Datei erstellen

        if(fBmpdatei == NULL) // Ge�ffnete Datei vorhanden?
        {
            printf("Keine Datei wurde geoeffnet.");
            return 0;
        }

        einlesen(fBmpdatei,&bhead,&binfo);

        tcolortab = (bmpcolor**)malloc(binfo.biHeight*sizeof(bmpcolor*)); // Speicher reservieren f�r die erste Dimension

        for(iL=0; iL<binfo.biHeight;iL++) // Speicher reservieren f�r die zweite Dimension
        {
            tcolortab[iL] = (bmpcolor*)malloc(binfo.biWidth*sizeof(bmpcolor));
        }

        //speicher(binfo.biHeight,binfo.biWidth,tcolortab);

        tabellealt(neu,binfo.biHeight, binfo.biWidth, tcolortab, fBmpdatei);

        greyscale(tcolortab,binfo.biHeight,binfo.biWidth);

        auslesen(neu,&bhead,&binfo);

        tabelleneu(neu,binfo.biHeight, binfo.biWidth, tcolortab);


        printheadinfo(&bhead,&binfo); //Bitmapheader und Bitmapinfo auf der Konsole anzeigen lassen

        fclose(neu); // Neue Datei schlie�en
        fclose(fBmpdatei); // Alte Datei schlie�en

        free(tcolortab);
        printf("---------------\n");
    }
    return 0;
}

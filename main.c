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

void printheader(bmpheader *bhead) // Bitmapheader auf die Konsole ausprinten
{
    printf("Type: %d \n", bhead->bfType);
    printf("Size:%d \n", bhead->bfSize);
    printf("Reserved:%d \n", bhead->bfReserverd);
    printf("OffBits: %d \n", bhead->bfOffBits);
}

void printinfo(bmpinfo *binfo) // Bitmapinfoheader auf die Konsole ausprinten
{
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
/*
void speicher(bmpheader *bhead,bmpinfo *binfo, bmpcolor *tcolor)
{
    fread(&bhead,sizeof(bmpheader),1,fBmpdatei); // Bitmapheader einlesen in die Struktur fBmpdatei
    fread(&binfo,sizeof(bmpinfo),1,fBmpdatei); // Bitmapinfoheader einlesen in die Struktur fBmpdatei

    tcolortab = (bmpcolor**)malloc(binfo.biHeight*sizeof(bmpcolor*)); // Speicher reservieren für die erste Dimension
    for(iL=0; iL<binfo.biHeight;iL++) // Speicher reservieren für die zweite Dimension
    {
        tcolortab[iL] = (bmpcolor*)malloc(binfo.biWidth*sizeof(bmpcolor));
    }
}*/



int main(int argc, char *argv[])
{
    int iX , iY, iL, iP;
    bmpheader bhead;
    bmpinfo binfo;
    bmpcolor **tcolortab;

    FILE *fBmpdatei = fopen("naegel.bmp","rb");
    FILE *neu = fopen("naegelneu.bmp","wb"); // neue Datei erstellen

    if(fBmpdatei == NULL) // Geöffnete Datei vorhanden?
    {
        printf("Keine Datei wurde geoeffnet.");
        return 0;
    }


    fread(&bhead,sizeof(bmpheader),1,fBmpdatei); // Bitmapheader einlesen in die Struktur fBmpdatei
    fread(&binfo,sizeof(bmpinfo),1,fBmpdatei); // Bitmapinfoheader einlesen in die Struktur fBmpdatei

    tcolortab = (bmpcolor**)malloc(binfo.biHeight*sizeof(bmpcolor*)); // Speicher reservieren für die erste Dimension
    for(iL=0; iL<binfo.biHeight;iL++) // Speicher reservieren für die zweite Dimension
    {
        tcolortab[iL] = (bmpcolor*)malloc(binfo.biWidth*sizeof(bmpcolor));
    }

    for(iX=0 ; iX<binfo.biHeight; iX++) // Farbtabelle einlesen
    {
        for(iY=0; iY<binfo.biWidth; iY++)
        {
            fread(&tcolortab[iX][iY],sizeof(bmpcolor),1,fBmpdatei);
        }
        fseek(fBmpdatei,binfo.biWidth%4,SEEK_CUR);
    }

    fwrite(&bhead,sizeof(bmpheader),1,neu); // Bitmapheader in die neue Datei reinschreiben
    fwrite(&binfo,sizeof(bmpinfo),1,neu); // Bitmapinfoheader in die neue Datei reinschreiben

    for(iX=0; iX<binfo.biHeight; iX++) // Graustufe
    {
       for(iY=0; iY<binfo.biWidth; iY++)
        {
            iP = tcolortab[iX][iY].cR*0.299+tcolortab[iX][iY].cG*0.587+tcolortab[iX][iY].cB*0.114;
            tcolortab[iX][iY].cB = iP;
            tcolortab[iX][iY].cG = iP;
            tcolortab[iX][iY].cR = iP;
        }
    }

    for(iX=0; iX<binfo.biHeight; iX++) // Farbtabelle in die neue Datei reinschreiben
    {
       for(iY=0; iY<binfo.biWidth; iY++)
        {
            fwrite(&tcolortab[iX][iY],sizeof(bmpcolor),1,neu);
        }

        if(binfo.biWidth%4 != 0)
        {
            for(iL=0; iL<4-binfo.biWidth%4; iL++)
            {
                fputc(0,neu);
            }
        }
    }

    printheader(&bhead); //Bitmapheader auf der Konsole anzeigen lassen
    printf("\n");
    printinfo(&binfo); // Bitmapinfoheader auf der Konsole anzeigen lassen

    fclose(neu); // Neue Datei schließen
    fclose(fBmpdatei); // Alte Datei schließen
    return 0;
}

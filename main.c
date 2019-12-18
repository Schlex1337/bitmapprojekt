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

void printheader(bmpheader *tbmpfh) // Bitmapheader auf die Konsole ausprinten
{
    printf("Type: %d \n", tbmpfh->bfType);
    printf("Size:%d \n", tbmpfh->bfSize);
    printf("Reserved:%d \n", tbmpfh->bfReserverd);
    printf("OffBits: %d \n", tbmpfh->bfOffBits);
}

void printinfo(bmpinfo *tbmpif) // Bitmapinfoheader auf die Konsole ausprinten
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
    int iX , iY, iL, iP;
    bmpheader tbmpfh;
    bmpinfo tbmpif;
    bmpcolor **tcolortab;

    FILE *fBmpdatei = fopen("naegel.bmp","rb");
    FILE *neu = fopen("nneu.bmp","wb"); // neue Datei erstellen

    if(fBmpdatei == NULL) // Geöffnete Datei vorhanden?
    {
        printf("Keine Datei wurde geoeffnet.");
        return 0;
    }

    fread(&tbmpfh,sizeof(bmpheader),1,fBmpdatei); // Bitmapheader einlesen in die Struktur fBmpdatei
    fread(&tbmpif,sizeof(bmpinfo),1,fBmpdatei); // Bitmapinfoheader einlesen in die Struktur fBmpdatei

    tcolortab = (bmpcolor**)malloc(tbmpif.biHeight*sizeof(bmpcolor*)); // Speicher reservieren für die erste Dimension
    for(iL=0; iL<tbmpif.biHeight;iL++) // Speicher reservieren für die zweite Dimension
    {
        tcolortab[iL] = (bmpcolor*)malloc(tbmpif.biWidth*sizeof(bmpcolor));
    }

    for(iX=0 ; iX<tbmpif.biHeight; iX++) // Farbtabelle einlesen
    {
        for(iY=0; iY<tbmpif.biWidth; iY++)
        {
            fread(&tcolortab[iX][iY],sizeof(bmpcolor),1,fBmpdatei);
        }
        fseek(fBmpdatei,tbmpif.biWidth%4,SEEK_CUR);
    }

    fwrite(&tbmpfh,sizeof(bmpheader),1,neu); // Bitmapheader in die neue Datei reinschreiben
    fwrite(&tbmpif,sizeof(bmpinfo),1,neu); // Bitmapinfoheader in die neue Datei reinschreiben

    for(iX=0; iX<tbmpif.biHeight; iX++) // Graustufe
    {
       for(iY=0; iY<tbmpif.biWidth; iY++)
        {
            iP = tcolortab[iX][iY].cR*0.299+tcolortab[iX][iY].cG*0.587+tcolortab[iX][iY].cB*0.114;
            tcolortab[iX][iY].cB = iP;
            tcolortab[iX][iY].cG = iP;
            tcolortab[iX][iY].cR = iP;
        }
    }

    for(iX=0; iX<tbmpif.biHeight; iX++) // Farbtabelle in die neue Datei reinschreiben
    {
       for(iY=0; iY<tbmpif.biWidth; iY++)
        {
            fwrite(&tcolortab[iX][iY],sizeof(bmpcolor),1,neu);
        }

        if(4-tbmpif.biWidth%4 != 0)
        {
            for(iL=0; iL<4-tbmpif.biWidth%4; iL++)
            {
                fputc(0,neu);
            }
        }
    }

    printheader(&tbmpfh); //Bitmapheader auf der Konsole anzeigen lassen
    printf("\n");
    printinfo(&tbmpif); // Bitmapinfoheader auf der Konsole anzeigen lassen

    fclose(neu); // Neue Datei schließen
    fclose(fBmpdatei); // Alte Datei schließen
    return 0;
}

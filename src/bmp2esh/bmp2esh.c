#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct //  BMP HEADERS
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct // ESH HEADERS
{
    int16_t eshType;
    int32_t eshHeight;
    int32_t eshWidth;
    int32_t eshSize;
    int32_t eshOffBits;
} ESHHEADER;
#pragma pack(pop)

int Bmp_to_Esh(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    Bmp_to_Esh(argc, argv);

    return 0;
}

int Bmp_to_Esh(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage : %s <sorce.bmp> <destination.esh>\n", argv[0]);
        return 1;
    }

    FILE *Bmp_fp = fopen(argv[1], "rb");
    if (Bmp_fp == NULL)
    {
        printf("Error: Cannot open the Source file\n");
        fclose(Bmp_fp);
        return 1;
    }

    FILE *Esh_fp = fopen(argv[2], "wb");
    if (Esh_fp == NULL)
    {
        printf("Error: Cannot open the Destination file\n");
        fclose(Esh_fp);
        return 1;
    }

    BITMAPFILEHEADER bit_map_file_header;
    BITMAPINFOHEADER bit_map_info_header;

    ESHHEADER esh_header;

    if (fread(&bit_map_file_header, sizeof(BITMAPFILEHEADER), 1, Bmp_fp) != 1 ||
        fread(&bit_map_info_header, sizeof(BITMAPINFOHEADER), 1, Bmp_fp) != 1)
    {
        printf("Error reading BMP headers\n");
        fclose(Bmp_fp);
        fclose(Esh_fp);
        return 1;
    }

    if (bit_map_file_header.bfType != 0x4D42)
    {
        printf("This is not a valid BMP file\n");
        fclose(Bmp_fp);
        fclose(Esh_fp);
        return 1;
    }

    if (bit_map_info_header.biBitCount != 24 ||
        bit_map_info_header.biCompression != 0 ||
        bit_map_info_header.biHeight < 0)
    {
        printf("Cannot convert this file into a .Esh file\n");
        fclose(Bmp_fp);
        fclose(Esh_fp);
        return 1;
    }

    int32_t Bmp_height = bit_map_info_header.biHeight;
    int32_t Bmp_width = bit_map_info_header.biWidth;
    int row_padded = (Bmp_width * 3 + 3) & (~3);

    unsigned char *Bmp_pixel_data = (unsigned char *)malloc(Bmp_height * row_padded);
    if (!Bmp_pixel_data)
    {
        printf("Memory allocation failed\n");
        fclose(Bmp_fp);
        fclose(Esh_fp);
        return 1;
    }

    esh_header.eshType = 0x4548;
    esh_header.eshHeight = Bmp_height;
    esh_header.eshWidth = Bmp_width;
    esh_header.eshSize = sizeof(ESHHEADER) + esh_header.eshHeight * esh_header.eshWidth * 4;
    esh_header.eshOffBits = sizeof(ESHHEADER);

    fwrite(&esh_header, sizeof(esh_header), 1, Esh_fp);

    fseek(Bmp_fp, bit_map_file_header.bfOffBits, SEEK_SET);
    fread(Bmp_pixel_data, row_padded, Bmp_height, Bmp_fp);

    for (int i = 1; i <= Bmp_height; i++)
    {
        unsigned char *src = &(Bmp_pixel_data[(Bmp_height * row_padded) - (row_padded * i)]);
        for (int j = 0; j < Bmp_width; j++)
        {
            unsigned char blue = src[j * 3 + 0];
            unsigned char green = src[j * 3 + 1];
            unsigned char red = src[j * 3 + 2];

            int hexColor = (red << 16) | (green << 8) | blue;

            fwrite(&hexColor, sizeof(int), 1, Esh_fp);
        }
    }

    free(Bmp_pixel_data);
    fclose(Bmp_fp);
    fclose(Esh_fp);
    printf("BMP to ESH convertion is Done\n");

    return 0;
}

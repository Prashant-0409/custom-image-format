#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#pragma pack(push, 1)
typedef struct
{
    int16_t eshType;
    int32_t eshHeight;
    int32_t eshWidth;
    int32_t eshSize;
    int32_t eshOffBits;
} ESHHEADER;
#pragma pack(pop)

void draw_image(int *pixels, int width, int height, SDL_Surface *surface);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <image.esh>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp)
    {
        perror("Error opening file");
        return 1;
    }

    ESHHEADER header;
    if (fread(&header, sizeof(ESHHEADER), 1, fp) != 1)
    {
        printf("Error reading ESH header!\n");
        fclose(fp);
        return 1;
    }

    if (header.eshType != 0x4548) // 'HE'
    {
        printf("Not a valid ESH file!\n");
        fclose(fp);
        return 1;
    }

    int width = header.eshWidth;
    int height = header.eshHeight;

    int *pixels = (int *)malloc(width * height * sizeof(int));
    if (!pixels)
    {
        printf("Memory allocation failed!\n");
        fclose(fp);
        return 1;
    }

    fseek(fp, header.eshOffBits, SEEK_SET);
    fread(pixels, sizeof(int), width * height, fp);
    fclose(fp);

    // SDL Init
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Error: %s\n", SDL_GetError());
        free(pixels);
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("ESH Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    // Draw
    draw_image(pixels, width, height, surface);
    SDL_UpdateWindowSurface(window);

    // Event loop
    SDL_Event event;
    SDL_bool running = SDL_TRUE;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    free(pixels);

    return 0;
}

void draw_image(int *pixels, int width, int height, SDL_Surface *surface)
{
    SDL_Rect rect;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int colorInt = pixels[y * width + x];

            unsigned char r = (colorInt >> 16) & 0xFF;
            unsigned char g = (colorInt >> 8) & 0xFF;
            unsigned char b = (colorInt) & 0xFF;

            Uint32 color = SDL_MapRGB(surface->format, r, g, b);

            rect.x = x;
            rect.y = y; 
            rect.w = 1;
            rect.h = 1;

            SDL_FillRect(surface, &rect, color);
        }
    }
}

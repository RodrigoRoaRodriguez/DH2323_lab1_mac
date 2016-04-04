// Introduction lab that covers:
// * C++
// * SDL
// * 2D graphics
// * Plotting pixels
// * Video memory
// * Color representation
// * Linear interpolation
// * glm::vec3 and std::vector
// Mac version assembled from different sources by Rodrigo Roa Rodríguez

#include <iostream>
#include <vector>
#include "glm/glm/glm.hpp"
#include <SDL2/SDL.h>

using namespace std;
using glm::vec3;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Surface* screen;

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void PutPixel(SDL_Surface *, int, int, vec3);
void PutPixel(SDL_Surface *, int, int, vec3);
Uint32 vecToUint32(vec3);
Uint8 exercise;
SDL_Event e;

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
	int posX = 100, posY = 100; //Where you want the left top corner of the window to be
    Uint32 rmask, gmask, bmask, amask;

	window = SDL_CreateWindow("DH2323 Lab1", posX, posY, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the color to which the renderer clears (white in this case)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);

    if(screen == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }

//	SDL_Init(SDL_INIT_VIDEO);
	while( 1 )
	{
        // Check any events
        if (SDL_PollEvent(&e)) {
            // If either the windows is closed or ESC is pressed end the program.
            if (e.type == SDL_QUIT) {
                break;
            }
            if( e.type == SDL_KEYDOWN ){
                if( e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
                //Press 1, 2, or 3 to display the different exercises.
                switch(e.key.keysym.sym ){
                    case SDLK_1:
                        exercise = 1;
                        break;
                    case SDLK_2:
                        exercise = 2;
                        break;
                    case SDLK_3:
                        exercise = 3;
                        break;
                }
            }
        }

        //Draw modifies our surface
        Draw();
        //The surface is turned into a texture which is then rendered
        texture = SDL_CreateTextureFromSurface(renderer, screen);// Convert to texture so we can render.
        if (texture == NULL) {
            fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
            break;
        }

        SDL_RenderClear(renderer); // Clear the screen
        SDL_RenderCopy(renderer, texture, NULL, NULL); //Copy texture created from the surface to buffer
        SDL_RenderPresent(renderer); // Render buffer
	}

//    SDL_SaveBMP( screen, "screenshot.bmp" ); // Uncomment to start taking screenshots of your work

    // Cleanup in order not to leak memory.
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}

// Sets the colors for a single pixel on our surface at position (x, y).
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    // p is the address to the pixel we want to set,
    // pitch is the length of a surface scanline in bytes and
    // 4 is the number of bytes per pixel.
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)p = pixel;
}

// Converts the vector to 4 bytes (Uint32) and then calls putPixel
void PutPixel(SDL_Surface *surface, int x, int y, vec3 pixel)
{
    PutPixel(screen, x, y, vecToUint32(pixel));
}

//Converts the vectors into int32

Uint32 vecToUint32(vec3 color){
    Uint8 r = Uint8( glm::clamp( 255*color.r, 0.f, 255.f ) );
    Uint8 g = Uint8( glm::clamp( 255*color.g, 0.f, 255.f ) );
    Uint8 b = Uint8( glm::clamp( 255*color.b, 0.f, 255.f ) );

    Uint32 colorInt32 =
            r | // red
            (g << 8) | // green
            (b << 16) | // blue
            (255 << 24); // alpha;

    return colorInt32;
}

void Draw() {
    //The surface nees to be locked so that you are allowed change its pixels
    SDL_LockSurface(screen);

    //Do all the pixel manipulation here
    switch(exercise){
        case 1: //Exercise 1: Blue screen
            // For every pixel on screen
            for (int y = 0; y < SCREEN_HEIGHT; ++y) {
                for (int x = 0; x < SCREEN_WIDTH; ++x) {
                    vec3 color(0,0,1); // The values are (Red,Green,Blue) in a scale of 0 to 1
                    PutPixel(screen, x, y, color); //
                }
            }
            break;
        case 2: //Exercise 2: Bilinear interpolation
            for (int y = 0; y < SCREEN_HEIGHT; ++y) {
                for (int x = 0; x < SCREEN_WIDTH; ++x) {
                    vec3 color(0,1,0);
                    PutPixel(screen, x, y, color);
                }
            }
            break;
        case 3: //Exercise 3: Starfield
            for (int y = 0; y < SCREEN_HEIGHT; ++y) {
                for (int x = 0; x < SCREEN_WIDTH; ++x) {
                    vec3 color(1,0,0);
                    PutPixel(screen, x, y, color);
                }
            }
            break;
    }

    //Now we are done unlock the surface
    SDL_UnlockSurface(screen);
}

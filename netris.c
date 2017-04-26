#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 256
#define NUM_OFFSETS 16
#define STACK_SIZE 256

#define RESOURCE_PATH "resources/"

// Functions
SDL_bool update();
void draw();
SDL_bool menu_update(SDL_Keycode);

// Static variables

SDL_Window* window;
SDL_Surface* surface;

#define FLAG_TRUE       0b11111111
Uint8 flags;
#define FLAG_IN_MENU    0b10000000
#define FLAG_PAUSED     0b01000000

Uint8* offsets;
Uint8* stack;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow
        (
         "netris",
         SDL_WINDOWPOS_UNDEFINED,
         SDL_WINDOWPOS_UNDEFINED,
         WINDOW_WIDTH,
         WINDOW_HEIGHT,
         0
        );
    surface = SDL_GetWindowSurface(window);

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    if (surface == NULL)
    {
        printf("Could not create surface: %s\n", SDL_GetError());
        return 1;
    }

    // Init memory
    flags = 0b00000000;
    flags |= FLAG_IN_MENU & FLAG_TRUE;
    offsets = (Uint8*) SDL_malloc(sizeof(Uint8) * NUM_OFFSETS);
    stack = (Uint8*) SDL_malloc(sizeof(Uint8) * STACK_SIZE);
    SDL_memset(offsets, 0, sizeof(Uint8) * NUM_OFFSETS);
    SDL_memset(stack, 0, sizeof(Uint8) * STACK_SIZE);

    // The game loop
    SDL_bool done = SDL_FALSE;
    while (!done)
    {
        done = update();
        draw();
    }

    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

// Offsets:
//  0   Stack pointer
//  1
//  2
//  3
//  4
//  5
//  6
//  7
//  8
//  9
//  A
//  B
//  C
//  D
//  E
//  F

SDL_bool update()
{
    // Variables
    SDL_Event event;
    SDL_bool done = SDL_FALSE;

    // Main update code

    // Event Handling
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            done = SDL_TRUE;
        }
        if (event.type == SDL_KEYUP)
        {
            if (flags & FLAG_IN_MENU)
            {
                done = menu_update(event.key.keysym.sym);
            }
            else
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    done = SDL_TRUE;
                }
            }
        }
    }
    return done;
}

void draw()
{

    // Main draw code
    if (flags & FLAG_IN_MENU)
    {

    }

    //Update
    SDL_UpdateWindowSurface(window);
}

SDL_bool menu_update(SDL_Keycode key)
{
    SDL_bool done = SDL_FALSE;
    if (key == SDLK_ESCAPE)
    {
        if (flags & FLAG_PAUSED)
        {
            flags ^= FLAG_PAUSED | FLAG_IN_MENU;
        }
        else
        {
            flags ^= FLAG_IN_MENU;
            done = SDL_TRUE;
        }
    }
    if (key == SDLK_s)
    {
        // Menu down
    }
    if (key == SDLK_w)
    {
        // Menu up
    }
    if (key == SDLK_a)
    {
        // Menu left
    }
    if (key == SDLK_d)
    {
        // Menu right
    }
    return done;
}


#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300

void update();
void draw(SDL_Renderer*);

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer
        (
         WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer
        );

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    if (renderer == NULL)
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    // The game loop
    SDL_bool done = SDL_FALSE;
    SDL_Event event;
    while (!done)
    {
        update();
        draw(renderer);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = SDL_TRUE;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

void update()
{

}

void draw(SDL_Renderer* renderer)
{

}


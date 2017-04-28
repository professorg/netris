#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 256
#define NUM_OFFSETS 16
#define STACK_SIZE 256

#define STACK_POINTER_OFFSET 0x00
#define MENU_NUMBER_OFFSET 0x01
#define MENU_DATA_OFFSET 0x02

#define RESOURCE_PATH "resources/"
#define MENU_DATA "menus.bin"
#define FONT_DATA "font.bmp"
#define MAIN_MENU 0x00

// Functions
SDL_bool update();
void draw();
void menu_draw();
SDL_bool menu_update(SDL_Keycode);
void stack_push(Uint8);
Uint8 stack_pop();
void read_menu(Uint8);
void draw_text(Uint8, Uint8, char);

// Static variables

SDL_Window* window;
SDL_Surface* surface;

#define FLAG_TRUE       0b11111111
Uint8 flags;
#define FLAG_IN_MENU    0b10000000
#define FLAG_PAUSED     0b01000000

Uint8* offsets;
Uint8* stack;

SDL_Surface* font;

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
         SDL_WINDOW_BORDERLESS
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
    flags |= FLAG_IN_MENU;
    offsets = (Uint8*) SDL_malloc(sizeof(Uint8) * NUM_OFFSETS);
    stack = (Uint8*) SDL_malloc(sizeof(Uint8) * STACK_SIZE);
    SDL_memset(offsets, 0, sizeof(Uint8) * NUM_OFFSETS);
    SDL_memset(stack, 0, sizeof(Uint8) * STACK_SIZE);

    offsets[MENU_NUMBER_OFFSET] = offsets[STACK_POINTER_OFFSET]++;
    stack[offsets[MENU_NUMBER_OFFSET]] = MAIN_MENU;
    offsets[MENU_DATA_OFFSET] = offsets[STACK_POINTER_OFFSET];

    font = SDL_LoadBMP(RESOURCE_PATH FONT_DATA);

    read_menu(stack[offsets[MENU_NUMBER_OFFSET]]);

    // The game loop
    SDL_bool done = SDL_FALSE;
    while (!done)
    {
        done = update();
        draw();
    }

    SDL_free(offsets);
    SDL_free(stack);

    SDL_FreeSurface(font);

    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

// Offsets:
//  0   Stack pointer
//  1   Menu number
//  2   Menu data
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

    SDL_Rect bg_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_FillRect(surface, &bg_rect, 0xFFFFFF);
    // Main draw code
    if (flags & FLAG_IN_MENU)
    {
        menu_draw();
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

void menu_draw()
{
    size_t i = -1;
    size_t j = 0;
    Uint8 x;
    Uint8 y;
    while (stack[offsets[MENU_DATA_OFFSET] + (++i)] != (Uint8)'\n')
    {
        if (j == 0)
        {
            x = stack[offsets[MENU_DATA_OFFSET] + i++];
            y = stack[offsets[MENU_DATA_OFFSET] + i];
            j++;
        }
        else
        {
            if (stack[offsets[MENU_DATA_OFFSET] + i] == (Uint8)';')
            {
                j = 0;
            }
            else
            {
                draw_text(x, y, stack[offsets[MENU_DATA_OFFSET] + i]);
                x += 8;
            }
        }
    }

}

void stack_push(Uint8 data)
{
    stack[offsets[STACK_POINTER_OFFSET]++] = data;
}

Uint8 stack_pop()
{
    return stack[offsets[STACK_POINTER_OFFSET]--];
}

void read_menu(Uint8 menu_index)
{
    FILE* file = fopen(RESOURCE_PATH MENU_DATA, "r");
    char* str = (char*)SDL_malloc(256*sizeof(char));
    char c;
    size_t i = 0;
    while (i < menu_index && (c = fgetc(file)) != EOF)
    {
        if (c == '\n') i++;
    }
    i = 0;
    while ((c = fgetc(file)) != EOF && c != '\n')
    {
        str[i++] = c;
    }
    str[i++] = '\n';
    SDL_memcpy(stack + offsets[MENU_DATA_OFFSET], (Uint8*)str, i * sizeof(char));
    offsets[STACK_POINTER_OFFSET] += i;
    SDL_free(str);
    fclose(file);
}

void draw_text(Uint8 x, Uint8 y, char text)
{
    SDL_Rect src_rect;
    src_rect.x = 8*(text % 16);
    src_rect.y = 8*(text / 16);
    src_rect.w = 8;
    src_rect.h = 8;
    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    SDL_BlitSurface(font, &src_rect, surface, &dst_rect);
}


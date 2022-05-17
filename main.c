#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

// game variables
int WIN_W = 1000;
int WIN_H = 600;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
// when implementing sorting algorithms they only actually have to sort rects[i].h
// selection sort, bubble sort, insertion sort
void selectionsort(SDL_Rect rects[], int usernum)
{

    for (int i = 0; i < usernum-1; i++)
    {
        int min_idx = i;
        for (int j = i+1; j < usernum; j++)
        {
            if (rects[j].h < rects[min_idx].h)
            {
                min_idx = j;
            }
        }
        swap(&rects[min_idx].h, &rects[i].h);
        swap(&rects[min_idx].y, &rects[i].y);
    }

}

bool check(SDL_Rect rects[], int usernum)
{
    for (int i = 0; i < usernum; i++)
    {
        if (rects[i].h < rects[i-1].h)
        {
            return false;
        }
    }
    return true;
}

void shuffle(SDL_Rect rects[], int n)
{
    srand(time(NULL));
    int i;
    for (i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&rects[i].h, &rects[j].h);
    }
}

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage = ./main NUM SORTTYPE\n");
        return 1;
    }
    int usernum = atoi(argv[1]);
    char* sorttype = argv[2];
    if (usernum%100 != 0)
    {
        printf("Number must be divisible by 100\n");
        return 1;
    }

    SDL_Window* win;
    SDL_Renderer* rend;
    if (SDL_CreateWindowAndRenderer(WIN_W, WIN_H, 0, &win, &rend) < 0)
    {
        printf("error initializing window and renderer: %s\n", SDL_GetError());
    }

    // create or rectangle array and populate all of them
    SDL_Rect rects[usernum];
    int blockwidth = (WIN_W - (100*(usernum / 100)))/usernum;
    int blockheight = WIN_H/usernum;
    for (int i = 0, x = 0, h = blockheight; i < usernum; i++, x+=blockwidth + 1, h += blockheight)
    {
        rects[i].h = h;
        rects[i].w = blockwidth;
        rects[i].x = x;
    }
    shuffle(rects, usernum);
    for (int i = 0; i < usernum; i++)
    {
        rects[i].y = WIN_H - rects[i].h;
    }

    int sort = 0;
    int close_requested = 0;
    while (!close_requested)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    close_requested = 1;
                    break;    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_RETURN:
                            sort = 1;
                    }    
            }
        }

        if (sort == 1)
        {
            if (strcmp(sorttype, "selectionsort") == 0)
            {
                selectionsort(rects, usernum);
            }

            if (check(rects, usernum) == false)
                printf("Sorting failed with: %s\n", sorttype);
            sort = 0;
        }

        for (int i = 0; i < usernum; i++)
        {
            SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            SDL_RenderFillRect(rend, &rects[i]);
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0 , 255);
        SDL_RenderPresent(rend);
        SDL_RenderClear(rend);

        SDL_Delay(1000/60);
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
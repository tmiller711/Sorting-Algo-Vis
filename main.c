#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <pthread.h>

// game variables
int WIN_W = 1000;
int WIN_H = 600;
int usernum;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void drawrects(SDL_Rect rects[], SDL_Renderer* rend, int i, int j)
{
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    for(int k = 0; k < usernum; k++)
    {
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderFillRect(rend, &rects[k]);
    }
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderFillRect(rend, &rects[i - 1]);
    SDL_SetRenderDrawColor(rend, 0, 128, 0, 255);
    SDL_RenderFillRect(rend, &rects[j]);
    SDL_RenderPresent(rend);
}

void drawmerge(SDL_Rect rects[], SDL_Renderer* rend, int len, int l1, int r1)
{
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    for(int k = 0; k < usernum; k++)
    {
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderFillRect(rend, &rects[k]);
    }
	SDL_SetRenderDrawColor(rend, 0, 128, 0, 255);
	SDL_RenderFillRect(rend, &rects[l1]);
	SDL_RenderFillRect(rend, &rects[r1]);

    SDL_RenderPresent(rend);
}

void mergelists(SDL_Rect rects[], SDL_Renderer* rend, int l, int m, int r)
{
	// draw the line going through both halves at the same time
    int n1 = m - l + 1;
	int n2 = r - m;

	int l1 = l;
	int r1 = r-m+l;
	for (int i = 0; i < r-m; i++){
		drawmerge(rects, rend, 0, l1, r1);
		l1++;
		r1++;
	SDL_Delay(100);
	}
	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (int i = 0; i < n1; i++)
		L[i] = rects[l + i].h;
	for (int j = 0; j < n2; j++)
		R[j] = rects[m + 1 + j].h;

	/* Merge the temp arrays back into arr[l..r]*/
	int i = 0; // Initial index of first subarray
	int j = 0; // Initial index of second subarray
	int k = l; // Initial index of merged subarray
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			rects[k].h = L[i];
			rects[k].y = WIN_H - L[i];
			i++;
		}
		else {
			rects[k].h = R[j];
			rects[k].y = WIN_H - R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1) {
		rects[k].h = L[i];
		rects[k].y = WIN_H - L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2) {
		rects[k].h = R[j];
		rects[k].y = WIN_H - R[j];
		j++;
		k++;
	}

	drawmerge(rects, rend, 0, -1, -1);
    usleep(99900);
}

void mergesort(SDL_Rect rects[], SDL_Renderer* rend, int l, int r)
{
    if (l < r) 
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        // gets middle of list
        int m = l + (r - l) / 2;
  
        // Sort first and second halves
        mergesort(rects, rend, l, m);
        mergesort(rects, rend, m + 1, r);
        mergelists(rects, rend, l, m, r);
    }
}

void bubblesort(SDL_Rect rects[], SDL_Renderer* rend)
{
    int sleeptime = 2000;
    for (int i = 0; i < usernum - 1; i++)
    {
        for(int j = 0; j < usernum - i - 1; j++)
        {
            if (rects[j].h > rects[j + 1].h)
            {
                swap(&rects[j].h, &rects[j + 1].h);
                swap(&rects[j].y, &rects[j + 1].y);
            }
            drawrects(rects, rend, j, j);
            usleep(sleeptime);
        }
        usleep(sleeptime * 3);
    }
}

// when implementing sorting algorithms they only actually have to sort rects[i].h
// selection sort, bubble sort, insertion sort
void selectionsort(SDL_Rect rects[], SDL_Renderer* rend)
{
    int sleeptime = 1000;
    for (int i = 0; i < usernum; i++)
    {
        SDL_RenderClear(rend);
        int min_idx = i;
        for (int j = i; j < usernum; j++)
        {
            drawrects(rects, rend, i + 1, j);
            if (rects[j].h < rects[min_idx].h)
            {
                min_idx = j;
            }
            usleep(sleeptime);
        }
        swap(&rects[min_idx].h, &rects[i].h);
        swap(&rects[min_idx].y, &rects[i].y);
        usleep(sleeptime * 3);
    }
}

bool check(SDL_Rect rects[], SDL_Renderer* rend)
{
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    for(int k = 0; k < usernum; k++)
    {
		if (rects[k].h < rects[k-1].h) {
			return false;
		}
        SDL_SetRenderDrawColor(rend, 0, 128, 0, 255);
        SDL_RenderFillRect(rend, &rects[k]);
    }
    SDL_RenderPresent(rend);
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

void* userexit()
{
    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                exit(EXIT_SUCCESS);
        }
    }
}

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage = ./main NUM SORTTYPE\n");
        return 1;
    }
    usernum = atoi(argv[1]);
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

        SDL_SetRenderDrawColor(rend, 0, 0, 0 , 255);
        SDL_RenderClear(rend);

        pthread_t id;
        if (sort == 1)
        {
            // create thread that waits for user to exit
            pthread_create(&id, NULL, userexit, NULL);
            if (strcmp(sorttype, "selection") == 0)
                selectionsort(rects, rend);
            else if (strcmp(sorttype, "bubble") == 0)
                bubblesort(rects, rend);
            else if (strcmp(sorttype, "merge") == 0)
                mergesort(rects, rend, 0, usernum - 1);
				printf("done");

			if (check(rects, rend) == false)
                printf("Sorting failed with: %s\n", sorttype);

            pthread_join(id, NULL);
            //sort = 0;
        }

		if (sort == 0) {
				for (int i = 0; i < usernum; i++)
				{
					SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
					SDL_RenderFillRect(rend, &rects[i]);
				}
		}

        SDL_RenderPresent(rend);

        SDL_Delay(1000/60);
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

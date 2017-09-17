////////////////////////////////////////////////////////////////////////////////
/////// Author: Andres Zibula                                           ////////
/////// Source: https://github.com/andres-zibula/game-of-life           ////////
////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string>

const int BLOCK_SIZE = 64;
const int BLOCKS_X = 16;
const int BLOCKS_Y = 8;
const int LINE_WIDTH = 2;
const int SCREEN_WIDTH = LINE_WIDTH + (BLOCK_SIZE + LINE_WIDTH) * BLOCKS_X;
const int SCREEN_HEIGHT = LINE_WIDTH + (BLOCK_SIZE + LINE_WIDTH) * BLOCKS_Y;

bool init();
void close();
int cant_vecinos(bool **matrix, int x, int y);

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

bool init()
{
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    // Create window
    gWindow = SDL_CreateWindow("(paused) Game Of Life, Andres Zibula",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      // Create renderer for window
      gRenderer = SDL_CreateRenderer(
          gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (gRenderer == NULL)
      {
        printf("Renderer could not be created! SDL Error: %s\n",
               SDL_GetError());
        success = false;
      }
      else
      {
        // Initialize renderer color
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
          printf("SDL_image could not initialize! SDL_image Error: %s\n",
                 IMG_GetError());
          success = false;
        }
      }
    }
  }

  return success;
}

void close()
{
  // Destroy window
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  // Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

int cant_vecinos(bool **matrix, int x, int y)
{
  int cantidad = 0;

  if (x == 0)
  {
    if (y == 0)
    {
      if (matrix[y][x + 1])
        cantidad++;
      if (matrix[y + 1][x])
        cantidad++;
      if (matrix[y + 1][x + 1])
        cantidad++;
    }
    else if (y == BLOCKS_Y - 1)
    {

      if (matrix[y - 1][x])
        cantidad++;
      if (matrix[y - 1][x + 1])
        cantidad++;
      if (matrix[y][x + 1])
        cantidad++;
    }
    else
    {

      if (matrix[y - 1][x])
        cantidad++;
      if (matrix[y - 1][x + 1])
        cantidad++;
      if (matrix[y][x + 1])
        cantidad++;
      if (matrix[y + 1][x])
        cantidad++;
      if (matrix[y + 1][x + 1])
        cantidad++;
    }
  }
  else if (x == BLOCKS_X - 1)
  {
    if (y == 0)
    {
      if (matrix[y][x - 1])
        cantidad++;
      if (matrix[y + 1][x - 1])
        cantidad++;
      if (matrix[y + 1][x])
        cantidad++;
    }
    else if (y == BLOCKS_Y - 1)
    {
      if (matrix[y - 1][x - 1])
        cantidad++;
      if (matrix[y - 1][x])
        cantidad++;
      if (matrix[y][x - 1])
        cantidad++;
    }
    else
    {
      if (matrix[y - 1][x - 1])
        cantidad++;
      if (matrix[y - 1][x])
        cantidad++;
      if (matrix[y][x - 1])
        cantidad++;
      if (matrix[y + 1][x - 1])
        cantidad++;
      if (matrix[y + 1][x])
        cantidad++;
    }
  }
  else if (y == 0)
  {
    if (matrix[y][x - 1])
      cantidad++;
    if (matrix[y][x + 1])
      cantidad++;
    if (matrix[y + 1][x - 1])
      cantidad++;
    if (matrix[y + 1][x])
      cantidad++;
    if (matrix[y + 1][x + 1])
      cantidad++;
  }
  else if (y == BLOCKS_Y - 1)
  {
    if (matrix[y - 1][x - 1])
      cantidad++;
    if (matrix[y - 1][x])
      cantidad++;
    if (matrix[y - 1][x + 1])
      cantidad++;
    if (matrix[y][x - 1])
      cantidad++;
    if (matrix[y][x + 1])
      cantidad++;
  }
  else
  {
    if (matrix[y - 1][x - 1])
      cantidad++;
    if (matrix[y - 1][x])
      cantidad++;
    if (matrix[y - 1][x + 1])
      cantidad++;
    if (matrix[y][x - 1])
      cantidad++;
    if (matrix[y][x + 1])
      cantidad++;
    if (matrix[y + 1][x - 1])
      cantidad++;
    if (matrix[y + 1][x])
      cantidad++;
    if (matrix[y + 1][x + 1])
      cantidad++;
  }

  return cantidad;
}

int main(int argc, char *args[])
{
  // Start up SDL and create window
  if (!init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    bool **matrix = new bool *[BLOCKS_Y];
    for (int i = 0; i < BLOCKS_Y; i++)
    {
      matrix[i] = new bool[BLOCKS_X];
      memset(matrix[i], false, sizeof(bool) * BLOCKS_X);
    }

    bool **aux_matrix = new bool *[BLOCKS_Y];
    for (int i = 0; i < BLOCKS_Y; i++)
    {
      aux_matrix[i] = new bool[BLOCKS_X];
      memset(aux_matrix[i], false, sizeof(bool) * BLOCKS_X);
    }

    bool **tmp_matrix = 0;

    bool toggle_mouse = false;
    bool toggle_space = false;
    bool pause = true;

    // While application is running
    while (!quit)
    {
      // Handle events on queue
      while (SDL_PollEvent(&e) != 0)
      {
        // User requests quit
        if (e.type == SDL_QUIT)
        {
          quit = true;
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && !toggle_space)
        {
          toggle_space = true;
          pause = !pause;

          if (pause)
          {
            SDL_SetWindowTitle(gWindow, "(paused) Game Of Life, Andres Zibula");
          }
          else
          {
            SDL_SetWindowTitle(gWindow, "Game Of Life, Andres Zibula");
          }
        }

        if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE)
        {
          toggle_space = false;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && !toggle_mouse)
        {
          toggle_mouse = true;
          int x, y;
          SDL_GetMouseState(&x, &y);

          matrix[y / (BLOCK_SIZE + 2)][x / (BLOCK_SIZE + 2)] =
              !matrix[y / (BLOCK_SIZE + 2)][x / (BLOCK_SIZE + 2)];
        }

        if (e.type == SDL_MOUSEBUTTONUP)
        {
          toggle_mouse = false;
        }
      }

      if (!pause)
      {
        for (int i = 0; i < BLOCKS_Y; i++)
        {
          memset(aux_matrix[i], false, sizeof(bool *) * BLOCKS_X);
        }

        for (int y = 0; y < BLOCKS_Y; y++)
        {
          for (int x = 0; x < BLOCKS_X; x++)
          {
            if (matrix[y][x])
            {
              if (cant_vecinos(matrix, x, y) < 2)
                aux_matrix[y][x] = false;
              else if (cant_vecinos(matrix, x, y) > 3)
                aux_matrix[y][x] = false;
              else
                aux_matrix[y][x] = true;
            }
            else
            {
              if (cant_vecinos(matrix, x, y) == 3)
                aux_matrix[y][x] = true;
            }
          }
        }

        tmp_matrix = matrix;
        matrix = aux_matrix;
        aux_matrix = tmp_matrix;
      }

      // Clear screen
      SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderClear(gRenderer);

      for (int x = 0; x < BLOCKS_X; x++)
      {
        for (int y = 0; y < BLOCKS_Y; y++)
        {
          if (matrix[y][x])
          {
            SDL_Rect fillRect = {2 + x * BLOCK_SIZE + 2 * x,
                                 2 + y * BLOCK_SIZE + 2 * y, BLOCK_SIZE,
                                 BLOCK_SIZE};
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &fillRect);
          }
          else
          {
            SDL_Rect fillRect = {2 + x * BLOCK_SIZE + 2 * x,
                                 2 + y * BLOCK_SIZE + 2 * y, BLOCK_SIZE,
                                 BLOCK_SIZE};
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderFillRect(gRenderer, &fillRect);
          }
        }
      }

      // Update screen
      SDL_RenderPresent(gRenderer);
      if (!pause)
        SDL_Delay(300);
    }

    for (int i = 0; i < BLOCKS_Y; i++)
    {
      delete[] aux_matrix[i];
    }
    delete[] aux_matrix;

    for (int i = 0; i < BLOCKS_Y; i++)
    {
      delete[] matrix[i];
    }
    delete[] matrix;
  }

  // Free resources and close SDL
  close();

  return 0;
}

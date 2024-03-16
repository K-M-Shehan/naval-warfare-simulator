#include <SDL2/SDL.h>
#define WINDOW_SIZE 700

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void initializeSDL (void) 
{
  // SDL Initialization
  SDL_Init(SDL_INIT_VIDEO);
  // Creating a window
  window = SDL_CreateWindow("Naval Warfare Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
  // Creating a renderer for the window
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
}

void cleanup (void)
{
   // Cleanup
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main (void) 
{
  initializeSDL();

   // Quiting
  int quit = 0;
  SDL_Event e;

  while (!quit)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        quit = 1;
      }
    }
  }

  cleanup();
  return 0;
}

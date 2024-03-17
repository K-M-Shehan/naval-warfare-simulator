#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_SIZE 700

typedef struct
{
  int x, y;         // position
  short state;      // active or destroyed
  char* timeActive; // time spent in simulator
  char* name;       // name of escort ship       
  int id;           // unique id
} EscortShip;

void initializeSDL (SDL_Window* window, SDL_Renderer* renderer) 
{
  // SDL Initialization
  SDL_Init(SDL_INIT_VIDEO);
  // Creating a window
  window = SDL_CreateWindow("Naval Warfare Simulator", 
                            SDL_WINDOWPOS_UNDEFINED, 
                            SDL_WINDOWPOS_UNDEFINED, 
                            WINDOW_SIZE, 
                            WINDOW_SIZE, 
                            SDL_WINDOW_SHOWN
                            );
  // Creating a renderer for the window
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
}

int processEvents (SDL_Window* window)
{
  SDL_Event event;
  int done = 0;

     while (SDL_PollEvent(&event))
      {
        switch (event.type)
        {
          case SDL_QUIT:
            done = 1;
          break;
          
        }
      }

  return done;
}

void cleanup (SDL_Window* window, SDL_Renderer* renderer)
{
   // Cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

int main (void) 
{
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  initializeSDL(window, renderer);

  // Running events
  int done = 0;
  while (!done)
  {  
    // Check for events
    done = processEvents(window);
  }

  // Free memory
  cleanup(window, renderer);
  return 0;
}

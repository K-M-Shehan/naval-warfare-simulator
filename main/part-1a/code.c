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

typedef struct
{

  // Sprites
  SDL_Texture *battleship;
} SimState;

int processEvents (SDL_Window* window, SimState *sim) // TODO: include structs in this to move around idk
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

void doRender (SDL_Renderer* renderer, SimState* sim) // TODO:  include struct here as well to render the struct, when initializing rect in this use the positions of the struct to do it ma man
{
  // set drawing color to blue
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  // clear the screen to blue
  SDL_RenderClear(renderer);

  // draw the battleship
  SDL_Rect battleshipRect = { 50, 50, 64, 64 };
  SDL_RenderCopy(renderer, sim->battleship, NULL, &battleshipRect);

  SDL_RenderPresent(renderer);
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
  SimState simState;
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  SDL_Surface *battleshipSurface = NULL;

  // SDL Initialization
  SDL_Init(SDL_INIT_VIDEO);
 
  // Creating a window
  window = SDL_CreateWindow("Naval Warfare Simulator", 
                            SDL_WINDOWPOS_UNDEFINED, 
                            SDL_WINDOWPOS_UNDEFINED, 
                            WINDOW_SIZE, 
                            WINDOW_SIZE, 
                            0
                            );
  
  // Creating a renderer for the window
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
  
  // Load images and render textures
  battleshipSurface = IMG_Load("battleship.png");
  if (battleshipSurface == NULL)
  {
    printf("Cannot find battleship.png!\n\n");
    cleanup(window, renderer);
    return 1;
  }

  if (simState.battleship == NULL)
  {
    printf("Failed to create texture from surface: %s\n", SDL_GetError());
    cleanup(window, renderer);
    return 1;
  }

  // Load surface into texture and free surface
  simState.battleship = SDL_CreateTextureFromSurface(renderer, battleshipSurface);
  SDL_FreeSurface(battleshipSurface);
  
  // Running events
  int done = 0;
  while (!done)
  {  
    // Check for events
    done = processEvents(window, &simState);
    doRender(renderer, &simState);
  }

  // Free memory
  SDL_DestroyTexture(simState.battleship);
  cleanup(window, renderer);
  return 0;
}

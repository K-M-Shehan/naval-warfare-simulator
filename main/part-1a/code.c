#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#define WINDOW_SIZE 500
#define PI 3.14159265

const float G = 9.8f; // Gravitational acceleration

typedef struct
{
  int x, y;         // position
  short state;      // active or destroyed
  char* timeActive; // time spent in simulator
  char* name;       // name of escort ship       
  int id;           // unique id
  SDL_Texture *texE;
} EscortShip;

typedef struct
{
  int x, y;
  float angle;
  SDL_Texture *texB;
} Battleship;

typedef struct
{
  int x, y;
  SDL_Texture *texS;
} Shell;

typedef struct
{
  /*Main Sprites*/
  // Escorts
  EscortShip escortA, escortB, escortC, escortD, escortE;   // you could also use an array to draw these
  // Battleships
  Battleship battleU, battleM, battleR, battleS;  // all of these types would use the same sprite
  // Shell
  Shell shellB, shellC;
} SimState;




void cleanup (SDL_Window* window, SDL_Renderer* renderer)
{
   // Cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void loadSim (SimState *sim, SDL_Window *window, SDL_Renderer *renderer)
{  
  SDL_Surface *battleUSurface = NULL;
  SDL_Surface *battleMSurface = NULL;
  SDL_Surface *battleRSurface = NULL;
  SDL_Surface *battleSSurface = NULL;

  SDL_Surface *escortASurface = NULL;
  SDL_Surface *escortBSurface = NULL;
  SDL_Surface *escortCSurface = NULL;
  SDL_Surface *escortDSurface = NULL;
  SDL_Surface *escortESurface = NULL;
  // Load images and render textures
  
  // Load battleship image
  battleUSurface = IMG_Load("resources/battleship.png");
  if (battleUSurface == NULL)
  {
    printf("Cannot find battleship.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }
  if (sim->battleU.texB == NULL)
  {
    printf("Failed to create battleship texture from surface: %s\n", SDL_GetError());
    cleanup(window, renderer);
    exit(1);
  }

  battleMSurface = IMG_Load("resources/battleship.png");
  if (battleMSurface == NULL)
  {
    printf("Cannot find battleship.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }
  if (sim->battleM.texB == NULL)
  {
    printf("Failed to create battleship texture from surface: %s\n", SDL_GetError());
    cleanup(window, renderer);
    exit(1);
  }
 
  battleRSurface = IMG_Load("resources/battleship.png");
  if (battleRSurface == NULL)
  {
    printf("Cannot find battleship.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }
  if (sim->battleR.texB == NULL)
  {
    printf("Failed to create battleship texture from surface: %s\n", SDL_GetError());
    cleanup(window, renderer);
    exit(1);
  }
 
  battleSSurface = IMG_Load("resources/battleship.png");
  if (battleSSurface == NULL)
  {
    printf("Cannot find battleship.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }
  if (sim->battleS.texB == NULL)
  {
    printf("Failed to create battleship texture from surface: %s\n", SDL_GetError());
    cleanup(window, renderer);
    exit(1);
  }


  // Load escortships' images
  escortASurface = IMG_Load("resources/escortshipA.png");
  if (escortASurface == NULL)
  {
    printf("Cannot find escortshipA.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }
   escortBSurface = IMG_Load("resources/escortshipB.png");
  if (escortBSurface == NULL)
  {
    printf("Cannot find escortshipB.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  escortCSurface = IMG_Load("resources/escortshipC.png");
  if (escortCSurface == NULL)
  {
    printf("Cannot find escortshipC.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  escortDSurface = IMG_Load("resources/escortshipD.png");
  if (escortDSurface == NULL)
  {
    printf("Cannot find escortshipD.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  escortESurface = IMG_Load("resources/escortshipE.png");
  if (escortESurface == NULL)
  {
    printf("Cannot find escortshipE.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  // Load surface into texture and free surface
  sim->battleU.texB = SDL_CreateTextureFromSurface(renderer, battleUSurface);
  SDL_FreeSurface(battleUSurface);

  sim->battleM.texB = SDL_CreateTextureFromSurface(renderer, battleMSurface);
  SDL_FreeSurface(battleMSurface);

  sim->battleR.texB = SDL_CreateTextureFromSurface(renderer, battleRSurface);
  SDL_FreeSurface(battleRSurface);

  sim->battleS.texB = SDL_CreateTextureFromSurface(renderer, battleSSurface);
  SDL_FreeSurface(battleSSurface);


  // init escorts
  sim->escortA.texE = SDL_CreateTextureFromSurface(renderer, escortASurface);
  SDL_FreeSurface(escortASurface);

  sim->escortB.texE = SDL_CreateTextureFromSurface(renderer, escortBSurface);
  SDL_FreeSurface(escortBSurface);
  
  sim->escortC.texE = SDL_CreateTextureFromSurface(renderer, escortCSurface);
  SDL_FreeSurface(escortCSurface);
  
  sim->escortD.texE = SDL_CreateTextureFromSurface(renderer, escortDSurface);
  SDL_FreeSurface(escortDSurface);
  
  sim->escortE.texE = SDL_CreateTextureFromSurface(renderer, escortESurface);
  SDL_FreeSurface(escortESurface);

  // x and y positions of battleships
  sim->battleU.x = 50;
  sim->battleU.y = WINDOW_SIZE - 100;

  sim->battleM.x = 50;
  sim->battleM.y = WINDOW_SIZE - 100;

  sim->battleR.x = 50;
  sim->battleR.y = WINDOW_SIZE - 100;

  sim->battleS.x = 50;
  sim->battleS.y = WINDOW_SIZE - 100;


  // x and y positions of escorts
  sim->escortA.x = rand() % (WINDOW_SIZE - 64);
  sim->escortA.y = rand() % (WINDOW_SIZE - 64);

  sim->escortB.x = rand() % (WINDOW_SIZE - 64);
  sim->escortB.y = rand() % (WINDOW_SIZE - 64);

  sim->escortC.x = rand() % (WINDOW_SIZE - 64);
  sim->escortC.y = rand() % (WINDOW_SIZE - 64);
  
  sim->escortD.x = rand() % (WINDOW_SIZE - 64);
  sim->escortD.y = rand() % (WINDOW_SIZE - 64);
  
  sim->escortE.x = rand() % (WINDOW_SIZE - 64);
  sim->escortE.y = rand() % (WINDOW_SIZE - 64);
}

void drawBattleShell (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)
{
  // Load the shell image and put it into a surface
  SDL_Surface *shellSurface = NULL;
  shellSurface = IMG_Load("resources/shell.png");
  if (shellSurface == NULL)
  {
    printf("Cannot find shell.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  // Create texture from surface and free surface
  sim->shellB.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellB.x = 65;
  sim->shellB.y = WINDOW_SIZE - 70;

  SDL_Rect shellBRect = { sim->shellB.x, sim->shellB.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellB.texS, NULL, &shellBRect);
  SDL_RenderPresent(renderer);
}

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

void doRender (SDL_Renderer* renderer, SimState* sim, int battleshipType) // TODO:  include struct here as well to render the struct, when initializing rect in this use the positions of the struct to do it ma man
{
  // set drawing color to blue
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  // clear the screen to blue
  SDL_RenderClear(renderer);

  switch(battleshipType) // TODO: gotta change this int to mouse click or something
  {
    // draw battleships
    case 1:
      SDL_Rect battleURect = { sim->battleU.x, sim->battleU.y, 64, 64 };
      SDL_RenderCopy(renderer, sim->battleU.texB, NULL, &battleURect);
    break;
    case 2:
      SDL_Rect battleMRect = { sim->battleM.x, sim->battleM.y, 64, 64 };
      SDL_RenderCopy(renderer, sim->battleM.texB, NULL, &battleMRect);
    break;
    case 3:
      SDL_Rect battleRRect = { sim->battleR.x, sim->battleR.y, 64, 64 };
      SDL_RenderCopy(renderer, sim->battleR.texB, NULL, &battleRRect);
    break;
    case 4:
      SDL_Rect battleSRect = { sim->battleS.x, sim->battleS.y, 64, 64 };
      SDL_RenderCopy(renderer, sim->battleS.texB, NULL, &battleSRect);
    break;
  }


  // draw escorts
  SDL_Rect escortARect = {sim->escortA.x, sim->escortA.y, 64, 64}; 
  SDL_RenderCopy(renderer, sim->escortA.texE, NULL, &escortARect);

  SDL_Rect escortBRect = {sim->escortB.x, sim->escortB.y, 64, 64}; 
  SDL_RenderCopy(renderer, sim->escortB.texE, NULL, &escortBRect);
  
  SDL_Rect escortCRect = {sim->escortC.x, sim->escortC.y, 64, 64}; 
  SDL_RenderCopy(renderer, sim->escortC.texE, NULL, &escortCRect);

  SDL_Rect escortDRect = {sim->escortD.x, sim->escortD.y, 64, 64}; 
  SDL_RenderCopy(renderer, sim->escortD.texE, NULL, &escortDRect);

  SDL_Rect escortERect = {sim->escortE.x, sim->escortE.y, 64, 64}; 
  SDL_RenderCopy(renderer, sim->escortE.texE, NULL, &escortERect);

  SDL_RenderPresent(renderer);
}


int main (void) 
{
  SimState simState;
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  // SDL Initialization
  SDL_Init(SDL_INIT_VIDEO);

  // Inilialize random number generator
  srand((int)time(NULL)); 

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
   
  loadSim(&simState, window, renderer);
  
  // Running events
  int done = 0;
  while (!done)
  {  
    // Check for events
    done = processEvents(window, &simState);
    doRender(renderer, &simState, 1); // the 3rd arg is the battleship type (using ints for now)
    drawBattleShell(renderer, window, &simState);
  }

  // Free memory
  SDL_DestroyTexture(simState.battleU.texB);
  SDL_DestroyTexture(simState.battleM.texB);
  SDL_DestroyTexture(simState.battleR.texB);
  SDL_DestroyTexture(simState.battleS.texB);

  SDL_DestroyTexture(simState.escortA.texE);
  SDL_DestroyTexture(simState.escortB.texE);
  SDL_DestroyTexture(simState.escortC.texE);
  SDL_DestroyTexture(simState.escortD.texE);
  SDL_DestroyTexture(simState.escortE.texE);

  SDL_DestroyTexture(simState.shellB.texS);
  cleanup(window, renderer);
  return 0;
}

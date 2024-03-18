#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#define WINDOW_SIZE 300
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
  float v;
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

  // Load surface into texture and free surface
  sim->battleU.texB = SDL_CreateTextureFromSurface(renderer, battleUSurface);
  SDL_FreeSurface(battleUSurface);
  
  if (sim->battleU.texB == NULL)
  {
    printf("Failed to create battleshipU texture from surface: %s\n", SDL_GetError());
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

  // Load surface into texture and free surface
  sim->battleM.texB = SDL_CreateTextureFromSurface(renderer, battleMSurface);
  SDL_FreeSurface(battleMSurface);

  if (sim->battleM.texB == NULL)
  {
    printf("Failed to create battleshipM texture from surface: %s\n", SDL_GetError());
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

  // Load surface into texture and free surface
  sim->battleR.texB = SDL_CreateTextureFromSurface(renderer, battleRSurface);
  SDL_FreeSurface(battleRSurface);

  if (sim->battleR.texB == NULL)
  {
    printf("Failed to create battleshipR texture from surface: %s\n", SDL_GetError());
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

  // Load surface into texture and free surface
  sim->battleS.texB = SDL_CreateTextureFromSurface(renderer, battleSSurface);
  SDL_FreeSurface(battleSSurface);

  if (sim->battleS.texB == NULL)
  {
    printf("Failed to create battleshipS texture from surface: %s\n", SDL_GetError());
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
  sim->battleU.y = WINDOW_SIZE - (64+50);

  sim->battleM.x = 50;
  sim->battleM.y = WINDOW_SIZE - (64+50);

  sim->battleR.x = 50;
  sim->battleR.y = WINDOW_SIZE - (64+50);

  sim->battleS.x = 50;
  sim->battleS.y = WINDOW_SIZE - (64+50);


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

  // v of battleships
  sim->battleU.v = 35;
  sim->battleM.v = 35;
  sim->battleR.v = 35;
  sim->battleS.v = 35;

  // state of escorts
  sim->escortA.state = 1;
  sim->escortB.state = 1;
  sim->escortC.state = 1;
  sim->escortD.state = 1;
  sim->escortE.state = 1;
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
  sim->shellB.x = 50;
  sim->shellB.y = WINDOW_SIZE - (64+50);

  SDL_Rect shellBRect = { sim->shellB.x, sim->shellB.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellB.texS, NULL, &shellBRect);
  SDL_RenderPresent(renderer);
}

bool checkCollisionEscortA(SimState *sim)
{
  // calculate corner coordinates
  int escortTopRightX = sim->escortA.x;
  int escortTopRightY = sim->escortA.y;
  int escortBotLeftX = sim->escortA.x + (64-1);
  int escortBotLeftY = sim->escortA.y + (64-1);

  int shellTopRightX = sim->shellB.x;
  int shellTopRightY = sim->shellB.y;
  int shellBotLeftX = sim->shellB.x + (10-1);
  int shellBotLeftY = sim->shellB.y + (10-1);

  // check for collision
  if ((shellTopRightX <= escortBotLeftX) &&
      (shellBotLeftX >= escortTopRightX) &&
      (shellTopRightY <= escortBotLeftY) &&
      (shellBotLeftY >= escortTopRightY))
  {
    return true;
    printf("collision is true\n");
  }
  else
  {
    return false;
  }
}

bool checkCollisionEscortB(SimState *sim)
{
  // calculate corner coordinates
  int escortTopRightX = sim->escortB.x;
  int escortTopRightY = sim->escortB.y;
  int escortBotLeftX = sim->escortB.x + (64-1);
  int escortBotLeftY = sim->escortB.y + (64-1);

  int shellTopRightX = sim->shellB.x;
  int shellTopRightY = sim->shellB.y;
  int shellBotLeftX = sim->shellB.x + (10-1);
  int shellBotLeftY = sim->shellB.y + (10-1);

  // check for collision
  if ((shellTopRightX <= escortBotLeftX) &&
      (shellBotLeftX >= escortTopRightX) &&
      (shellTopRightY <= escortBotLeftY) &&
      (shellBotLeftY >= escortTopRightY))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool checkCollisionEscortC(SimState *sim)
{
  // calculate corner coordinates
  int escortTopRightX = sim->escortC.x;
  int escortTopRightY = sim->escortC.y;
  int escortBotLeftX = sim->escortC.x + (64-1);
  int escortBotLeftY = sim->escortC.y + (64-1);

  int shellTopRightX = sim->shellB.x;
  int shellTopRightY = sim->shellB.y;
  int shellBotLeftX = sim->shellB.x + (10-1);
  int shellBotLeftY = sim->shellB.y + (10-1);

  // check for collision
  if ((shellTopRightX <= escortBotLeftX) &&
      (shellBotLeftX >= escortTopRightX) &&
      (shellTopRightY <= escortBotLeftY) &&
      (shellBotLeftY >= escortTopRightY))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool checkCollisionEscortD(SimState *sim)
{
  // calculate corner coordinates
  int escortTopRightX = sim->escortD.x;
  int escortTopRightY = sim->escortD.y;
  int escortBotLeftX = sim->escortD.x + (64-1);
  int escortBotLeftY = sim->escortD.y + (64-1);

  int shellTopRightX = sim->shellB.x;
  int shellTopRightY = sim->shellB.y;
  int shellBotLeftX = sim->shellB.x + (10-1);
  int shellBotLeftY = sim->shellB.y + (10-1);

  // check for collision
  if ((shellTopRightX <= escortBotLeftX) &&
      (shellBotLeftX >= escortTopRightX) &&
      (shellTopRightY <= escortBotLeftY) &&
      (shellBotLeftY >= escortTopRightY))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool checkCollisionEscortE(SimState *sim)
{
  // calculate corner coordinates
  int escortTopRightX = sim->escortE.x;
  int escortTopRightY = sim->escortE.y;
  int escortBotLeftX = sim->escortE.x + (64-1);
  int escortBotLeftY = sim->escortE.y + (64-1);

  int shellTopRightX = sim->shellB.x;
  int shellTopRightY = sim->shellB.y;
  int shellBotLeftX = sim->shellB.x + (10-1);
  int shellBotLeftY = sim->shellB.y + (10-1);

  // check for collision
  if ((shellTopRightX <= escortBotLeftX) &&
      (shellBotLeftX >= escortTopRightX) &&
      (shellTopRightY <= escortBotLeftY) &&
      (shellBotLeftY >= escortTopRightY))
  {
    return true;
  }
  else
  {
    return false;
  }
}

float getRange (float v)
{
  const float angle = PI / 4;
  // printf("return value of range: %f\n", (pow(v, 2) * sin(2 * angle)) / G);
  return (pow(v, 2) * sin(2 * angle)) / G;
}

float getTimeToTarget (float y, float vy) // this isn't executed at all !
{
  // printf("return value of time to tar: %f\n", 2 * vy / G);
  return 2 * vy / G;
}

void fireBattleShell (SDL_Renderer *renderer, SDL_Window *window, SimState *sim, int battleshipType, int xPos, int yPos)
{
  float battleshipX = 0;
  float battleshipY = 0;
  float v = 0;

  switch(battleshipType)
  {
    case 1:
      battleshipX = sim->battleU.x;
      battleshipY = sim->battleU.y;
      v = sim->battleU.v;
    break;
    case 2:
      battleshipX = sim->battleM.x; 
      battleshipY = sim->battleM.y;
      v = sim->battleM.v;
    break;
    case 3:
      battleshipX = sim->battleR.x;
      battleshipY = sim->battleR.y;
      v = sim->battleR.v;
    break;
    case 4:
      battleshipX = sim->battleS.x;
      battleshipY = sim->battleS.y;
      v = sim->battleS.v;
    break;
  }
  
  float target_x = xPos - 32;
  float target_y = yPos - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double dist = sqrt(pow(dx, 2) + pow(dy, 2));
  // printf("distance: %lf\n", dist); // remove this after
  
  double vx = v * dx / dist;
  double vy = v * dy / dist;
  vx = fabs(vy);
  vy = fabs(vy);

  float x = battleshipX;
  float y = battleshipY;
  // printf("x: %f\n", x);
  // printf("y: %f\n", y);
  float time = 0;

  while (y > 0 && dist <= getRange(v))
  {
    // Update position
    x += vx * time;
    y += vy * time - 0.5f * G * pow(time, 2);

    // check for collision
    if (checkCollisionEscortA(sim))
    {
      sim->escortA.state = 0; // that means destroyed
      printf("escortA destroyed!\n");
      SDL_DestroyTexture(sim->escortA.texE);
      break;
    }
    if (checkCollisionEscortB(sim))
    {
      sim->escortB.state = 0; // that means destroyed

      printf("escortB destroyed!\n");
      SDL_DestroyTexture(sim->escortB.texE);
      break;
    }
    if (checkCollisionEscortC(sim))
    {
      sim->escortC.state = 0; // that means destroyed
      
      printf("escortC destroyed!\n");
      SDL_DestroyTexture(sim->escortC.texE);
      break;
    }
    if (checkCollisionEscortD(sim))
    {
      sim->escortD.state = 0; // that means destroyed 
      
      printf("escortD destroyed!\n");
      SDL_DestroyTexture(sim->escortD.texE);
      break;
    }
    if (checkCollisionEscortE(sim))
    {
      sim->escortE.state = 0; // that means destroyed 
      printf("escortE destroyed!\n");
      SDL_DestroyTexture(sim->escortE.texE);
      break;
    }
    
    sim->shellB.x = (int) x;
    sim->shellB.y = (int) y;
    
    // draw shell
    drawBattleShell(renderer, window, sim);
    if (time > getTimeToTarget(y, vy))
      break;
    else
    {
      time += 0.1f; // Adjust time
    }
  }
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
    int battleshipType = 1;
    doRender(renderer, &simState, battleshipType); // the 3rd arg is the battleship type (using ints for now)
    while (simState.escortA.state == 1 && simState.escortB.state == 1 && simState.escortC.state == 1 && simState.escortD.state == 1 && simState.escortE.state == 1)
    {
      fireBattleShell(renderer, window, &simState, battleshipType, simState.escortA.x, simState.escortA.y);
      fireBattleShell(renderer, window, &simState, battleshipType, simState.escortB.x, simState.escortB.y);
      fireBattleShell(renderer, window, &simState, battleshipType, simState.escortC.x, simState.escortC.y);
      fireBattleShell(renderer, window, &simState, battleshipType, simState.escortD.x, simState.escortD.y);
      fireBattleShell(renderer, window, &simState, battleshipType, simState.escortE.x, simState.escortE.y);
    }
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

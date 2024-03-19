#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
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
  SDL_Texture *texE;// texture of escort ship
  int angle;
  float v;
} EscortShip;

typedef struct
{
  int x, y;         // position
  float angle;      // max vertical angle of shell movement
  float v;          // max (initial velocity) of battle ship
  SDL_Texture *texB;// texture of battleship
  char *name;
  char* timeActive;
  short state;
} Battleship;

typedef struct
{
  int x, y;         // position
  SDL_Texture *texS;// texture of shell
} Shell;

typedef struct
{
  /*Main Sprites*/
  // Escorts
  EscortShip escortA, escortB, escortC, escortD, escortE;   // you could also use an array to draw these, but i'm an idiot so i did this
  // Battleships
  Battleship battleU, battleM, battleR, battleS;  // all of these types would use the same sprite
  // Shell
  Shell shellBEA, shellBEB, shellBEC, shellBED, shellBEE, shellEA, shellEB, shellEC, shellED, shellEE; // same sprite used to render these shells
} SimState;

void cleanup (SDL_Window* window, SDL_Renderer* renderer) // cleans window and renderer to prevent memory leaks
{
   // Cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void loadSim (SimState *sim, SDL_Window *window, SDL_Renderer *renderer) // loads sprites of battlship and all escorts
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
  
  // Load battleship image into surfaces
  // U
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
  
  // check if texture is NULL, error handling for when texture failed to be created
  if (sim->battleU.texB == NULL)
  {
    printf("Failed to create battleshipU texture from surface: %s\n", SDL_GetError());
    cleanup(window, renderer);
    exit(1); // exit the program
  }

  // M
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
 
  // R
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
 
  // S
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


  // Load escortships' images into surfaces
  // A
  escortASurface = IMG_Load("resources/escortshipA.png");
  if (escortASurface == NULL)
  {
    printf("Cannot find escortshipA.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }
  // TODO: add errror handling for all escorts for when texture fails to be made from surface
  // B
  escortBSurface = IMG_Load("resources/escortshipB.png");
  if (escortBSurface == NULL)
  {
    printf("Cannot find escortshipB.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  // C
  escortCSurface = IMG_Load("resources/escortshipC.png");
  if (escortCSurface == NULL)
  {
    printf("Cannot find escortshipC.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  // D
  escortDSurface = IMG_Load("resources/escortshipD.png");
  if (escortDSurface == NULL)
  {
    printf("Cannot find escortshipD.png!\n\n");
    cleanup(window, renderer);
    exit(1);
  }

  // E
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
  // U
  sim->battleU.x = 50;
  sim->battleU.y = WINDOW_SIZE - (64+50);

  // M
  sim->battleM.x = 50;
  sim->battleM.y = WINDOW_SIZE - (64+50);

  // R
  sim->battleR.x = 50;
  sim->battleR.y = WINDOW_SIZE - (64+50);

  // S
  sim->battleS.x = 50;
  sim->battleS.y = WINDOW_SIZE - (64+50);


  // x and y positions of escorts (positions are random within the window)
  // A
  sim->escortA.x = rand() % (WINDOW_SIZE - 64);
  sim->escortA.y = rand() % (WINDOW_SIZE - 64);

  // B
  sim->escortB.x = rand() % (WINDOW_SIZE - 64);
  sim->escortB.y = rand() % (WINDOW_SIZE - 64);

  // C
  sim->escortC.x = rand() % (WINDOW_SIZE - 64);
  sim->escortC.y = rand() % (WINDOW_SIZE - 64);
  
  // D
  sim->escortD.x = rand() % (WINDOW_SIZE - 64);
  sim->escortD.y = rand() % (WINDOW_SIZE - 64);
  
  // E
  sim->escortE.x = rand() % (WINDOW_SIZE - 64);
  sim->escortE.y = rand() % (WINDOW_SIZE - 64);

  // v of battleships
  sim->battleU.v = 35;    // U
  sim->battleM.v = 35;    // M
  sim->battleR.v = 35;    // R
  sim->battleS.v = 35;    // S

  // v of escorts
  sim->escortA.v = 35 * 1.2; //A
  sim->escortB.v = rand() % 35;
  sim->escortC.v = rand() % 35;
  sim->escortD.v = rand() % 35;
  sim->escortE.v = rand() % 35;

  // state of escorts (1 means alive, 0 is when its destroyed)
  sim->escortA.state = 1; // A
  sim->escortB.state = 1; // B
  sim->escortC.state = 1; // C
  sim->escortD.state = 1; // D
  sim->escortE.state = 1; // E
  
  // state of battlships (1 = alive, 0 = destroyed) 
}

void drawEscortShellA (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)   // renders shellE
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
  sim->shellEA.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellEA.x = sim->escortA.x;
  sim->shellEA.y = sim->escortA.y;

  SDL_Rect shellEARect = { sim->shellEA.x, sim->shellEA.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellEA.texS, NULL, &shellEARect);
  SDL_RenderPresent(renderer);
}

void drawEscortShellB (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)   // renders shellEB
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
  sim->shellEB.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellEB.x = sim->escortB.x;
  sim->shellEB.y = sim->escortB.y;

  SDL_Rect shellEBRect = { sim->shellEB.x, sim->shellEB.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellEB.texS, NULL, &shellEBRect);
  SDL_RenderPresent(renderer);
}

void drawEscortShellC (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)   // renders shellEC
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
  sim->shellEC.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellEC.x = sim->escortC.x;
  sim->shellEC.y = sim->escortC.y;

  SDL_Rect shellECRect = { sim->shellEC.x, sim->shellEC.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellEC.texS, NULL, &shellECRect);
  SDL_RenderPresent(renderer);
}

void drawEscortShellD (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)   // renders shellED
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
  sim->shellED.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellED.x = sim->escortD.x;
  sim->shellED.y = sim->escortD.y;

  SDL_Rect shellEDRect = { sim->shellED.x, sim->shellED.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellED.texS, NULL, &shellEDRect);
  SDL_RenderPresent(renderer);
}

void drawEscortShellE (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)   // renders shellEE
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
  sim->shellEE.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellEE.x = sim->escortE.x;
  sim->shellEE.y = sim->escortE.y;

  SDL_Rect shellEERect = { sim->shellEE.x, sim->shellEE.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellEE.texS, NULL, &shellEERect);
  SDL_RenderPresent(renderer);
}



void drawBattleShellA (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)   // renders shellBEA
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
  sim->shellBEA.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellBEA.x = 50;
  sim->shellBEA.y = WINDOW_SIZE - (64+50);

  SDL_Rect shellBEARect = { sim->shellBEA.x, sim->shellBEA.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellBEA.texS, NULL, &shellBEARect);
  SDL_RenderPresent(renderer);
}

void drawBattleShellB (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)
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
  sim->shellBEB.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellBEB.x = 50;
  sim->shellBEB.y = WINDOW_SIZE - (64+50);

  SDL_Rect shellBEBRect = { sim->shellBEB.x, sim->shellBEB.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellBEB.texS, NULL, &shellBEBRect);
  SDL_RenderPresent(renderer);
}

void drawBattleShellC (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)
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
  sim->shellBEC.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellBEC.x = 50;
  sim->shellBEC.y = WINDOW_SIZE - (64+50);

  SDL_Rect shellBECRect = { sim->shellBEC.x, sim->shellBEC.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellBEC.texS, NULL, &shellBECRect);
  SDL_RenderPresent(renderer);
}

void drawBattleShellD (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)
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
  sim->shellBED.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellBED.x = 50;
  sim->shellBED.y = WINDOW_SIZE - (64+50);

  SDL_Rect shellBEDRect = { sim->shellBED.x, sim->shellBED.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellBED.texS, NULL, &shellBEDRect);
  SDL_RenderPresent(renderer);
}

void drawBattleShellE (SDL_Renderer *renderer, SDL_Window *window, SimState *sim)
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
  sim->shellBEE.texS = SDL_CreateTextureFromSurface(renderer, shellSurface);
  SDL_FreeSurface(shellSurface);

  // x and y positions of the shell at initial point
  sim->shellBEE.x = 50;
  sim->shellBEE.y = WINDOW_SIZE - (64+50);

  SDL_Rect shellBEERect = { sim->shellBEE.x, sim->shellBEE.y, 10, 10};
  SDL_RenderCopy(renderer, sim->shellBEE.texS, NULL, &shellBEERect);
  SDL_RenderPresent(renderer);
}


bool checkCollisionEscortA(SimState *sim)     // collision detection for shell and escortA
{
  // calculate corner coordinates
  int escortTopRightX = sim->escortA.x;
  int escortTopRightY = sim->escortA.y;
  int escortBotLeftX = sim->escortA.x + (64-1);
  int escortBotLeftY = sim->escortA.y + (64-1);

  int shellTopRightX = sim->shellBEA.x;
  int shellTopRightY = sim->shellBEA.y;
  int shellBotLeftX = sim->shellBEA.x + (10-1);
  int shellBotLeftY = sim->shellBEA.y + (10-1);

  // check for collision
  if ((shellTopRightX <= escortBotLeftX) &&
      (shellBotLeftX >= escortTopRightX) &&
      (shellTopRightY <= escortBotLeftY) &&
      (shellBotLeftY >= escortTopRightY))
  {
    printf("collision is true\n");
    return true;
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

  int shellTopRightX = sim->shellBEB.x;
  int shellTopRightY = sim->shellBEB.y;
  int shellBotLeftX = sim->shellBEB.x + (10-1);
  int shellBotLeftY = sim->shellBEB.y + (10-1);

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

  int shellTopRightX = sim->shellBEC.x;
  int shellTopRightY = sim->shellBEC.y;
  int shellBotLeftX = sim->shellBEC.x + (10-1);
  int shellBotLeftY = sim->shellBEC.y + (10-1);

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

  int shellTopRightX = sim->shellBED.x;
  int shellTopRightY = sim->shellBED.y;
  int shellBotLeftX = sim->shellBED.x + (10-1);
  int shellBotLeftY = sim->shellBED.y + (10-1);

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

  int shellTopRightX = sim->shellBEE.x;
  int shellTopRightY = sim->shellBEE.y;
  int shellBotLeftX = sim->shellBEE.x + (10-1);
  int shellBotLeftY = sim->shellBEE.y + (10-1);

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

float getRangeB (float v)
{
  const float angle = PI/4;
  // printf("return value of range: %f\n", (pow(v, 2) * sin(2 * angle)) / G);
  return (pow(v, 2) * sin(2 * angle)) / G;
}

float getTimeToTarget (float y, float vy) // this isn't executed at all !
{
  // printf("return value of time to tar: %f\n", 2 * vy / G);
  return 2 * vy / G;
}

// Pray to god that it works
void fireShellToEscortA (SDL_Renderer *renderer, SDL_Window *window, SimState *sim, int battleshipType)
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
  
  float target_x = sim->escortA.x - 32;
  float target_y = sim->escortA.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double dist = sqrt(pow(dx, 2) + pow(dy, 2));
  // printf("distance: %lf\n", dist); // remove this after
  
  double vx = fabs(dx) * v / dist;
  double vy = fabs(dy) * v / dist;
  vx = fabs(vx);
  vy = fabs(vy);
  //printf("vx: %lf\n", vx);
  //printf("vy: %lf\n", vy);

  float x = battleshipX;
  float y = battleshipY;
  // printf("x: %f\n", x);
  // printf("y: %f\n", y);
  float time = 0;
  printf("shell position: (%f, %f)", x, y);  
  while (y > 0 && dist <= getRangeB(v))
  {
    float dt = 0.2f;
    // Update position
    x += vx / 60; // * //dt;
    y += vy / 60; // * //dt - 0.5f * G * pow(dt, 2);
    // check for collision
    if (checkCollisionEscortA(sim))
    {
      sim->escortA.state = 0; // that means destroyed
      printf("escortA destroyed!\n");
      SDL_DestroyTexture(sim->escortA.texE);
      break;
    }

      x += 
      sim->shellBEA.x = (int) x;
      sim->shellBEA.y = (int) y;
      printf("shell position: (%f, %f)", x, y);
      if (checkCollisionEscortA(sim))
    {
      sim->escortA.state = 0; // that means destroyed
      printf("escortA destroyed!\n");
      SDL_DestroyTexture(sim->escortA.texE);
      break;
    }
      // draw shell
      drawBattleShellA(renderer, window, sim);
      SDL_Delay(1000/60);
      if (time > getTimeToTarget(battleshipY, vy))
        break;
      else
        time += dt; // Adjust time
  }
}

void fireShellToEscortB (SDL_Renderer *renderer, SDL_Window *window, SimState *sim, int battleshipType)
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
  
  float target_x = sim->escortB.x - 32;
  float target_y = sim->escortB.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double dist = sqrt(pow(dx, 2) + pow(dy, 2));
  // printf("distance: %lf\n", dist); // remove this after
  
  double vx = fabs(dx) * v / dist;
  double vy = fabs(dy) * v / dist;
  vx = fabs(vx);
  vy = fabs(vy);
  //printf("vx: %lf\n", vx);
  //printf("vy: %lf\n", vy);

  float x = battleshipX;
  float y = battleshipY;
  // printf("x: %f\n", x);
  // printf("y: %f\n", y);
  float time = 0;
  printf("shell position: (%f, %f)", x, y);
  while (y > 0 && dist <= getRangeB(v))
  {
    float dt = 0.2f;
    // Update position
    x += vx * dt;
    y += vy * dt - 0.5f * G * pow(dt, 2);
    // check for collision
    if (checkCollisionEscortB(sim))
    {
      sim->escortB.state = 0; // that means destroyed

      printf("escortB destroyed!\n");
      SDL_DestroyTexture(sim->escortB.texE);
      break;
    }
      sim->shellBEB.x = (int) x;
      sim->shellBEB.y = (int) y;
      printf("shell position: (%f, %f)", x, y);
      if (checkCollisionEscortB(sim))
    {
      sim->escortB.state = 0; // that means destroyed

      printf("escortB destroyed!\n");
      SDL_DestroyTexture(sim->escortB.texE);
      break;
    }
      // draw shell
      drawBattleShellB(renderer, window, sim);
      if (time > getTimeToTarget(battleshipY, vy))
        break;
      else
        time += dt; // Adjust time
  }
} 

void fireShellToEscortC (SDL_Renderer *renderer, SDL_Window *window, SimState *sim, int battleshipType)
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
  
  float target_x = sim->escortC.x - 32;
  float target_y = sim->escortC.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double dist = sqrt(pow(dx, 2) + pow(dy, 2));
  // printf("distance: %lf\n", dist); // remove this after
  
  double vx = fabs(dx) * v / dist;
  double vy = fabs(dy) * v / dist;
  vx = fabs(vx);
  vy = fabs(vy);
  //printf("vx: %lf\n", vx);
  //printf("vy: %lf\n", vy);

  float x = battleshipX;
  float y = battleshipY;
  // printf("x: %f\n", x);
  // printf("y: %f\n", y);
  float time = 0;
  printf("shell position: (%f, %f)", x, y); 
  while (y > 0 && dist <= getRangeB(v))
  {
    float dt = 0.2f;
    // Update position
    x += vx * dt;
    y += vy * dt - 0.5f * G * pow(dt, 2);
    // check for collision
    if (checkCollisionEscortC(sim))
    {
      sim->escortC.state = 0; // that means destroyed
      
      printf("escortC destroyed!\n");
      SDL_DestroyTexture(sim->escortC.texE);
      break;
    }
      sim->shellBEC.x = (int) x;
      sim->shellBEC.y = (int) y;
      printf("shell position: (%f, %f)", x, y); 
      if (checkCollisionEscortC(sim))
    {
      sim->escortC.state = 0; // that means destroyed
      
      printf("escortC destroyed!\n");
      SDL_DestroyTexture(sim->escortC.texE);
      break;
    }
      // draw shell
      drawBattleShellC(renderer, window, sim);
      if (time > getTimeToTarget(battleshipY, vy))
        break;
      else
        time += dt; // Adjust time
  }
} 

void fireShellToEscortD (SDL_Renderer *renderer, SDL_Window *window, SimState *sim, int battleshipType)
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
  
  float target_x = sim->escortD.x - 32;
  float target_y = sim->escortD.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double dist = sqrt(pow(dx, 2) + pow(dy, 2));
  // printf("distance: %lf\n", dist); // remove this after
  
  double vx = fabs(dx) * v / dist;
  double vy = fabs(dy) * v / dist;
  vx = fabs(vx);
  vy = fabs(vy);
  //printf("vx: %lf\n", vx);
  //printf("vy: %lf\n", vy);

  float x = battleshipX;
  float y = battleshipY;
  // printf("x: %f\n", x);
  // printf("y: %f\n", y);
  float time = 0;
  printf("shell position: (%f, %f)", x, y); 
  while (y > 0 && dist <= getRangeB(v))
  {
    float dt = 0.2f;
    // Update position
    x += vx * dt;
    y += vy * dt - 0.5f * G * pow(dt, 2);
    // check for collision
    if (checkCollisionEscortD(sim))
    {
      sim->escortD.state = 0; // that means destroyed 
      
      printf("escortD destroyed!\n");
      SDL_DestroyTexture(sim->escortD.texE);
      break;
    }
      sim->shellBED.x = (int) x;
      sim->shellBED.y = (int) y;
      printf("shell position: (%f, %f)", x, y); 
      if (checkCollisionEscortD(sim))
    {
      sim->escortD.state = 0; // that means destroyed 
      
      printf("escortD destroyed!\n");
      SDL_DestroyTexture(sim->escortD.texE);
      break;
    }
      // draw shell
      drawBattleShellD(renderer, window, sim);
      if (time > getTimeToTarget(battleshipY, vy))
      {
        break;
      }
      else
      {
        time += dt; // Adjust time
      }
  }
} 

void fireShellToEscortE (SDL_Renderer *renderer, SDL_Window *window, SimState *sim, int battleshipType)
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
  
  float target_x = sim->escortE.x - 32;
  float target_y = sim->escortE.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double dist = sqrt(pow(dx, 2) + pow(dy, 2));
  // printf("distance: %lf\n", dist); // remove this after
  
  double vx = fabs(dx) * v / dist;
  double vy = fabs(dy) * v / dist;
  vx = fabs(vx);
  vy = fabs(vy);
  //printf("vx: %lf\n", vx);
  //printf("vy: %lf\n", vy);

  float x = battleshipX;
  float y = battleshipY;
  // printf("x: %f\n", x);
  // printf("y: %f\n", y);
  float time = 0;
  printf("shell position: (%f, %f)", x, y); 
  while (y > 0 && dist <= getRangeB(v))
  {
    float dt = 0.2f;
    // Update position
    x += vx * dt;
    y += vy * dt - 0.5f * G * pow(dt, 2);
    // check for collision
    if (checkCollisionEscortE(sim))
    {
      sim->escortE.state = 0; // that means destroyed 
      printf("escortE destroyed!\n");
      SDL_DestroyTexture(sim->escortE.texE);
      break;
    }
      sim->shellBEE.x = (int) x;
      sim->shellBEE.y = (int) y;
      printf("shell position: (%f, %f)", x, y); 
      if (checkCollisionEscortE(sim))
    {
      sim->escortE.state = 0; // that means destroyed 
      printf("escortE destroyed!\n");
      SDL_DestroyTexture(sim->escortE.texE);
      break;
    }
      // draw shell
      drawBattleShellE(renderer, window, sim);
      if (time > getTimeToTarget(battleshipY, vy))
        break;
      else
        time += dt; // Adjust time
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

    fireShellToEscortA(renderer, window, &simState, battleshipType);
    fireShellToEscortB(renderer, window, &simState, battleshipType);
    fireShellToEscortC(renderer, window, &simState, battleshipType);
    fireShellToEscortD(renderer, window, &simState, battleshipType);
    fireShellToEscortE(renderer, window, &simState, battleshipType);
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

  SDL_DestroyTexture(simState.shellBEA.texS);
  SDL_DestroyTexture(simState.shellBEB.texS);
  SDL_DestroyTexture(simState.shellBEC.texS);
  SDL_DestroyTexture(simState.shellBED.texS);
  SDL_DestroyTexture(simState.shellBEE.texS);
  cleanup(window, renderer);
  return 0;
}

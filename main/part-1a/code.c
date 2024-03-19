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
  float maxAngle;
  float minAngle;
  float angleR;
  float v;
  int type;       // type of escort A,B,C,D,E (denoted with numbers 1-5)
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
  /*Main Sprites*/
  // Escorts
  EscortShip escortA, escortB, escortC, escortD, escortE;   // you could also use an array to draw these, but i'm an idiot so i did this
  // Battleships
  Battleship battleU, battleM, battleR, battleS;  // all of these types would use the same sprite
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
  sim->battleU.state = 1;    // U
  sim->battleM.state = 1;    // M
  sim->battleR.state = 1;    // R
  sim->battleS.state = 1;    // S

  // angle of battleships
  sim->battleU.angle = PI / 4;    // U
  sim->battleM.angle = PI / 4;    // M
  sim->battleR.angle = PI / 4;    // R
  sim->battleS.angle = PI / 4;    // S

  // Angle range of escorts
  sim->escortA.angleR = 20; // A
  sim->escortB.angleR = 30; // B
  sim->escortC.angleR = 25; // C
  sim->escortD.angleR = 50; // D
  sim->escortE.angleR = 70; // E
  
  // Min angle of escorts PI / 9 = 20deg
  sim->escortA.minAngle = (rand() % 20) * (PI / 180);  // A 
  sim->escortB.minAngle = (rand() % 20) * (PI / 180);  // B
  sim->escortC.minAngle = (rand() % 20) * (PI / 180);  // C
  sim->escortD.minAngle = (rand() % 20) * (PI / 180);  // D 
  sim->escortE.minAngle = (rand() % 20) * (PI / 180);  // E
  
  // Max angle of escorts (the max angle will be smaller than 90)
  sim->escortA.maxAngle = sim->escortA.minAngle + sim->escortA.angleR;
  sim->escortB.maxAngle = sim->escortB.minAngle + sim->escortB.angleR; 
  sim->escortC.maxAngle = sim->escortC.minAngle + sim->escortC.angleR;  
  sim->escortD.maxAngle = sim->escortD.minAngle + sim->escortD.angleR;  
  sim->escortE.maxAngle = sim->escortE.minAngle + sim->escortE.angleR; 

  // Type of escorts
  sim->escortA.type = 1; // A
  sim->escortB.type = 2; // B
  sim->escortC.type = 3; // C
  sim->escortD.type = 4; // D
  sim->escortE.type = 5; // E
}
 

float getRange (float v, float angle)
{
  // printf("return value of range: %f\n", (pow(v, 2) * sin(2 * angle)) / G);
  return (pow(v, 2) * sin(2 * angle)) / G;
}

float getTimeToTarget (float v) // this isn't executed at all !
{
  // printf("return value of time to tar: %f\n", 2 * vy / G);
  return 2 * (v / G);
}

float getRangeEscort(float v, float maxAngle, float minAngle) {
    float maxRange = (pow(v, 2) * sin(2 * maxAngle)) / G;
    float minRange = (pow(v, 2) * sin(2 * minAngle)) / G;
    return fabs(maxRange - minRange); // Return absolute difference to ensure positive range
}

void impactB (SimState *sim, int escortType, int battleshipType, short *battleshipState) // include this in struct
{
  float escortshipX = 0;
  float escortshipY = 0;
  float v, maxAngle, minAngle;
  switch(escortType)
  {
    case 1:
      escortshipX = sim->escortA.x;
      escortshipY = sim->escortA.y;
      v = sim->escortA.v;
      maxAngle = sim->escortA.maxAngle;
      minAngle = sim->escortA.minAngle;
    break;
    case 2:
      escortshipX = sim->escortB.x;
      escortshipY = sim->escortB.y;
      v = sim->escortB.v;
      maxAngle = sim->escortB.maxAngle;
      minAngle = sim->escortB.minAngle;
    break;
    case 3:
      escortshipX = sim->escortC.x;
      escortshipY = sim->escortC.y;
      v = sim->escortB.v;
      maxAngle = sim->escortC.maxAngle;
      minAngle = sim->escortC.minAngle;
    break;
    case 4:
      escortshipX = sim->escortD.x;
      escortshipY = sim->escortD.y;
      v = sim->escortD.v;
      maxAngle = sim->escortD.maxAngle;
      minAngle = sim->escortD.minAngle;
    break;
    case 5:
      escortshipX = sim->escortE.x;
      escortshipY = sim->escortE.y;
      v = sim->escortE.v;
      maxAngle = sim->escortE.maxAngle;
      minAngle = sim->escortE.minAngle;
    break;
  }

  float target_x = 0;
  float target_y = 0;
  SDL_Texture *battleshipTex;
  switch(battleshipType)
  {
    case 1:
      target_x = sim->battleU.x - 32;
      target_y = sim->battleU.y -32;
      battleshipTex = sim->battleU.texB;
    break;
    case 2:
      target_x = sim->battleM.x -32; 
      target_y = sim->battleM.y -32;
      battleshipTex = sim->battleM.texB;
    break;
    case 3:
      target_x = sim->battleR.x - 32;
      target_y = sim->battleR.y - 32;
      battleshipTex = sim->battleR.texB;
    break;
    case 4:
      target_x = sim->battleS.x - 32;
      target_y = sim->battleS.y - 32;
      battleshipTex = sim->battleR.texB;
    break;
  }
  double dx = target_x - escortshipX;
  double dy = target_y - escortshipY;
  double distance = sqrt(pow(dx, 2) + pow(dy, 2));
  if (distance > getRangeEscort(v, maxAngle, minAngle))
  {
    *battleshipState = 0;
    //printf("BState: %hd\n", *battleshipState);
    printf("battleship destroyed!\n");
    SDL_DestroyTexture(battleshipTex);
  }
}


// Pray to god that it works
void impactEA (SimState *sim, int battleshipType)
{
  float battleshipX = 0;
  float battleshipY = 0;
  float v, angle;
  switch(battleshipType)
  {
    case 1:
      battleshipX = sim->battleU.x;
      battleshipY = sim->battleU.y;
      v = sim->battleU.v;
      angle = sim->battleU.angle;
    break;
    case 2:
      battleshipX = sim->battleM.x; 
      battleshipY = sim->battleM.y;
      v = sim->battleM.v;
      angle = sim->battleM.angle;
    break;
    case 3:
      battleshipX = sim->battleR.x;
      battleshipY = sim->battleR.y;
      v = sim->battleR.v;
      angle = sim->battleR.angle;
    break;
    case 4:
      battleshipX = sim->battleS.x;
      battleshipY = sim->battleS.y;
      v = sim->battleS.v;
      angle = sim->battleS.angle;
    break;
  }

  float target_x = sim->escortA.x - 32;
  float target_y = sim->escortA.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double distance = sqrt(pow(dx, 2) + pow(dy, 2));
  if (distance < getRange(v, angle))
  {
    sim->escortA.state = 0; // that means destroyed
    printf("escortA destroyed!\n");
    SDL_DestroyTexture(sim->escortA.texE);
  }
}

void impactEB (SimState *sim, int battleshipType)
{
  float battleshipX = 0;
  float battleshipY = 0;
  float v, angle;
  switch(battleshipType)
  {
    case 1:
      battleshipX = sim->battleU.x;
      battleshipY = sim->battleU.y;
      v = sim->battleU.v;
      angle = sim->battleU.angle;
    break;
    case 2:
      battleshipX = sim->battleM.x; 
      battleshipY = sim->battleM.y;
      v = sim->battleM.v;
      angle = sim->battleM.angle;
    break;
    case 3:
      battleshipX = sim->battleR.x;
      battleshipY = sim->battleR.y;
      v = sim->battleR.v;
      angle = sim->battleR.angle;
    break;
    case 4:
      battleshipX = sim->battleS.x;
      battleshipY = sim->battleS.y;
      v = sim->battleS.v;
      angle = sim->battleS.angle;
    break;
  }

  float target_x = sim->escortB.x - 32;
  float target_y = sim->escortB.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double distance = sqrt(pow(dx, 2) + pow(dy, 2));
  if (distance < getRange(v, angle))
  {
    sim->escortB.state = 0; // that means destroyed
    printf("escortB destroyed!\n");
    SDL_DestroyTexture(sim->escortB.texE);
  }
}

void impactEC (SimState *sim, int battleshipType)
{
  float battleshipX = 0;
  float battleshipY = 0;
  float v, angle;
  switch(battleshipType)
  {
    case 1:
      battleshipX = sim->battleU.x;
      battleshipY = sim->battleU.y;
      v = sim->battleU.v;
      angle = sim->battleU.angle;
    break;
    case 2:
      battleshipX = sim->battleM.x; 
      battleshipY = sim->battleM.y;
      v = sim->battleM.v;
      angle = sim->battleM.angle;
    break;
    case 3:
      battleshipX = sim->battleR.x;
      battleshipY = sim->battleR.y;
      v = sim->battleR.v;
      angle = sim->battleR.angle;
    break;
    case 4:
      battleshipX = sim->battleS.x;
      battleshipY = sim->battleS.y;
      v = sim->battleS.v;
      angle = sim->battleS.angle;
    break;
  }

  float target_x = sim->escortC.x - 32;
  float target_y = sim->escortC.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double distance = sqrt(pow(dx, 2) + pow(dy, 2));
  if (distance < getRange(v, angle))
  {
    sim->escortC.state = 0; // that means destroyed
    printf("escortC destroyed!\n");
    SDL_DestroyTexture(sim->escortC.texE);
  }
}

void impactED (SimState *sim, int battleshipType)
{
  float battleshipX = 0;
  float battleshipY = 0;
  float v, angle;
  switch(battleshipType)
  {
    case 1:
      battleshipX = sim->battleU.x;
      battleshipY = sim->battleU.y;
      v = sim->battleU.v;
      angle = sim->battleU.angle;
    break;
    case 2:
      battleshipX = sim->battleM.x; 
      battleshipY = sim->battleM.y;
      v = sim->battleM.v;
      angle = sim->battleM.angle;
    break;
    case 3:
      battleshipX = sim->battleR.x;
      battleshipY = sim->battleR.y;
      v = sim->battleR.v;
      angle = sim->battleR.angle;
    break;
    case 4:
      battleshipX = sim->battleS.x;
      battleshipY = sim->battleS.y;
      v = sim->battleS.v;
      angle = sim->battleS.angle;
    break;
  }

  float target_x = sim->escortD.x - 32;
  float target_y = sim->escortD.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double distance = sqrt(pow(dx, 2) + pow(dy, 2));
  if (distance < getRange(v, angle))
  {
    sim->escortD.state = 0; // that means destroyed
    printf("escortD destroyed!\n");
    SDL_DestroyTexture(sim->escortD.texE);
  }
}

void impactEE (SimState *sim, int battleshipType)
{
  float battleshipX = 0;
  float battleshipY = 0;
  float v, angle;
  switch(battleshipType)
  {
    case 1:
      battleshipX = sim->battleU.x;
      battleshipY = sim->battleU.y;
      v = sim->battleU.v;
      angle = sim->battleU.angle;
    break;
    case 2:
      battleshipX = sim->battleM.x; 
      battleshipY = sim->battleM.y;
      v = sim->battleM.v;
      angle = sim->battleM.angle;
    break;
    case 3:
      battleshipX = sim->battleR.x;
      battleshipY = sim->battleR.y;
      v = sim->battleR.v;
      angle = sim->battleR.angle;
    break;
    case 4:
      battleshipX = sim->battleS.x;
      battleshipY = sim->battleS.y;
      v = sim->battleS.v;
      angle = sim->battleS.angle;
    break;
  }

  float target_x = sim->escortE.x - 32;
  float target_y = sim->escortE.y - 32;
  
  double dx = target_x - battleshipX;
  double dy = target_y - battleshipY;
  double distance = sqrt(pow(dx, 2) + pow(dy, 2));
  if (distance < getRange(v, angle))
  {
    sim->escortE.state = 0; // that means destroyed
    printf("escortE destroyed!\n");
    SDL_DestroyTexture(sim->escortE.texE);
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


void playSimulation (int battleshipType)
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

  short *battleshipState;
  switch(battleshipType)
  {
    case 1:
      battleshipState = &simState.battleU.state; 
    break;
    case 2:
      battleshipState = &simState.battleM.state; 
    break;
    case 3:
      battleshipState = &simState.battleR.state; 
    break;
    case 4:
      battleshipState = &simState.battleR.state; 
    break;
  } 

  // Running events
  int done = 0;
  while (!done)
  {  
    // Check for events
    done = processEvents(window, &simState);
    //int battleshipType = 1;
    doRender(renderer, &simState, battleshipType); // the 3rd arg is the battleship type (using ints for now)
    impactEA(&simState, battleshipType);
    impactEB(&simState, battleshipType);
    impactEC(&simState, battleshipType);
    impactED(&simState, battleshipType);
    impactEE(&simState, battleshipType);
    impactB(&simState, simState.escortA.type, battleshipType, battleshipState);
    impactB(&simState, simState.escortB.type, battleshipType, battleshipState);
    impactB(&simState, simState.escortC.type, battleshipType, battleshipState);
    impactB(&simState, simState.escortD.type, battleshipType, battleshipState);
    impactB(&simState, simState.escortE.type, battleshipType, battleshipState);
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

  cleanup(window, renderer);
}

int setup() {
    int setupChoice;
    int battleshipType;

    do {
        printf("\nSetup\n");
        printf("1. Battleship Settings\n");
        printf("2. Escorts in simulation\n");
        printf("3. Back to Start Simulation Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &setupChoice);

        switch(setupChoice) {
            case 1:
                printf("Battleship settings...\n");
                printf("\nChoose your desired Battleship\n");
                printf("1. USS Iowa (BB-61)\n");
                printf("2. MS King George V\n");
                printf("3. Richelieu\n");
                printf("4. Sovetsky Soyuz-class\n");
                printf("Enter your choice: ");
                scanf("%d", &battleshipType);
                printf("\nChosen type: %d\n", battleshipType);
                while (battleshipType < 0 || battleshipType >= 5)
                {
                  printf("Number entered is not valid, number must be between 1-4\n");
                  printf("Enter your choice: ");
                  scanf("%d", &battleshipType);
                  printf("\nChosen type: %d\n", battleshipType);
                }
                break;
            case 2:
                printf("Escortship settings...\n");
                break;
            case 3:
                printf("Returning to start simulation menu...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 3.\n");
        }
    } while(setupChoice != 3);
  return battleshipType;
}

void startSimulation() 
{
  int subChoice;
  int battleshipType;

  do {
    printf("\nStart Simulation\n");
    printf("1. Setup\n");
    printf("2. Show Simulation\n");
    printf("3. Back to Main Menu\n");
    printf("Please note that you have to select a battleship before running the simulation\n");
    printf("Enter your choice: ");
    scanf("%d", &subChoice);

    switch(subChoice) {
        case 1:
            printf("Setup selected.\n");
            battleshipType = setup();
            printf("Chosen type: %d\n", battleshipType);
            break;
        case 2:
            printf("Opening simulation...\n"); 
            playSimulation(battleshipType);
            break;
        case 3:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 3.\n");
    }
  } while (subChoice != 3);
}

void viewInstructions() {
  printf("\nInstructions:\n");
  // Print instructions here
}

void simulationStats() {
  printf("\nSimulation Stats:\n");
  // Print simulation statistics here
}

void mainMenu() 
{
  int choice;

  do {
    printf("\nMain Menu\n");
    printf("1. Start Simulation\n");
    printf("2. View Instructions\n");
    printf("3. Simulation Stats\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            startSimulation();
            break;
        case 2:
            viewInstructions();
            break;
        case 3:
            simulationStats();
            break;
        case 4:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
    }
  } while (choice != 4);
}

int main (void) 
{
  mainMenu(); 
  return 0;
}

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"

// types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;


#define CGL_ARRAY_WIDTH           200
#define CGL_ARRAY_HEIGHT          200
#define CGL_CELL_WIDTH            2
#define CGL_CELL_HEIGHT           2
#define CGL_SCREEN_WIDTH          (CGL_ARRAY_WIDTH * CGL_CELL_WIDTH)
#define CGL_SCREEN_HEIGHT         (CGL_ARRAY_HEIGHT * CGL_CELL_HEIGHT) + CGL_HELP_HEIGHT
#define CGL_HELP_HEIGHT           60

#define CGL_LIVE_CELLS            50

// Colors: https://www.raylib.com/cheatsheet/cheatsheet.html
#define CGL_BACKGROUND_COLOR      BLACK
#define CGL_HELP_BACKGROUND_COLOR (Color){140, 165, 140, 255}
#define CGL_HELP_FONT_COLOR       BLACK

#define CGL_COLOR_00              GREEN
#define CGL_COLOR_01              BLUE
#define CGL_COLOR_02              YELLOW
#define CGL_COLOR_03              RED
#define CGL_COLOR_COUNT           4

#define CGL_TITLE                 "Conway's Game of Life"
#define CGL_FONT                  "fonts/LiberationMono-Regular.ttf"
#define CGL_FPS                   30


typedef struct  {
  u8 (*array)[CGL_ARRAY_HEIGHT + 2];
  Font font;
  bool running;
  Color cell_color_array[CGL_COLOR_COUNT];
  int cell_color_index;
} App;


void shuffle(u8 *array) {

  int min = 0;
  int max = CGL_ARRAY_WIDTH;

  for (int i = CGL_ARRAY_WIDTH - 1; i > 0; i--) {
    int j = min + (int)rand() % (max - min);
    u8 temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }

}

void init_board(u8 (*array)[CGL_ARRAY_WIDTH + 2]) {

  for (int i = 1; i < CGL_ARRAY_HEIGHT + 1; i++) {
    for (int j = 1; j < CGL_ARRAY_WIDTH + 1; j++) {
      array[i][j] = (j < CGL_LIVE_CELLS) ? 1 : 0;
    }
    shuffle(&array[i][1]);
  }

}

void next_generation(App *app) {

  u8 neighbors[CGL_ARRAY_HEIGHT + 2][CGL_ARRAY_WIDTH + 2];

  for (int i = 1; i < CGL_ARRAY_HEIGHT + 1; i++) {
    for (int j = 1; j < CGL_ARRAY_WIDTH + 1; j++) {
      neighbors[i][j] =
        // top-left
        app->array[i-1][j-1] +
        // top-middle
        app->array[i-1][j] +
        // top-right
        app->array[i-1][j+1] +
        // left
        app->array[i][j-1] +
        // right
        app->array[i][j+1] +
        // bottom-left
        app->array[i+1][j-1] +
        // bottom-middle
        app->array[i+1][j] +
        // bottom-right
        app->array[i+1][j+1];
    }
  }

  for (int i = 1; i < CGL_ARRAY_HEIGHT + 1; i++) {
    for (int j = 1; j < CGL_ARRAY_WIDTH + 1; j++) {
      switch(neighbors[i][j]) {
        case 2:
          // Do nothing
          break;
        case 3:
          app->array[i][j] = 1;
          break;
        default:
          app->array[i][j] = 0;
          break;
      }
    }
  }

}

void draw_board(App *app) {

  for (int i = 1; i < CGL_ARRAY_HEIGHT + 1; i++) {
    for (int j = 1; j < CGL_ARRAY_WIDTH + 1; j++) {
      DrawRectangle(
          CGL_CELL_WIDTH * (j - 1),
          CGL_CELL_HEIGHT * (i - 1),
          CGL_CELL_HEIGHT,
          CGL_CELL_WIDTH,
          (app->array[i][j] == 1) ? app->cell_color_array[app->cell_color_index] : CGL_BACKGROUND_COLOR);
    }
  }

}

void update(App *app) {

  if (app->running) {
    next_generation(app);
  }
  draw_board(app);

  DrawRectangle(
      0, (CGL_ARRAY_HEIGHT * CGL_CELL_HEIGHT), CGL_SCREEN_WIDTH, CGL_HELP_HEIGHT, CGL_HELP_BACKGROUND_COLOR);
  DrawTextEx(app->font, "Start/Stop: [SPACE]", (Vector2){10, 410}, 18, 0, CGL_HELP_FONT_COLOR);
  DrawTextEx(app->font, "Regenerate: [R]", (Vector2){10, 410 + 23}, 18, 0, CGL_HELP_FONT_COLOR);
  DrawTextEx(app->font, "Color: [C]", (Vector2){200, 410}, 18, 0, CGL_HELP_FONT_COLOR);

}

App *init(void) {

  App *app = malloc(sizeof(App));

  if (app == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(-1);
  }

  app->array = calloc((CGL_ARRAY_WIDTH + 2) * (CGL_ARRAY_HEIGHT + 2), sizeof(u8));

  if (app->array == NULL) {
    fprintf(stderr, "out of memory\n");
    free(app);
    exit(-1);
  }

  init_board(app->array);

  app->font = LoadFont(CGL_FONT);
  app->running = false;
  app->cell_color_index = 0;

  // set colors
  app->cell_color_array[0] = CGL_COLOR_00;
  app->cell_color_array[1] = CGL_COLOR_01;
  app->cell_color_array[2] = CGL_COLOR_02;
  app->cell_color_array[3] = CGL_COLOR_03;

  return app;

}

void deinit(App *app) {

  UnloadFont(app->font);
  free(app->array);
  free(app);

} 

int main(void) {

  InitWindow(CGL_SCREEN_WIDTH, CGL_SCREEN_HEIGHT, CGL_TITLE);

  SetTargetFPS(CGL_FPS);

  srand((u32)time(NULL));

  App *app = init();

  SetTextureFilter(app->font.texture, TEXTURE_FILTER_BILINEAR);

  SetExitKey(KEY_NULL);

  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(CGL_BACKGROUND_COLOR);

    update(app);

    if (IsKeyPressed(KEY_SPACE)) {
      app->running = !app->running;
    } else if (IsKeyPressed(KEY_C)) {
      app->cell_color_index++;
      if (app->cell_color_index == CGL_COLOR_COUNT) {
        app->cell_color_index = 0;
      }
    } else if (IsKeyPressed(KEY_R)) {
      init_board(app->array);
      draw_board(app);
    }

    EndDrawing();
  }


  deinit(app);

  CloseWindow();

  return 0;

}

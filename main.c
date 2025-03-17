#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Close Window"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  bool is_running;
};

bool game_init_sdl(struct Game *g);
bool game_new(struct Game *g);
void game_free(struct Game *g);
void game_events(struct Game *g);
void game_draw(struct Game *g);
void game_run(struct Game *g);

bool game_new(struct Game *g) {
  if (!game_init_sdl(g)) {
    return false;
  }

  g->is_running = true;

  return true;
}

bool game_init_sdl(struct Game *g) {
  if (!SDL_Init(SDL_FLAGS)) {
    fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
    return false;
  }

  g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (!g->window) {
    fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
    return false;
  }

  g->renderer = SDL_CreateRenderer(g->window, NULL);

  if (!g->renderer) {
    fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void game_free(struct Game *g) {
  if (g->window) {
    SDL_DestroyWindow(g->window);
    g->window = NULL;
  }

  if (g->renderer) {
    SDL_DestroyRenderer(g->renderer);
    g->renderer = NULL;
  }

  SDL_Quit();
  printf("All Clean!\n");
}

void game_events(struct Game *g) {
  while (SDL_PollEvent(&g->event)) {
    switch (g->event.type) {
    case SDL_EVENT_QUIT:
      g->is_running = false;
      break;
    default:
      break;
    }
  }
}

void game_draw(struct Game *g) {
  SDL_RenderClear(g->renderer);
  SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g) {
  while (g->is_running) {
    game_events(g); // checks events
    game_draw(g);   // draws in the renderer
    SDL_Delay(16);  // 60hz screen refresh
  }
}

int main(void) {
  bool exit_status = EXIT_FAILURE;

  struct Game game = {0};

  if (game_new(&game)) {
    game_run(&game);
    exit_status = EXIT_SUCCESS;
  }

  game_free(&game);

  return exit_status;
}

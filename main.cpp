#include "Chip8.h"
#include <SDL.h>

#define PIXEL_SIZE 20

struct engine_t {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool quit = false;


};


void init(engine_t& engine){

    SDL_Init(SDL_INIT_EVERYTHING);

    engine.window = SDL_CreateWindow("Chip8",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, GRAPHICS_WIDTH * PIXEL_SIZE,GRAPHICS_HEIGHT * PIXEL_SIZE, SDL_WINDOW_SHOWN);
    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);


}

void handle(engine_t& engine){

    SDL_Event e;

    while(SDL_PollEvent(&e)){

        if(e.type == SDL_QUIT){

            engine.quit = true;

        }

    }

}

void render(engine_t& engine, Chip8*& chip8){

    SDL_SetRenderDrawColor(engine.renderer,0x0,0x0,0x0,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(engine.renderer);

    SDL_Rect fillRect = { 0,0,GRAPHICS_WIDTH * PIXEL_SIZE,GRAPHICS_HEIGHT * PIXEL_SIZE };
    SDL_RenderFillRect(engine.renderer,&fillRect);

    if(chip8->getDrawFlag()){




        chip8->setDrawFlag(false);

    }

    SDL_RenderPresent(engine.renderer);

}



int main() {


    engine_t engine;

    init(engine);

    auto* chip8 = Chip8::Instance();
    chip8->loadProgram("../invaders.c8");




    while(!engine.quit){

        chip8->cycle();
        handle(engine);

        render(engine,chip8);

    }

    delete chip8;

    return 0;
}

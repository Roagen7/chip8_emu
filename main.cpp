#include "Chip8.h"
#include <SDL.h>
#include <iostream>

#define PIXEL_SIZE 20

struct engine_t {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;


    uint8_t drawMap[GRAPHICS_HEIGHT * GRAPHICS_WIDTH] = { 0 };

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


void update(engine_t& engine, Chip8*& chip8){

    if(chip8->getDrawFlag()){

        for(int y = 0; y < GRAPHICS_HEIGHT; y++){

            for(int x = 0; x < GRAPHICS_WIDTH; x++){

                if(chip8->getGrahicsAt(y,x)) engine.drawMap[y * GRAPHICS_WIDTH + x] = 1;
                else engine.drawMap[y * GRAPHICS_WIDTH + x] = 0;

            }

        }

        chip8->setDrawFlag(false);

    }

}

void render(engine_t &engine) {

    SDL_SetRenderDrawColor(engine.renderer,0x0,0x0,0x0,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(engine.renderer);

    SDL_Rect fillRect = { 0,0,GRAPHICS_WIDTH * PIXEL_SIZE,GRAPHICS_HEIGHT * PIXEL_SIZE };
    SDL_RenderFillRect(engine.renderer,&fillRect);



    SDL_SetRenderDrawColor(engine.renderer,0xFF,0xFF,0xFF,SDL_ALPHA_OPAQUE);
    for(int y = 0; y < GRAPHICS_HEIGHT; y++){

        for(int x = 0; x < GRAPHICS_WIDTH; x++){

            SDL_Rect pixel = {x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};

            if(engine.drawMap[y * GRAPHICS_WIDTH + x])
                SDL_RenderFillRect(engine.renderer,&pixel);

        }

    }




    SDL_RenderPresent(engine.renderer);

}



int main() {


    engine_t engine;

    init(engine);

    auto* chip8 = Chip8::Instance();
    chip8->loadProgram("../pong.rom");


    while(!engine.quit){

        chip8->cycle();
        handle(engine);
        update(engine,chip8);
        render(engine);

    }

    delete chip8;

    return 0;
}

#include "Chip8.h"
#include <SDL.h>
#include <iostream>

#define PIXEL_SIZE 20

struct engine_t {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;


    uint8_t drawMap[GRAPHICS_HEIGHT * GRAPHICS_WIDTH] = { 0 };

    bool renderFlag = true;
    bool quit = false;


};


void init(engine_t& engine){

    SDL_Init(SDL_INIT_EVERYTHING);

    engine.window = SDL_CreateWindow("Chip8",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     GRAPHICS_WIDTH * PIXEL_SIZE,
                                     GRAPHICS_HEIGHT * PIXEL_SIZE,
                                     SDL_WINDOW_SHOWN);
    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);


}

void handle(engine_t &engine, Chip8 *&chip8) {

    SDL_Event e;


    SDL_Keycode keyTranslation[KEY_S] = {

            SDLK_x, // 0
            SDLK_1, // 1
            SDLK_2, // 2
            SDLK_3, // 3
            SDLK_q, // 4
            SDLK_w, // 5
            SDLK_e, // 6
            SDLK_a, //7
            SDLK_s, // 8
            SDLK_d, // 9
            SDLK_z, // A
            SDLK_c, // B
            SDLK_4, // C
            SDLK_r, // D
            SDLK_f, // E
            SDLK_v // F
    };


    while(SDL_PollEvent(&e)){

        switch (e.type) {

            case SDL_QUIT:
                engine.quit = true;
                break;
            case SDL_KEYDOWN:

               for(int i = 0; i < KEY_S; i++){

                   if(e.key.keysym.sym == keyTranslation[i]) {

                       chip8->setKey(i);
                       break;

                   }


               }


                break;
            case SDL_KEYUP:

                for(int i = 0; i < KEY_S; i++){

                    if(e.key.keysym.sym == keyTranslation[i]) {

                        chip8->unsetKey(i);
                        break;

                    }

                }

                break;

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
        engine.renderFlag = true;
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
    chip8->loadProgram("../c8games/UFO");

    while(!engine.quit){

        uint64_t start = SDL_GetPerformanceCounter();

        chip8->cycle();
        handle(engine, chip8);
        update(engine,chip8);

        if(engine.renderFlag){

            render(engine);
            engine.renderFlag = false;

        }
        uint64_t end = SDL_GetPerformanceCounter();

        auto et = (double)(end - start) / (double) SDL_GetPerformanceFrequency() * 1000.f;

        SDL_Delay(floor(4.0 - et));

    }

    delete chip8;

    return 0;
}

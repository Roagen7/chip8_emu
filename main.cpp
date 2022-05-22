#include "Chip8.h"



int main() {

    auto* chip8 = Chip8::Instance();
    chip8->loadProgram("../pong.rom");

    chip8->cycle();



    return 0;
}

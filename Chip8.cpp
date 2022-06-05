#include "Chip8.h"
#include <iostream>

Chip8* Chip8::_chip8 = nullptr;


Chip8 *Chip8::Instance() {


    if(_chip8 == nullptr){

        _chip8 = new Chip8;

    }

    return _chip8;

}

Chip8::Chip8(){

    //load font

    unsigned char font[FONT_HEIGHT * FONT_CHARSET_SIZE] =
            {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };


    for(int i = 0; i < FONT_HEIGHT * FONT_CHARSET_SIZE; i++){

        memory[i] = font[i];

    }

};

void Chip8::cycle() {

    //fetch opcode

    opcode = memory[PC] << 8 | memory[PC + 1];


    //decode opcode and call the right function
    uint8_t firstDigit = (opcode & 0xF000) >> 12;

    auto rightFunction = callTable[firstDigit];

    (this->*(rightFunction))(); // call the function on this

    //timers

    if(delayTimer > 0) delayTimer--;

    if(soundTimer > 0) soundTimer--;

}

bool Chip8::getDrawFlag() const {
    return drawFlag;
}

void Chip8::loadProgram(const std::string& location) {

    auto file = fopen(location.c_str(),"rb");

    fseek(file,0,SEEK_END);

    long size = ftell(file);

    fseek(file, 0, SEEK_SET);

    auto* buffer = new uint8_t[size];

    fread(buffer,size,1,file);

    fclose(file);

    for(int i = 0; i < size; i++){

        memory[i + PC_START] = buffer[i];

    }

    delete[] buffer;

}

void Chip8::opcodes_x0() {

    switch(opcode & 0x000F){

        case 0x0000:
            // clear screen - graphics = 0;
            for(uint8_t& i : graphics) i = 0;

            drawFlag = true;
            PC += 2;

            break;

        case 0x000E:
            //return fron subroutine - take last PC from stack

            PC = stack[--SP];
            PC += 2;

            break;

        default:

            throw(0);

    }

}

void Chip8::opcodes_x8() {


    uint8_t& vX =  v[(opcode & 0x0F00) >> 8];
    uint8_t& vY = v[(opcode & 0x00F0) >> 4];


    switch (opcode & 0x000F) {

        case 0x0000:
            // for 0x8XY0 set vX = vY

            vX = vY;
            PC += 2;
            break;

        case 0x0001:

            // for 0x8XY1 set vX = vX | vY

            vX |= vY;
            PC += 2;
            break;

        case 0x0002:

            // for 0x8XY2 set vX = vX & vY

            vX &= vY;
            PC += 2;

            break;

        case 0x0003:

            // for 0x8XY3 set vX = vX ^ vY
            vX ^= vY;
            PC += 2;
            break;

        case 0x0004:

            // for 0x8XY4 set vX = vX + vY, set vF = 1 if there is a carry, otherwise set vF = 0

            if(vY > 0xFF - vX)
                v[0xF] = 1;
            else
                v[0xF] = 0;

            vX += vY;
            PC += 2;
            break;

        case 0x0005:

            // for 0x8XY5 set vX = vX - vY, set vF = 0 if there is a borrow, otherwise set vF = 1

            if(vY > vX)
                v[0xF] = 0;
            else
                v[0xF] = 1;

            vX -= vY;
            PC += 2;
            break;

        case 0x0006:

            // for 0x8XY6 store vF = LSB(vX), then vX >> 1

            v[0xF] = 0x1 & vX;
            vX >>= 1;
            PC += 2;

            break;

        case 0x0007:

            // for 0x8XY7 set vX = vY - vX, if there is a borrow vF = 0, otherwise vF = 1

            if(vX > vY)
                v[0xF] = 0;
            else
                v[0xF] = 1;

            vX = vY - vX;
            PC += 2;

            break;

        case 0x000E:

            // for 0x8XYE set vF = MSB(vX) , then vX << 1

            v[0xF] = vX >> 7;
            vX <<= 1;
            PC += 2;

            break;

        default:
            throw(0);

    }

}

void Chip8::opcodes_xD() {

    //sprite
    auto& vX = v[(opcode & 0x0F00) >> 8];
    auto& vY = v[(opcode & 0x00F0) >> 4];
    auto H = opcode & 0x000F;

    v[0xF] = 0;


    for(int y = 0; y < H; y++){

        uint16_t pixel = memory[I+y];

        for(int x = 0; x < 8; x++){

            if((pixel & (0x80 >> x)) != 0){

                auto& gr = graphics[vX + x + ((vY + y) * 64)];

                if(gr == 1) v[0xF] = 1;

                gr ^= 1;

            }

        }

    }

    drawFlag = true;

    PC += 2;

}

void Chip8::opcodes_xE() {


    switch (opcode & 0x000F) {

        case 0x000E:
            // for 0xEX9E if key pressed is not equal to vX skip next instruction

            if(key[v[(opcode & 0x0F00) >> 8]]) PC += 2;
            PC += 2;

            break;

        case 0x0001:

            // for 0xEXA1 if key pressed is equal to vX skip next instruction


            if(key[v[(opcode & 0x0F00) >> 8]] == 0) PC += 2;
            PC += 2;

            break;

        default:
            throw(0);

    }

}

void Chip8::opcodes_xF() {


    auto& vX = v[(opcode & 0x0F00) >> 8];

    switch (opcode & 0x00FF) {

        case 0x0007:
            vX = delayTimer;
            PC += 2;
            break;

        case 0x000A:
            //await key press

            for(int i = 0; i < KEY_S; i++){

                if(key[i] != 0){

                    vX = key[i];
                    PC += 2;
                    return;
                }

            }

            break;

        case 0x0015:
            delayTimer = vX;
            PC += 2;
            break;

        case 0x0018:
            soundTimer = vX;
            PC += 2;
            break;

        case 0x001E:
            I += vX;
            PC += 2;
            break;

        case 0x0029:
            I = vX * 0x5;
            PC += 2;

            break;

        case 0x0033:
            // store vX as bcd: first digit in memory[I], second digit in memory[I+1], third digit in memory[I+2]
            // vX = mem[I]mem[I+1]mem[I+2]

            memory[I]     = vX / 100;
            memory[I + 1] = (vX / 10) % 10;
            memory[I + 2] = vX % 10;

            PC += 2;
            break;

        case 0x0055: {

            uint8_t x = (opcode & 0x0F00) >> 8;

            for (int i = 0; i <= x; i++) {

                memory[I + i] = v[i];

            }
            PC += 2;

            break;
        }

        case 0x0065:

            uint8_t x = (opcode & 0x0F00) >> 8;

            for (int i = 0; i <= x; i++) {

                 v[i] = memory[I + i] ;

            }
            PC += 2;

            break;

    }



}

void Chip8::setDrawFlag(bool _drawFlag) {

    this->drawFlag = _drawFlag;

}

uint8_t Chip8::getGrahicsAt(int y, int x) {

    return graphics[y * GRAPHICS_WIDTH + x];

}

void Chip8::setKey(uint8_t keycode) {

    key[keycode] = 1;

}

void Chip8::unsetKey(uint8_t keycode) {

    key[keycode] = 0;

}

void Chip8::opcodes_x1() {

    //for 0x1NNN jump to NNN

    PC = opcode & 0x0FFF;

}

void Chip8::opcodes_x2() {

    //for 0x2NNN call subroutine at NNN

    stack[SP++] = PC;
    PC = opcode & 0x0FFF;


}

void Chip8::opcodes_x3() {

    //for 0x3XNN if vX == NN skip next instruction

    if(v[(0x0F00 & opcode) >> 8] == (0x00FF & opcode)) PC += 2;

    PC += 2;

}

void Chip8::opcodes_x4() {

    //for 0x4XNN if vX != NN skip next instruction

    if(v[(0x0F00 & opcode) >> 8] != (0x00FF & opcode)) PC += 2;

    PC += 2;


}

void Chip8::opcodes_x5() {

    //for 0x5XY0 if vX == vY skip next instruction

    if(v[(0x0F00 & opcode) >> 8] == v[(0x00F0 & opcode) >> 4]) PC += 2;

    PC += 2;

}

void Chip8::opcodes_x6() {

    //for 0x6XNN set vX = NN

    v[(0x0F00 & opcode) >> 8] = 0x00FF & opcode;

    PC += 2;

}

void Chip8::opcodes_x7() {

    v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;

    PC += 2;

}

void Chip8::opcodes_x9() {



    if(v[(0x0F00 & opcode) >> 8] == v[(0x00F0 & opcode) >> 4]) PC += 2;

    PC += 2;


}

void Chip8::opcodes_xA() {

    //for 0xANNN set I = NNN

    I = opcode & 0x0FFF;
    PC += 2;


}

void Chip8::opcodes_xB() {


    //for 0xBNNN jump to v0 + NNN
    PC = (0x0FFF & opcode) + v[0];



}

void Chip8::opcodes_xC() {

    //for 0xCXNN set Vx = rand() & NN

    v[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);

    PC += 2;

}

#include "Chip8.h"


Chip8* Chip8::_chip8 = nullptr;


Chip8 *Chip8::Instance() {


    if(_chip8 == nullptr){

        _chip8 = new Chip8;

    }

    return _chip8;

}

Chip8::Chip8() = default;

void Chip8::cycle() {

    //fetch opcode

    opcode = memory[PC] << 8 | memory[PC + 1];

    //decode opcode

    switch(opcode & 0xF000){

        //execute opcode

        case 0x0000:

            opcodes_x0();
            break;


        case 0x1000:

            //for 0x1NNN jump to NNN
            PC = opcode & 0x0FFF;

            break;

        case 0x2000:

            //for 0x2NNN call subroutine at NNN
            stack[SP++] = PC;
            PC = opcode & 0x0FFF;

            break;

        case 0x3000:

            //for 0x3XNN if vX == NN skip next instruction
            if(v[(0x0F00 & opcode) >> 8] == (0x00FF & opcode)) PC += 2;

            PC += 2;

            break;

        case 0x4000:

            //for 0x4XNN if vX != NN skip next instruction
            if(v[(0x0F00 & opcode) >> 8] != (0x00FF & opcode)) PC += 2;

            PC += 2;

            break;

        case 0x5000:

            //for 0x5XY0 if vX == vY skip next instruction
            if(v[(0x0F00 & opcode) >> 8] == v[(0x00F0 & opcode) >> 4]) PC += 2;

            PC += 2;

            break;

        case 0x6000:

            v[(0x0F00 & opcode) >> 8] = 0x00FF & opcode;
            PC += 2;

            break;

        case 0x7000:

            v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            PC += 2;

            break;

        case 0x8000:
            opcodes_x8();
            break;

        case 0x9000:

            if(v[(0x0F00 & opcode) >> 8] == v[(0x00F0 & opcode) >> 4]) PC += 2;

            PC += 2;

            break;

        case 0xA000:

            I = opcode & 0x0FFF;
            PC += 2;

            break;

        case 0xB000:

            break;

        case 0xC000:

            break;

        case 0xD000:
            opcodes_xD();
            break;

        case 0xE000:
            opcodes_xE();
            break;

        case 0xF000:
            opcodes_xF();
            break;


        default:
            throw(0);

    }

    //timers

    if(delayTimer > 0) delayTimer--;

    if(soundTimer > 0) soundTimer--;


}

bool Chip8::drawFlag() {
    return false;
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

            break;

        case 0x000E:

            break;

    }

}

void Chip8::opcodes_x8() {


    switch (opcode & 0x000F) {

        case 0x0004:

            break;

        default:
            throw(0);

    }



}

void Chip8::opcodes_xD() {

}

void Chip8::opcodes_xE() {

}

void Chip8::opcodes_xF() {

}

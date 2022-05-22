#pragma once

#include <cstdint>
#include <string>

#define MEMORY 4096
#define REGISTERS 16
#define STACK 16
#define KEY_S 16

#define GRAPHICS_WIDTH 64
#define GRAPHICS_HEIGHT 32


#define PC_START 0x200

class Chip8 {

public:

    static Chip8* Instance();

    Chip8(const Chip8& other) = delete;
    void operator=(const Chip8& other) = delete;

    void cycle();
    void loadProgram(const std::string& location);

    bool getDrawFlag();


private:

    Chip8();

    static Chip8* _chip8;

    uint16_t opcode = 0;
    uint8_t memory[MEMORY] = {0};

    //registers
    uint8_t v[REGISTERS] = {0};
    uint16_t I = 0; // 12 bits
    uint16_t PC = PC_START; // 12 bits

    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

    uint16_t stack[STACK] = { 0 };
    uint16_t SP = 0;

    //ui
    uint8_t graphics[GRAPHICS_HEIGHT * GRAPHICS_WIDTH] = { 0 };
    uint16_t key[KEY_S] = { 0 };

    bool drawFlag = false;


    void opcodes_x0();
    void opcodes_x8();
    void opcodes_xD();
    void opcodes_xE();
    void opcodes_xF();





};
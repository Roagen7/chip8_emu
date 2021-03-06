/*
 * chip 8 emulator
 * https://en.wikipedia.org/wiki/CHIP-8
 *
 */


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


#define FONT_WIDTH 4
#define FONT_HEIGHT 5
#define FONT_CHARSET_SIZE 16

class Chip8 {

public:

    static Chip8* Instance();

    Chip8(const Chip8& other) = delete;
    void operator=(const Chip8& other) = delete;

    void cycle();
    void loadProgram(const std::string& location);

    bool getDrawFlag() const;
    void setDrawFlag(bool _drawFlag);

    uint8_t getGrahicsAt(int y, int x);

    void setKey(uint8_t keycode);
    void unsetKey(uint8_t keycode);



private:

    Chip8();

    static Chip8* _chip8;

    void (Chip8::*callTable[16])() = {
            &Chip8::opcodes_x0, &Chip8::opcodes_x1, &Chip8::opcodes_x2, &Chip8::opcodes_x3,
            &Chip8::opcodes_x4, &Chip8::opcodes_x5, &Chip8::opcodes_x6, &Chip8::opcodes_x7,
            &Chip8::opcodes_x8, &Chip8::opcodes_x9, &Chip8::opcodes_xA, &Chip8::opcodes_xB,
            &Chip8::opcodes_xC, &Chip8::opcodes_xD, &Chip8::opcodes_xE, &Chip8::opcodes_xF
    };

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

    void opcodes_x1();
    void opcodes_x2();
    void opcodes_x3();
    void opcodes_x4();
    void opcodes_x5();
    void opcodes_x6();
    void opcodes_x7();

    void opcodes_x8();

    void opcodes_x9();
    void opcodes_xA();
    void opcodes_xB();
    void opcodes_xC();

    void opcodes_xD();
    void opcodes_xE();
    void opcodes_xF();





};
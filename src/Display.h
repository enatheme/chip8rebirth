#pragma once

#include "Processor.h"

#include "../external/imgui/lib/imgui.h"
#include "../external/imgui/lib/imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <cstring>
#include <iostream>

struct Sprites
{
    using Sprite = bool[4][5];
    constexpr void generate(std::array<uint8_t, 5> input, Sprite & sprite)
    {
        for (uint8_t line = 0 ; line < 5 ; ++line)
        {
            sprite[line][0] = input[line] & 0x8;
            sprite[line][1] = input[line] & 0x4;
            sprite[line][2] = input[line] & 0x2;
            sprite[line][3] = input[line] & 0x1;
        }
    }
    constexpr Sprites() : values()
    {
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0x9, 0x9, 0xF}, values[0x0]);
        generate(std::array<uint8_t, 5>{0x2, 0x6, 0x2, 0x2, 0x7}, values[0x1]);
        generate(std::array<uint8_t, 5>{0xF, 0x1, 0xF, 0x8, 0xF}, values[0x2]);
        generate(std::array<uint8_t, 5>{0xF, 0x1, 0xF, 0x1, 0xF}, values[0x3]);
        generate(std::array<uint8_t, 5>{0x9, 0x9, 0xF, 0x1, 0x1}, values[0x4]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x1, 0xF}, values[0x5]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x9, 0xF}, values[0x6]);
        generate(std::array<uint8_t, 5>{0xF, 0x1, 0x2, 0x4, 0x4}, values[0x7]);
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0xF, 0x9, 0xF}, values[0x8]);
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0xF, 0x1, 0xF}, values[0x9]);
        generate(std::array<uint8_t, 5>{0xF, 0x9, 0xF, 0x9, 0x9}, values[0xA]);
        generate(std::array<uint8_t, 5>{0xE, 0x9, 0xE, 0x9, 0xE}, values[0xB]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0x8, 0x8, 0xF}, values[0xC]);
        generate(std::array<uint8_t, 5>{0xE, 0x9, 0x9, 0x9, 0xE}, values[0xD]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x8, 0xF}, values[0xE]);
        generate(std::array<uint8_t, 5>{0xF, 0x8, 0xF, 0x8, 0x8}, values[0xF]);
    }
    std::array<Sprite, 17> values;
    constexpr const Sprite & operator[](uint8_t i) const
    {
        return values[i];
    }
};


class Display
{
public:
    Display(const char * title, bool is_visible) : m_title(title), m_is_visible(is_visible) {}
    virtual ~Display() = default;
    virtual void update() = 0;
    virtual void clear() = 0;

protected:
    const char * m_title;
    bool m_is_visible;
};

class Screen: public Display
{
/*
 * Note on the screen, there is one instruction to drawn on it:
 * "Dxyn - DRW Vx, Vy, nibble
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

 * The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
 */
public:
    Screen() : Display("Screen", true)
    {
        std::memset(m_screen, '*', Screen::X * Screen::Y);
    }

    void update() override final;

    void clear() override final;

    void draw(uint8_t sprite, uint8_t x, uint8_t y);

private:
    constexpr void flip(char & c)
    {
        c = ((c == '*') ? ' ' : '*');
    }
    constexpr void xor_flip(char & c)
    {
        c = ((c == ' ') ? ' ' : '*');
    }
    static constexpr uint8_t X = 32;
    static constexpr uint8_t Y = 64;
    char m_screen[Screen::X][Screen::Y];
};

template <class PROCESSOR>
class Options: public Display
{
public:
    Options(Processor<PROCESSOR> & proc) : Display("Options", true), m_processor(proc)
    {}

    void update() override final;

    void clear() override final;

private:
    bool m_diagram_animated = false;
    Processor<PROCESSOR> & m_processor;

};

#include "Display.impl"

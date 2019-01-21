#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <functional>
#include <string_view>

#include <cstddef>
#include <cstdio>
#include <cstdlib>


#include <iostream>

struct HexToString
{
    static constexpr char hexmap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    constexpr HexToString() : values()
    {
        for (uint8_t i = 0 ; i < 0xFF ; ++i)
        {
            values[i][0] = '0';
            values[i][1] = 'x';
            values[i][2] = hexmap[(i & 0xF0) >> 4];
            values[i][3] = hexmap[(i & 0x0F)];
            values[i][4] = '\0';
        }
    }
    char values[0xFF][5];
    constexpr const char * operator[](uint8_t i) const
    {
        return values[i];
    }
};

constexpr size_t MemorySize = 4096;
constexpr size_t RegistersSize = 16;
constexpr std::string_view register_title[] = {"v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "va", "vb", "vc", "vd", "ve", "vf"};

    /*  
     * 12 bits : nnn or addr
     * 4 bits : n or nibble
     * 4 bits : x
     * 4 bits : y
     * a byte : kk or byte
     * 
     *  ___________________
     * |0xf0|0x0f|0xf0|0x0f|
     * |____|____|____|____|
     * |dbyte1   | dbyte2  |
     * |_________|_________|
     * |    | nnn or addr  |
     * |____|______________|
     * |    |    | kk/byte |
     * |____|____|_________|
     * |    | x  | y  | n  |
     * |____|____|____|____|
     * |a char   |a char   |
     * |____|____|____|____|
     * |4bit|4bit|4bit|4bit|
     * |____|____|____|____|
     * |0101|1101|0110|1100|
     * |____|____|____|____|
     * 
     */

template <class DISPLAY_TYPE>
class Processor
{
public:
    Processor(const std::string & rom_path, DISPLAY_TYPE display) : m_display(display)
    {
        FILE* fp = std::fopen(rom_path.c_str(), "r");
        if(!fp) {
            std::perror("File opening failed");
            exit(-1);
        }

        int c; // note: int, not char, required to handle EOF
        unsigned int a = 0x200;
        while ((c = std::fgetc(fp)) != EOF) { // standard C I/O file reading loop
             m_memory[a] = c;
             a++;
        }

        m_registers = {0};
    }

    Processor(const Processor &) = delete;
    Processor(const Processor &&) = delete;
    ~Processor() = default;

    void display(std::function<void(const char *, size_t)> f)
    {
        do_display_memory(f);
        do_display_register(f);
    }

    void display_memory(std::function<void(const char *, size_t)> f)
    {
        do_display_memory(f);
    }

    void display_register(std::function<void(const char *, size_t)> f)
    {
        do_display_register(f);
    }

    void display_current_instruction(std::function<void(const char *, size_t)> f)
    {
        do_display_current_instruction(f);
    }

    void next_instruction()
    {
        do_next_instruction();
    }

private:
    template <typename DISPLAY_FUNC>
    void do_display_memory(DISPLAY_FUNC render)
    {
        for (size_t i = 0 ; i < MemorySize ; i+=8) {
            std::string name;
            for (size_t j = 0 ; j < 8 ; j++)
            {
                name += HexToString()[m_memory[i + j]];
                name += " ";
            }
            render(name.c_str(), size_t(name.c_str() + name.size()));
        }
    }

    template <typename DISPLAY_FUNC>
    void do_display_register(DISPLAY_FUNC render)
    {
        for (size_t i = 0 ; i < RegistersSize ; i++)
        {
            std::string name;
            name += register_title[i];
            name += " ";
            name += HexToString()[m_registers[i]];
            render(name.c_str(), size_t(name.c_str() + name.size()));
        }
    }

    template <typename DISPLAY_FUNC>
    void do_display_current_instruction(DISPLAY_FUNC render) const;

    void dispatch();

    void do_next_instruction();
private:
    std::array<uint8_t, MemorySize> m_memory;
    std::array<uint8_t, RegistersSize> m_registers;

    DISPLAY_TYPE m_display;

    short m_register_I = 0;
    short m_ip = 0;
    short m_sp = 0;
    short m_bp = 0;
    
    // current instruction
    uint8_t m_dbyte1 = 0;
    uint8_t m_dbyte2 = 0;
};

#include "Processor.impl"

#pragma once

#include "Display.h"
#include "Processor.h"

class Chip8
{
public:
    Chip8(const std::string & filename)
    {
        m_processor = Processor<Screen>(filename, m_screen);
        m_options = std::make_unique<Options<Screen>>(m_processor);
        m_registers = std::make_unique<Registers<Screen>>(m_processor);
        m_memory = std::make_unique<Memory<Screen>>(m_processor);
        m_debug = std::make_unique<Debug<Screen>>(m_processor, m_screen);
    }

    void update()
    {
        m_screen.update();
        m_options->update();
        m_registers->update();
        m_memory->update();
        m_debug->update();
    }

private:
    Screen m_screen;
    Processor<Screen> m_processor;
    std::unique_ptr<Options<Screen>> m_options = nullptr;
    std::unique_ptr<Registers<Screen>> m_registers = nullptr;
    std::unique_ptr<Memory<Screen>> m_memory = nullptr;
    std::unique_ptr<Debug<Screen>> m_debug = nullptr;
};

#include "Display.h"

void Screen::update()
{
    ImGui::Begin(m_title);
    for (uint8_t i = 0 ; i < Screen::X ; ++i)
    {
        ImGui::TextUnformatted(m_screen[i], m_screen[i] + Screen::Y);
    }
    ImGui::End();
}

void Screen::clear()
{
    std::memset(m_screen, '*', Screen::X * Screen::Y);
}

void Screen::draw(uint8_t sprite, uint8_t x, uint8_t y)
{
    for (uint8_t iter_x = 0, end_x = std::min(uint8_t(x + 5), uint8_t(Screen::X - 1)) ; x + iter_x < end_x ; ++iter_x)
    {
        for (uint8_t iter_y = 0, end_y = std::min(uint8_t(y + 4), uint8_t(Screen::Y - 1)) ; y + iter_y < end_y ; ++iter_y)
        {
            if (Sprites()[sprite][iter_x][iter_y])
            {
                flip(m_screen[x + iter_x][y + iter_y]);
            }
            else
            {
                xor_flip(m_screen[x + iter_x][y + iter_y]);
            }
        }
    }
}

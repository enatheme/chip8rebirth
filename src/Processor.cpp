#include "Processor.h"

template <class DISPLAY_TYPE>
void Processor<DISPLAY_TYPE>::next_instruction()
{
    m_dbyte1 = m_memory[m_ip++];
    m_dbyte2 = m_memory[m_ip++];
}

template <class DISPLAY_TYPE>
void Processor<DISPLAY_TYPE>::dispatch()
{
    switch(m_dbyte1 & 0xf0)
    {
        case 0x00:
            switch(m_dbyte2)
            {
                case 0xE0:
                    // 00E0 - CLS
                    m_display.clear();
                break;
                case 0xEE:
                    // 00EE - RET
                    m_ip = m_sp;
                    m_sp--;
                break;
                default:
                    // 0nnn - SYS addr
                    /*
                     * ignored
                     * This instruction is only used on the old computers on which Chip-8 was originally implemented.
                     * It is ignored by modern interpreters.
                     */
                break;
            }
        break;

        case 0x10:
            // 1nnn - JP addr
            m_ip = ((m_dbyte1 & 0x0F) << 8 | m_dbyte2);
        break;
        case 0x20:
            // 2nnn - CALL addr
            m_memory[m_sp] = m_ip;
            m_sp++;
        break;
        case 0x30:
            // 3xkk - SE Vx, byte
            if (m_registers[(m_dbyte1 & 0x0F)] == m_dbyte2)
            {
                m_ip += 2;
            }
        break;
        case 0x40:
            // 4xkk - SNE Vx, byte
            if (m_registers[(m_dbyte1 & 0x0F)] != m_dbyte2)
            {
                m_ip += 2;
            }
        break;
        case 0x50:
            // 5xy0 - SE Vx, Vy
            if (m_registers[(m_dbyte1 & 0x0F)] != m_registers[(m_dbyte2 & 0xF0) >> 4])
            {
                m_ip += 2;
            }
        break;
        case 0x60:
            // 6xkk - LD Vx, byte
            m_registers[m_dbyte1 & 0x0F] = m_dbyte2;
        break;
        case 0x70:
            // 7xkk - ADD Vx, byte
            m_registers[m_dbyte1 & 0x0F] += m_dbyte2;
        break;
        case 0x80:
            switch(m_dbyte2 & 0x0f)
            {
                case 0x0:
                    // 8xy0 - LD Vx, Vy
                    m_registers[m_dbyte1 & 0x0F] = m_registers[(m_dbyte2 & 0xF0) >> 4];
                break;
                case 0x1:
                    // 8xy1 - OR Vx, Vy
                    m_registers[m_dbyte1 & 0x0F] |= m_registers[(m_dbyte2 & 0xF0) >> 4];
                break;
                case 0x2:
                    // 8xy2 - AND Vx, Vy
                    m_registers[m_dbyte1 & 0x0F] &= m_registers[(m_dbyte2 & 0xF0) >> 4];
                break;
                case 0x3:
                    // 8xy3 - XOR Vx, Vy
                    m_registers[m_dbyte1 & 0x0F] ^= m_registers[(m_dbyte2 & 0xF0) >> 4];
                break;
                case 0x4:
                    // 8xy4 - ADD Vx, Vy
                    //instruc_15();
                break;
                case 0x5:
                    // 8xy5 - SUB Vx, Vy
                    //instruc_16();
                break;
                case 0x6:
                    // 8xy6 - SHR Vx {, Vy}
                    //instruc_17();
                break;
                case 0x7:
                    // 8xy7 - SUBN Vx, Vy
                    //instruc_18();
                break;
                case 0xE:
                    // 8xyE - SHL Vx {, Vy}
                    //instruc_19();
                break;
            }

        break;
        case 0x90:
            // 9xy0 - SNE Vx, Vy
            //instruc_20();
        break;
        case 0xa0:
            // Annn - LD I, addr
            m_register_I = ((m_dbyte1 & 0x0F) << 8) | m_dbyte2;
        break;
        case 0xb0:
            // Bnnn - JP V0, addr
            m_ip = (((m_dbyte1 & 0x0F) << 8) | m_dbyte2) + m_registers[0]; 
        break;
        case 0xc0:
            // Cxkk - RND Vx, byte
            m_registers[m_dbyte1 & 0x0F] = (std::rand() % 255) ^ m_dbyte2;
        break;
        case 0xd0:
            // Dxyn - DRW Vx, Vy, nibble
        break;
        case 0xe0:
            switch(m_dbyte2)
            {
                case 0x9E:
                    // Ex9E - SKP Vx
                break;
                case 0xA1:
                    // ExA1 - SKNP Vx
                break;
            }
        break;
        case 0xf0:
            switch(m_dbyte2)
            {
                case 0x07:
                    // Fx07 - LD Vx, DT
                break;
                case 0x0A:
                    // Fx0A - LD Vx, K
                break;
                case 0x15:
                    // Fx15 - LD DT, Vx
                break;
                case 0x18:
                    // Fx18 - LD ST, Vx
                break;
                case 0x1E:
                    // Fx1E - ADD I, Vx
                break;
                case 0x29:
                    // Fx29 - LD F, Vx
                break;
                case 0x33:
                    // Fx33 - LD B, Vx
                break;
                case 0x55:
                    // Fx55 - LD [I], Vx
                break;
                case 0x65:
                    // Fx65 - LD Vx, [I]
                break;
            }
        break;
    }
}

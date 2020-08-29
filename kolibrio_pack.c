#include "kolibrio_pack.h"

void init_a_packet(kolibrio_pack_t *packet, uint8_t length, uint8_t *buffer, uint8_t len_of_buffer)
{
    packet->length = length;
    packet->buffer = buffer;
    packet->length_of_buffer = len_of_buffer;

    packet->_index = 0;
    packet->_cursor = 0;
    packet->buffer[packet->_cursor++] = length;
}

void deserialize_a_packet(kolibrio_pack_t *packet, uint8_t *buffer)
{
    packet->buffer = buffer;
    packet->length = buffer[0];

    packet->_cursor = 0;
    packet->_index = 0;
}

void writeFloat(kolibrio_pack_t *packet, float value)
{

    if (packet->_cursor < packet->length_of_buffer && packet->_index < packet->length)
    {
        packet->buffer[packet->_cursor++] = TYPE_FLOAT;
        packet->buffer[packet->_cursor++] = (*(uint32_t *)(&value) >> 24) & 0xFF;
        packet->buffer[packet->_cursor++] = (*(uint32_t *)(&value) >> 16) & 0xFF;
        packet->buffer[packet->_cursor++] = (*(uint32_t *)(&value) >> 8) & 0xFF;
        packet->buffer[packet->_cursor++] = (*(uint32_t *)(&value) >> 0) & 0xFF;

        packet->_index++;
        if (packet->_index == packet->length - 1)
        {
            packet->_index = 0;
        }
    }
}

void writeBoolean(kolibrio_pack_t *packet, bool value)
{
    if (packet->_cursor < packet->length_of_buffer && packet->_index < packet->length)
    {
        packet->buffer[packet->_cursor++] = TYPE_BOOLEAN;
        packet->buffer[packet->_cursor++] = value ? 1 : 0;

        packet->_index++;
        if (packet->_index == packet->length - 1)
        {
            packet->_index = 0;
        }
    }
}

float getFloat(kolibrio_pack_t *packet, uint8_t index)
{
    // we'll ignore the first byte
    uint8_t cursor = 1;

    for (int i = 0; i < packet->length; i++)
    {
        if (packet->buffer[cursor] == TYPE_FLOAT)
        {
            if (i == index)
            {
                cursor += 1;

                uint32_t float_bytes = (uint32_t)packet->buffer[cursor++] << 24 |
                                       (uint32_t)packet->buffer[cursor++] << 16 |
                                       (uint32_t)packet->buffer[cursor++] << 8 |
                                       (uint32_t)packet->buffer[cursor++] << 0;
                return *(float *)&float_bytes;
            }
            else
            {
                cursor += 5;
            }
        }
    }
}

bool getBoolean(kolibrio_pack_t *packet, uint8_t index)
{
    uint8_t cursor = 1;

    for (int i = 0; i < packet->length; i++)
    {
        if (packet->buffer[cursor] == TYPE_BOOLEAN)
        {
            if (i == index)
            {
                cursor += 1;
                return packet->buffer[cursor++] ? true : false;
            }
            else
            {
                cursor += 2;
            }
        }
        else if (packet->buffer[cursor] == TYPE_FLOAT)
        {
            cursor += 5;
        }
    }
}
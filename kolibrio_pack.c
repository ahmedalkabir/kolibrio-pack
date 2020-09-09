#include "kolibrio_pack.h"
#include <string.h>
// private functions
uint8_t _increase_cursor_by_type(uint8_t cursor, kolibrio_pack_t *packet)
{
    switch (packet->buffer[cursor])
    {
    case TYPE_FLOAT:
        return 5;
        break;
    case TYPE_INTEGER:
        return 3;
    case TYPE_BOOLEAN:
        return 2;
    case TYPE_STRING:
        cursor = (uint8_t)packet->buffer[++cursor] + 2;
    default:
        break;
    }
    return cursor;
}

void init_a_packet(kolibrio_pack_t *packet, uint8_t length, uint8_t *buffer, uint8_t len_of_buffer)
{
    packet->length = length;
    packet->buffer = buffer;

    // lenght of the buffer help us
    // for not exceding the lenght of holder
    // buffer
    packet->length_of_buffer = len_of_buffer;

    packet->_index = 0;
    packet->_cursor = 0;
    // setup the length of the packet
    // or let's say how many value the packet
    // can hold
    packet->buffer[packet->_cursor++] = length;
}

void deserialize_a_packet(kolibrio_pack_t *packet, uint8_t *buffer)
{
    packet->buffer = buffer;
    packet->length = buffer[0];

    packet->_cursor = 0;
    packet->_index = 0;
}

void writeInteger(kolibrio_pack_t *packet, int value)
{
    if (packet->_cursor < packet->length_of_buffer && packet->_index < packet->length)
    {
        packet->buffer[packet->_cursor++] = TYPE_INTEGER;
        packet->buffer[packet->_cursor++] = (value >> 8) & 0xff;
        packet->buffer[packet->_cursor++] = (value >> 0) & 0xff;

        packet->_index++;
    }
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
    }
}

void writeBoolean(kolibrio_pack_t *packet, bool value)
{
    if (packet->_cursor < packet->length_of_buffer && packet->_index < packet->length)
    {
        packet->buffer[packet->_cursor++] = TYPE_BOOLEAN;
        packet->buffer[packet->_cursor++] = value ? 1 : 0;

        packet->_index++;
    }
}

void writeString(kolibrio_pack_t *packet, const char *value)
{
    if (packet->_cursor < packet->length_of_buffer && packet->_index < packet->length)
    {
        packet->buffer[packet->_cursor++] = TYPE_STRING;
        char *pointer_string = value;

        // add length of the string for helping
        // in deserialization
        uint8_t lenth_of_string = strlen(value) + 1;
        packet->buffer[packet->_cursor++] = (uint8_t)lenth_of_string;

        while (*pointer_string != '\0')
            packet->buffer[packet->_cursor++] = (uint8_t)*pointer_string++;

        // as we know in c and c++ the string is array of characters with '\0' terminator at the end
        // of the array
        packet->buffer[packet->_cursor++] = '\0';

        packet->_index++;
    }
}

int getInteger(kolibrio_pack_t *packet, uint8_t index)
{
    // we'll ignore the first byte
    // because it contains the length of
    // the packet
    uint8_t cursor = 1;

    for (uint8_t i = 0; i < packet->length; i++)
    {
        if (packet->buffer[cursor] == TYPE_INTEGER)
        {
            if (i == index)
            {
                cursor += 1;

                int integer_value = (int)packet->buffer[cursor++] << 8 |
                                    (int)packet->buffer[cursor++] << 0;
                return integer_value;
            }
            else
            {
                cursor += _increase_cursor_by_type(cursor, packet);
            }
        }
        else
        {
            cursor += _increase_cursor_by_type(cursor, packet);
        }
    }
}

float getFloat(kolibrio_pack_t *packet, uint8_t index)
{
    // we'll ignore the first byte
    // because it contains the length of
    // the packet
    uint8_t cursor = 1;

    for (uint8_t i = 0; i < packet->length; i++)
    {
        if (packet->buffer[cursor] == TYPE_FLOAT)
        {
            if (i == index)
            {
                // ignore the type of the value
                cursor += 1;

                uint32_t float_bytes = (uint32_t)packet->buffer[cursor++] << 24 |
                                       (uint32_t)packet->buffer[cursor++] << 16 |
                                       (uint32_t)packet->buffer[cursor++] << 8 |
                                       (uint32_t)packet->buffer[cursor++] << 0;
                return *(float *)&float_bytes;
            }
            else
            {
                cursor += _increase_cursor_by_type(cursor, packet);
            }
        }
        else
        {
            cursor += _increase_cursor_by_type(cursor, packet);
        }
    }
}

bool getBoolean(kolibrio_pack_t *packet, uint8_t index)
{
    uint8_t cursor = 1;

    for (uint8_t i = 0; i < packet->length; i++)
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
                cursor += _increase_cursor_by_type(cursor, packet);
            }
        }
        else
        {
            cursor += _increase_cursor_by_type(cursor, packet);
        }
    }
}

char *getString(kolibrio_pack_t *packet, uint8_t index)
{
    uint8_t cursor = 1;

    for (uint8_t i = 0; i < packet->length; i++)
    {
        if (packet->buffer[cursor] == TYPE_STRING)
        {
            if (i == index)
            {
                cursor += 2;
                return &(packet->buffer[cursor]);
            }
            else
            {
                cursor += _increase_cursor_by_type(cursor, packet);
            }
        }
        else
        {
            cursor += _increase_cursor_by_type(cursor, packet);
        }
    }
}

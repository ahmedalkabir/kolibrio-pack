#ifndef KOLIBRIO_PACK_H_
#define KOLIBRIO_PACK_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*
    form of the serilized packet
    LENGTH|TYPE|VALUE

    for now i'll support integer, float, boolean and string

    if the TYPE == STR
    TYPE|LEN_OF_STR|VALUE
*/

#define TYPE_NONE    0x00
#define TYPE_BOOLEAN 0x20
#define TYPE_FLOAT   0x40
#define TYPE_STRING  0x80
#define TYPE_BYTES   0xA0
#define TYPE_INTEGER 0xC0

#if defined __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        // public
        // the holder buffer
        uint8_t *buffer;
        // length of the packet in binary 
        // how much bytes can hold
        uint8_t length_of_buffer;
        // length of the packet 
        uint8_t length;

        // private
        // internal cursor for tracking 
        // bytes in the packet
        uint8_t _cursor;
        // internal index of the packet
        uint8_t _index;

        // array of adresses 
        // i know i used a fixed array 
        // but this type of library will be used for 
        // wireless communication
        size_t *array_of_addresses[50];

        size_t *test;
    } kolibrio_pack_t;

    // public funtion
    void init_a_packet(kolibrio_pack_t *packet, uint8_t length, uint8_t *buffer, uint8_t len_of_buffer);

    void deserialize_a_packet(kolibrio_pack_t *packet, uint8_t *buffer);
    
    void writeFloat(kolibrio_pack_t *packet, float value);

    void writeInteger(kolibrio_pack_t *packet, int value);

    void writeBoolean(kolibrio_pack_t *packet, bool value);

    void writeString(kolibrio_pack_t *packet, const char *value);
    
    int getInteger(kolibrio_pack_t *packet, uint8_t index);

    float getFloat(kolibrio_pack_t *packet, uint8_t index);

    float getFloat_old(kolibrio_pack_t *packet, uint8_t index);

    bool getBoolean(kolibrio_pack_t *packet, uint8_t index);

    char *getString(kolibrio_pack_t *packet, uint8_t index);
    // private functions
    uint8_t _increase_cursor_by_type(uint8_t cursor, kolibrio_pack_t *packet); 
#if defined __cplusplus
}
#endif

#endif
#ifndef KOLIBRIO_PACK_H_
#define KOLIBRIO_PACK_H_

#include <stdbool.h>
#include <stdint.h>

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

#if defined __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        // public
        uint8_t *buffer;
        uint8_t length_of_buffer;
        uint8_t length;

        // private
        uint8_t _cursor;
        uint8_t _index;
    } kolibrio_pack_t;

    // public funtion
    void init_a_packet(kolibrio_pack_t *packet, uint8_t length, uint8_t *buffer, uint8_t len_of_buffer);

    void deserialize_a_packet(kolibrio_pack_t *packet, uint8_t *buffer);
    
    void writeFloat(kolibrio_pack_t *packet, float value);

    void writeInteger(kolibrio_pack_t *packet, int value);

    void writeBoolean(kolibrio_pack_t *packet, bool value);

    float getFloat(kolibrio_pack_t *packet, uint8_t index);

    bool getBoolean(kolibrio_pack_t *packet, uint8_t index);
    // private functions

#if defined __cplusplus
}
#endif

#endif
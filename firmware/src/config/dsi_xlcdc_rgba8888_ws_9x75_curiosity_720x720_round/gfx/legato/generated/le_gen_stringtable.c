#include "gfx/legato/generated/le_gen_assets.h"

/*****************************************************************************
 * Legato String Table
 * Encoding        ASCII
 * Language Count: 1
 * String Count:   8
 *****************************************************************************/

/*****************************************************************************
 * string table data
 * 
 * this table contains the raw character data for each string
 * 
 * unsigned short - number of indices in the table
 * unsigned short - number of languages in the table
 * 
 * index array (size = number of indices * number of languages
 * 
 * for each index in the array:
 *   unsigned byte - the font ID for the index
 *   unsigned byte[3] - the offset of the string codepoint data in
 *                      the table
 * 
 * string data is found by jumping to the index offset from the start
 * of the table
 * 
 * string data entry:
 *     unsigned short - length of the string in bytes (encoding dependent)
 *     codepoint data - the string data
 ****************************************************************************/

const uint8_t stringTable_data[134] =
{
    0x08,0x00,0x01,0x00,0x00,0x24,0x00,0x00,0x01,0x28,0x00,0x00,0x01,0x34,0x00,0x00,
    0x01,0x40,0x00,0x00,0x02,0x4A,0x00,0x00,0x01,0x50,0x00,0x00,0x01,0x6A,0x00,0x00,
    0x01,0x78,0x00,0x00,0x01,0x00,0x30,0x00,0x09,0x00,0x54,0x75,0x72,0x6E,0x20,0x4C,
    0x65,0x66,0x74,0x00,0x0A,0x00,0x54,0x75,0x72,0x6E,0x20,0x52,0x69,0x67,0x68,0x74,
    0x08,0x00,0x41,0x52,0x52,0x49,0x56,0x45,0x44,0x21,0x04,0x00,0x6B,0x6D,0x2F,0x68,
    0x17,0x00,0x54,0x69,0x6D,0x65,0x20,0x4C,0x65,0x66,0x74,0x3A,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x6D,0x69,0x6E,0x73,0x00,0x0B,0x00,0x44,0x52,0x49,0x56,
    0x45,0x20,0x53,0x41,0x46,0x45,0x21,0x00,0x0B,0x00,0x47,0x6F,0x20,0x53,0x74,0x72,
    0x61,0x69,0x67,0x68,0x74,0x00,
};

/* font asset pointer list */
leFont* fontList[3] =
{
    (leFont*)&NotoSansBoldI_48,
    (leFont*)&NotoSansB_18,
    (leFont*)&NotoSansBoldI_25,
};

const leStringTable stringTable =
{
    {
        LE_STREAM_LOCATION_ID_INTERNAL, // data location id
        (void*)stringTable_data, // data address pointer
        134, // data size
    },
    (void*)stringTable_data, // string table data
    fontList, // font lookup table
    LE_STRING_ENCODING_ASCII // encoding standard
};


// string list
leTableString string_Speed;
leTableString string_Left;
leTableString string_Right;
leTableString string_Arrived;
leTableString string_SpeedUnit;
leTableString string_remainingTime;
leTableString string_Instruction;
leTableString string_Straight;

void initializeStrings(void)
{
    leTableString_Constructor(&string_Speed, stringID_Speed);
    leTableString_Constructor(&string_Left, stringID_Left);
    leTableString_Constructor(&string_Right, stringID_Right);
    leTableString_Constructor(&string_Arrived, stringID_Arrived);
    leTableString_Constructor(&string_SpeedUnit, stringID_SpeedUnit);
    leTableString_Constructor(&string_remainingTime, stringID_remainingTime);
    leTableString_Constructor(&string_Instruction, stringID_Instruction);
    leTableString_Constructor(&string_Straight, stringID_Straight);
}

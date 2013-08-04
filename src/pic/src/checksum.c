#include "checksum.h"
#include "messages.h"

void
sendToMainLowIfValid(
    unsigned char* data,
    unsigned char length,
    unsigned char msgType,
    unsigned char checksum)
{
    if (isValidChecksum(data, length, checksum) == 1) {
        ToMainLow_sendmsg(length, msgType, (void *) data);
    }
}

unsigned char
isValidChecksum(
    unsigned char* data,
    unsigned char length,
    unsigned char checksum)
{
    unsigned char actualChecksum =
        constructChecksum(data, length);

    if (checksum == actualChecksum)
        return 1;
    else
        return 0;
}

unsigned char
constructChecksum(
    unsigned char* data,
    unsigned char length)
{
    // Construct a checksum by XORing all bytes of data
    unsigned char checksum = 0x00;
    unsigned char temp[10];
    {
        unsigned char i;
        for (i = 0; i < length+1; ++i) {
            temp[i] = data[i];
        }
    }
    {
        unsigned char i;
        for (i = 0; i < length; ++i) {
            checksum ^= data[i];
        }
    }

    return checksum;
}

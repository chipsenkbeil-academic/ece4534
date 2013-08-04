#ifndef _CHECKSUM_H
#define	_CHECKSUM_H

/**
 * Sends a message to main low if the checksum is valid.
 * @param data The data to send
 * @param length The length of bytes
 * @param msgType The message in main
 * @param checksum The checksum to compare to the data
 */
void sendToMainLowIfValid(unsigned char* data, unsigned char length,
        unsigned char msgType, unsigned char checksum);

/**
 * Checks hether or not the packet of data provided is valid using the
 * checksum provided.
 * @param data The list of bytes to compare to the checksum
 * @param checksum The byte representing the checksum
 * @return 0 if invalid, 1 if valid
 */
unsigned char isValidChecksum(unsigned char* data, unsigned char length,
        unsigned char checksum);

/**
 * Creates a checksum byte from an array of bytes.
 * @param data The list of bytes
 * @param length The total number of bytes
 * @return The checksum byte
 */
unsigned char constructChecksum(unsigned char* data, unsigned char length);

#endif


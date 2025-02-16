#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // While there's still data left to read from the memory card
    uint8_t buffer[512]; // Pointer

    char filename[8]; // 7 + 1 for null
    int i = 0;
    int flag = 0;
    FILE *jpeg = NULL;

    // fread(address of buffer, sizeof(buffer), no. of size, address to read)
    while (fread(buffer, sizeof(uint8_t), 512, card) == 512)
    {

        // If detect new JPEG header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0 && flag == 1)
        {
            fclose(jpeg);
            flag = 0;
        }
        // if JPEG header,
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0 && flag == 0)
        {
            // Create JPEG from the data.
            sprintf(filename, "%03i.jpg", i);
            jpeg = fopen(filename, "w");
            i = i + 1;
            flag = 1;
        }
        // Continue writing file
        if (flag == 1)
        {
            fwrite(buffer, sizeof(uint8_t), 512, jpeg);
        }
    }

    // Close files
    fclose(card);
    if (jpeg != NULL)
    {
        fclose(jpeg);
    }
    return 0;
}

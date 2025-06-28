#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef int bool;

typedef uint8_t BYTE;

const int FAT_BLOCK_SIZE = 512;

const BYTE JPEG_HEADER_BYTE_0 = 0xff;
const BYTE JPEG_HEADER_BYTE_1 = 0xd8;
const BYTE JPEG_HEADER_BYTE_2 = 0xff;
const BYTE JPEG_HEADER_MASK = 0xf0;
const BYTE JPEG_HEADER_MATCH = 0xe0;

// PROTOTYPES
// Checks whether the block is the header of a JPEG
bool is_jpeg_header(BYTE buffer[]);

// Creates the name of JPG files
void create_filename(char filename[], int count);

// Opens a file and returns the pointer
FILE *open_file(const char *filename, const char *mode);

// Writes 512 bytes (one block) to a JPG file
void write_jpeg_block(FILE *img, BYTE buffer[]);

int main(int argc, char *argv[])
{
    // Checks for Command-Line Arguments
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // Open the Card
    FILE *card = open_file(argv[1], "r");
    if (card == NULL)
    {
        return 1;
    }

    // Temporary buffer for the fread
    BYTE byte[FAT_BLOCK_SIZE];

    int image_count = 0;
    FILE *img = NULL;
    bool found_jpg = false;

    // While there's enough space left in the card
    while (fread(byte, sizeof(BYTE), FAT_BLOCK_SIZE, card) ==
           FAT_BLOCK_SIZE) // Per the manual, fread returns the number of bytes read. It should be
                           // either 0 or 512. That is, it only enters the loop if it can read the
                           // whole 512 bytes. Otherwise, it means there's no more space left for a
                           // whole block
    {
        // If the JPEG header was found
        if (is_jpeg_header(byte))
        {
            // If there was already an image read before this one
            if (found_jpg == true)
            {
                fclose(img); // Close it
            }

            // Creating name of JPG files
            char filename[8]; // Creates an array that'll have the name of each file (###.jpg + \0)
            create_filename(filename, image_count);

            // Creating and Opening JPG Files
            img = open_file(filename, "w");
            if (img == NULL)
            {
                printf("Error: Failed to create image file '%s'\n", filename);
                fclose(card);
                return 2;
            }

            // Incrementing the number of the JPG file's name
            image_count++;

            found_jpg = true;
        }

        if (found_jpg == true)
        {
            write_jpeg_block(img, byte);
        }
    }

    if (img != NULL)
    {
        fclose(img);
    }

    fclose(card);
    return 0;
}

// Checks whether the block is the header of a JPEG
bool is_jpeg_header(BYTE buffer[])
{
    return buffer[0] == JPEG_HEADER_BYTE_0 && buffer[1] == JPEG_HEADER_BYTE_1 &&
           buffer[2] == JPEG_HEADER_BYTE_2 && (buffer[3] & JPEG_HEADER_MASK) == JPEG_HEADER_MATCH;
}

// Creates the name of JPG files
void create_filename(char filename[], int count)
{
    sprintf(filename, "%03i.jpg",
            count); // Sprintf works like printf but inside the program rather than outside (in the
                    // terminal window) Unraveling %03i - %i (Placeholder for Integers) - 03
                    // (Indicates that the rest of the digits that are not the %i must be zeros. And
                    // the total must be 3 digits)
}

// Opens a file and returns the pointer
FILE *open_file(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    return file;
}

// Writes 512 bytes (one block) to a JPG file
void write_jpeg_block(FILE *img, BYTE buffer[])
{
    fwrite(buffer, sizeof(BYTE), FAT_BLOCK_SIZE, img);
}

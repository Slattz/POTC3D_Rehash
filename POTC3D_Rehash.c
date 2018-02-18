#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "POTC3D_Rehash.h"

#define SAVE_MAIN 0xA70
#define SAVE_HEADER 0x18

void wait_to_exit(void)
{
    printf("\nPress Any Key To Exit.\n");
    getchar(); 
    return;
}

void help_text(void)
{
    printf("LEGO Pirates Of The Carribean 3DS Checksum Fixer v1.0\n");
    printf("By Slattz\n");
    printf("Usage: 'POTC3D_Rehash filename', or drag-and-drop a file.\n\n");
}

u32 Fix_LEGOPOTC3D_Checksum(u8* data, u32 length)
{
    u32 checksum = 0xFFFFFFFF;
    if (length > 0)
    {
        u32 i;
        u32 j;
        u32 offset = 0;
        u8 byte = 0;
        u32 unk;
        for (i = length>>1; i > 0; i--)
        {
            for (j = 0; j < 2; j++)
            {
                byte = (u8)(data[offset+j]);
                byte ^= checksum;
                byte &= 0xFF;
                unk = CRC_TABLE[ (byte<<2)/4 ];
                checksum = unk ^ (checksum>>8);
            }
            offset += 2;
        }
        return checksum;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    help_text();
    if(argc < 2)
    {
        printf("Specify a filename or drag-and-drop a file.\n");
        wait_to_exit();
        return 0;
    }

    FILE *save = fopen(argv[1], "r+b");
    if (save == NULL)
    {
        printf("Error opening file!\n");
        wait_to_exit();
        return 0; 
    }
    
    fseek(save, 0, SEEK_END);
    u32 savesize = ftell(save);

    if (savesize != SAVE_HEADER+SAVE_MAIN)
    {
        printf("File isn't the correct size (0x%X)!\n", SAVE_HEADER+SAVE_MAIN);
        wait_to_exit();
        return 0;
    }

    u8 *buffer = malloc(SAVE_MAIN);
    
    fseek(save, 0x18, SEEK_SET);
    fread(buffer, 1, SAVE_MAIN, save);
    u32 checksum = Fix_LEGOPOTC3D_Checksum(buffer, SAVE_MAIN);
    fseek(save, 0, SEEK_SET);
    fwrite(&checksum, 1, 4, save);
    free(buffer);

    fclose(save);
    return 0;
}

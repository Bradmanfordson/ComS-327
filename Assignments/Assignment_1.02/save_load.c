#include <stdio.h>
#include <stdint.h>
#include <endian.h>

// This is a file used to "test" the features of saving to binary
// if this file goes as planned then it'll be included in my src
// and be implemented in the main program.

void write_file();
void read_file();

char header[] = "RLG327-F2018";
uint32_t version = 0;
uint8_t room[4];
uint32_t size = 1702;
uint8_t xpos = 65;
uint8_t ypos = 53;

int main(int argc, char *argv[]){

    room[0] = 3;
    room[1] = 6;
    room[2] = 99;
    room[3] = 243;


    if(argv[1][0] == 'w'){
        write_file(header);
    } else if(argv[1][0] == 'r'){
        read_file(header);
    } else {
        printf("Bad arguments.\n");
    }

    return 0;
}


void write_file(){
    FILE *file;
    file = fopen("test.bin", "w");

    // printf("Size of data WRITE: %ld\n vs %ld\n", sizeof(header), sizeof(&header));
    // printf("Writing: %s \n", header);

    fwrite(header, sizeof(header) - 1, 1, file); // Header is minus 1 because it's a char array (string) and will have an extra terminating value at the end
    fwrite(&version, sizeof(version), 1, file);
    fwrite(&size, sizeof(size), 1, file);
    fwrite(&xpos, sizeof(xpos), 1, file);
    fwrite(&ypos, sizeof(ypos), 1, file);

    fclose(file);

    //printf(" Write Room: %hhn\n", room);
}


void read_file(){
    FILE *file_test;
    char header[12];
    uint32_t version;
    uint32_t file_size;
    uint8_t player_xpos;
    uint8_t player_ypos;

    file_test = fopen("test.bin", "r");

    fread(&header, sizeof(header), 1, file_test);
    fread(&version, sizeof(version), 1, file_test);
    fread(&file_size, sizeof(file_size), 1, file_test);
    fread(&player_xpos, sizeof(player_xpos), 1, file_test);
    fread(&player_ypos, sizeof(player_ypos), 1, file_test);

    printf("Header: %s\n \
            Version: %d\n \
            File Size: %d\n \
            Player X Position: %d\n \
            Player Y Position: %d\n", header, version, file_size, player_xpos, player_ypos);

    fclose(file_test);
}
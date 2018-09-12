#include <stdio.h>
#include <stdint.h>
#include <endian.h>

// This is a file used to "test" the features of saving to binary
// if this file goes as planned then it'll be included in my src
// and be implemented in the main program.


enum Action{
    save, // --save the dungeon
    load // --load the dungeon
}; 




void write_file();
void read_file();
// struct GameData{
//     char *header;  // bits  0   -   11: Header file-type
//     int version;   // bits 12   -   15: unsigned 32-bit int file version marker with the value 0
//     int file_size; // bits 16   -   19: unsigned 32-bit int size of the file
//     int x;         // bit  20         : Character starting X position
//     int y;         // bit  21         : Character staring Y position
//     struct var;    // bits 22   - 1701: The row-major dungeon matrix from top to bottom, with one byte, containing cell hardness, per cell.
//     struct var;    // bits 1702 -  end: position of all the rooms in the dungeon, given with 4 unsigned 8-bit integers each
// };
char header[] = "RLG327-F2018";
uint32_t version = 0;
uint8_t room[4];

int main(int argc, char *argv[]){

    // struct GameData_tester init_data;
    // init_data.header = 'Hello';
    // init_data.footer = 'World';
    room[0] = 3;
    room[1] = 6;
    room[2] = 99;
    room[3] = 243;

    uint32_t size = 1702;

    printf("File Size: %d\n", size);

    uint32_t be_size;
    be_size = htobe32(size);

    printf("BE File Size: %d\n",be_size);

    uint32_t size_le;
    size_le = be32toh(be_size);
    printf("Back to host File Size: %d\n",size_le);

    printf("Test: %d\n", htobe32(1702));


    //printf("Room: %hhn\n", room);

    //printf("Header: %s \n", header);

    // FILE *file;
    // file = fopen("test.bin", "w");

   

    // fwrite(&header, sizeof(header), 1, file);

    // fclose(file);

    



    // printf("Size of data MAIN: %ld\n", sizeof(init_data));

    // printf("%s %s", init_data.header, init_data.footer);

    // if(argv[1][0] == 'w'){
    //     write_file(header);
    // } else if(argv[1][0] == 'r'){
    //     read_file(header);
    // } else {
    //     printf("Bad arguments.\n");
    // }

    return 0;
}


void write_file(){
    FILE *file;
    file = fopen("test.bin", "w");

    printf("Size of data WRITE: %ld\n", sizeof(header));
    printf("Writing: %s \n", header);

    fwrite(&header, sizeof(header), 1, file);

    fwrite(&version, sizeof(version), 1, file);

    fwrite(&room, sizeof(room), 1, file);

    fclose(file);
    printf(" Write Room: %hhn\n", room);
}


void read_file(){
    FILE *file_test;
    char test[12];
    file_test = fopen("test.bin", "r");

    fread(&test, sizeof(test), 1, file_test);
    fread(&room, sizeof(room), 1, file_test);
    printf("Size of data READ: %ld\n", sizeof(header));

    printf("Reading: %s %hhn \n", test, room);

    fclose(file_test);
}
#include <stdio.h>

// This is a file used to "test" the features of saving to binary
// if this file goes as planned then it'll be included in my src
// and be implemented in the main program.


enum Action{
    save, // --save the dungeon
    load // --load the dungeon
}; 

struct var{
    // Place holder for something. Need to figure that out
};

struct GameData_tester{
    char *header;
    char *footer;
};


void write_file(struct GameData_tester data);
void read_file(struct GameData_tester data);
// struct GameData{
//     char *header;  // bits  0   -   11: Header file-type
//     int version;   // bits 12   -   15: unsigned 32-bit int file version marker with the value 0
//     int file_size; // bits 16   -   19: unsigned 32-bit int size of the file
//     int x;         // bit  20         : Character starting X position
//     int y;         // bit  21         : Character staring Y position
//     struct var;    // bits 22   - 1701: The row-major dungeon matrix from top to bottom, with one byte, containing cell hardness, per cell.
//     struct var;    // bits 1702 -  end: position of all the rooms in the dungeon, given with 4 unsigned 8-bit integers each
// };

int main(int argc, char *argv[]){

    struct GameData_tester init_data;
    init_data.header = 'Hello';
    init_data.footer = 'World';

    printf("Size of data MAIN: %ld\n", sizeof(init_data));

    if(argv[1][0] == 'w'){
        write_file(init_data);
    } else if(argv[1][0] == 'r'){
        read_file(init_data);
    } else {
        printf("Bad arguments.\n");
    }

    return 0;
}


void write_file(struct GameData_tester data){
    FILE *file;
    file = fopen("test.bin", "w");

    printf("Size of data WRITE: %ld\n", sizeof(data));
    printf("Writing: %s %s\n", &data.header, &data.footer);

    fwrite(&data, sizeof(data), 1, file);

    fclose(file);
}

void read_file(struct GameData_tester data){
    FILE *file_test;

    file_test = fopen("test.bin", "r");

    fread(&data, sizeof(data), 1, file_test);
    printf("Size of data READ: %ld\n", sizeof(data));

    printf("Reading: %s %s \n", &data.header, &data.footer);

    fclose(file_test);
}
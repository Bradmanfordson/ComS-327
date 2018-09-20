#include <stdio.h>
#include <stdint.h>
#include <endian.h>
#include <string.h>
#include <time.h>

// This is a file used to "test" the features of saving to binary
// if this file goes as planned then it'll be included in my src
// and be implemented in the main program.

void write_file();
void read_file();

struct Bin{
    char header[12];
    uint32_t version;
    uint32_t size;
    uint8_t xpos;
    uint8_t ypos;
    uint8_t hardness[1680];
};

struct Bin test;

// char header[] = "RLG327-F2018";
//     uint32_t version = 0;
//     uint8_t room[4];
//     uint32_t size = 1702;
//     uint8_t xpos = 65;
//     uint8_t ypos = 53;


int main(int argc, char *argv[]){

    srand(time(NULL));
    
    //Test.header = "RLG327-F2018";
   // test.header = "RLG327-F2018";
    strcpy(test.header, "RLG327-F2018");
    test.version = 0;
    test.size = 1702;
    test.xpos = 9;
    test.ypos = 14;

    // room[0] = 3;
    // room[1] = 6;
    // room[2] = 12;
    // room[3] = 10;

// gets a random number between and not including 0 and 255
    for(int i = 0; i < sizeof(test.hardness); i++){
        int num = rand() % 255;
        if(num == 0){
            num++;
        } else if(num ==255){
            num--;
        }
        test.hardness[i] = num;
    }


    if(argv[1][0] == 'w'){
        write_file();
    } else if(argv[1][0] == 'r'){
        read_file("tmp/test_dungeon_files/101.little-endian.rlg327");
        read_file("tmp/test_dungeon_files/101.rlg327");
        read_file("tmp/test_dungeon_files/102.little-endian.rlg327");
        read_file("tmp/test_dungeon_files/102.rlg327");
        read_file("tmp/test_dungeon_files/103.little-endian.rlg327");
        read_file("tmp/test_dungeon_files/103.rlg327");
        read_file("tmp/test_dungeon_files/104.little-endian.rlg327");
        read_file("tmp/test_dungeon_files/104.rlg327");
        read_file("tmp/test_dungeon_files/105.little-endian.rlg327");
        read_file("tmp/test_dungeon_files/105.rlg327");
        read_file("tmp/test_dungeon_files/106.little-endian.rlg327");
        read_file("tmp/test_dungeon_files/106.rlg327");
    } else {
        printf("Bad arguments.\n");
    }

    return 0;
}


void write_file(){
    FILE *file;
    file = fopen("test.bin", "w");

    int room[4][4];
    for(int y = 0; y < 4; y++){
        for( int x = 0; x < 4; x++){
            room[y][x] = rand() % 255;
        }
    }

    // printf("Size of data WRITE: %ld\n vs %ld\n", sizeof(header), sizeof(&header));
    // printf("Writing: %s \n", header);

    // fwrite(header, sizeof(header) - 1, 1, file); // Header is minus 1 because it's a char array (string) and will have an extra terminating value at the end
    // fwrite(&version, sizeof(version), 1, file);
    // fwrite(&size, sizeof(size), 1, file);
    // fwrite(&xpos, sizeof(xpos), 1, file);
    // fwrite(&ypos, sizeof(ypos), 1, file);
    // fwrite(&room, sizeof(room), 1, file);

    fwrite(&test.header, sizeof(test.header), 1, file); // Header is minus 1 because it's a char array (string) and will have an extra terminating value at the end
    fwrite(&test.version, sizeof(test.version), 1, file);
    fwrite(&test.size, sizeof(test.size), 1, file);
    fwrite(&test.xpos, sizeof(test.xpos), 1, file);
    fwrite(&test.ypos, sizeof(test.ypos), 1, file);
    fwrite(&test.hardness, sizeof(test.hardness), 1, file);
    fwrite(&room, sizeof(room), 1, file);

    fclose(file);

    //printf(" Write Room: %hhn\n", room);
}


void read_file(char path[]){

    struct Bin test;
    FILE *file_test;
    // char header[12];
    // uint32_t version;
    // uint32_t file_size;
    // uint8_t player_xpos;
    // uint8_t player_ypos;
    // uint8_t room_test[4];
    int rooms[4][4];
    printf(path);

    file_test = fopen(path, "r");

    fread(&test.header, sizeof(test.header), 1, file_test);
    fread(&test.version, sizeof(test.version), 1, file_test);
    fread(&test.size, sizeof(test.size), 1, file_test);
    fread(&test.xpos, sizeof(test.xpos), 1, file_test);
    fread(&test.ypos, sizeof(test.ypos), 1, file_test);
    fread(&test.hardness, sizeof(test.hardness), 1, file_test);
    fread(&rooms, sizeof(rooms), 1, file_test);

    printf("Header: %s\n \ 
    Version: %d\n \
    File Size: %d\n \
    Player X Position: %d\n \
    Player Y Position: %d\n \ \n",
    // Room (X1,Y1): ( %d, %d)\n \
    // Room (X2, Y2): ( %d, %d)\n ", 
            test.header,  test.version, 
            test.size,    test.xpos, test.ypos); 
            // test.room[0], test.room[1], 
            // test.room[2], test.room[3]);

    for(int i = 0; i < 21; i++){
        for(int j = 0; j < 80; j++){
            int arr_pos = (i*80)+j;
            if(test.hardness[arr_pos] == 0){
                printf(".");
            } else if( test.hardness[arr_pos] == 255){
                
                if(i == 0 || i == 20){
                    printf("-");
                } else {
                    printf("|");
                }
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
            

    fclose(file_test);
}
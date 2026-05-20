#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    printf("CWD: %s\n", cwd);

    unsigned char buffer[256];
    FILE *ptr;

    ptr = fopen("Fleek_Canyon_of_Mercy.wld 2", "rb");
    if (!ptr) {
        perror("fopen");
        return 1;
    }

    fread(buffer, sizeof(buffer), 1, ptr);
    int offset = 0x0;

    // check version
    uint32_t version;
    memcpy(&version, &buffer[offset], 4);
    printf("Version: %u\n", version);  // little-endian 32-bit integer
    offset += 4;

    // check magic number
    if (memcmp(&buffer[offset], "relogic", 7) != 0) {
        fprintf(stderr, "invalid file format\n");
        fclose(ptr);
        return 1;
    }
    offset += 7;

    // check file type
    unsigned char wldFile = 2;
    
    if (memcmp(&buffer[offset], &wldFile, 1) == 0) {
        printf("This is a world file\n");
    } else {
        printf("This is not a world file\n");
    }
    offset += 1;

    // check revision number
    uint32_t revision;
    memcpy(&revision, &buffer[offset], 4);
    printf("Revision: %u\n", revision);
    offset += 4;

    // check isFavorite
    uint64_t isFavorite;
    memcpy(&isFavorite, &buffer[offset], 8);
    if (isFavorite) {
        printf("This world is set as favourite\n");
    } else {
        printf("This world is not set as favorite\n");
    }
    offset += 8;

    // check number of pointers
    short numPointers;
    memcpy(&numPointers, &buffer[offset], 2);
    printf("Number of pointers: %u\n", numPointers);
    offset += 2;

    int32_t *positions = malloc(numPointers * sizeof(int32_t));
    for (int i=0; i < numPointers; i++) {
        memcpy(&positions[i], &buffer[offset], 4);
        offset += 4;
    }

    for (int i = 0; i < numPointers; i++) {
        printf("Section %d starts at byte: %d\n", i, positions[i]);
    }

    free(positions);

    for(int i = 0; i<20; i++)
        if (buffer[i] >= 32 && buffer[i] <= 126)  // within ASCII range
            printf("%c", buffer[i]);  // prints as ASCII
        else
            printf(".");

    fclose(ptr);

    return 0;
}
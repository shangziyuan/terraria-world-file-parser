#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    printf("CWD: %s\n", cwd);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file.wld>\n", argv[0]);
        return 1;
    }

    unsigned char buffer[256];
    FILE *ptr;

    ptr = fopen(argv[1], "rb");
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

    // go to worldHeader
    printf("World header is located at byte %d\n", positions[0]);
    fseek(ptr, positions[0], SEEK_SET);
    unsigned char worldNameLength;
    fread(&worldNameLength, 1, 1, ptr);
    printf("World name length: %u\n", worldNameLength);

    char worldName[worldNameLength];
    fread(worldName, 1, worldNameLength, ptr);
    printf("World name: %s\n", worldName);

    unsigned char worldIdLength;
    fread(&worldIdLength, 1, 1, ptr);
    printf("World id length: %u\n", worldIdLength);

    char worldId[worldIdLength];
    fread(worldId, 1, worldIdLength, ptr);
    printf("World id: %s\n", worldId);  // 2.1.2.0.304257113

    // go to NPCs section
    printf("NPCs section is located at byte %d\n", positions[4]);
    fseek(ptr, positions[4], SEEK_SET);
    unsigned char npcBuffer[256];
    fread(npcBuffer, 1, 256, ptr);
    for (int i = 0; i < 256; i++)
        if (npcBuffer[i] >= 32 && npcBuffer[i] <= 126)
            printf("%c", npcBuffer[i]);
        else
            printf(".");
    printf("\n");

    // go to Bestiary
    printf("Bestiary is located at byte %d\n", positions[8]);
    printf("Next section is located at byte %d\n", positions[9]);
    fseek(ptr, positions[8], SEEK_SET);

    int32_t numKillEntries;
    fread(&numKillEntries, 4, 1, ptr);
    printf("Number of kill entries: %u\n", numKillEntries);
    for (int i=0; i<numKillEntries; i++) {
        unsigned char len;
        fread(&len, 1, 1, ptr);
        char name[len + 1];
        fread(name, 1, len, ptr);
        name[len] = '\0';
        int32_t kills;
        fread(&kills, 4, 1, ptr);
        printf("  %s: %d kills\n", name, kills);
    }

    int32_t numSeenEntries;
    fread(&numSeenEntries, 4, 1, ptr);
    printf("Number of seen entries: %u\n", numSeenEntries);
    for (int i=0; i<numSeenEntries; i++) {
        unsigned char len;
        fread(&len, 1, 1, ptr);
        char name[len + 1];
        fread(name, 1, len, ptr);
        name[len] = '\0';
        printf("  %s seen\n", name);
    }

    int32_t numChattedEntries;
    fread(&numChattedEntries, 4, 1, ptr);
    printf("Number of chatted NPCs entries: %u\n", numChattedEntries);
    for (int i=0; i<numChattedEntries; i++) {
        unsigned char len;
        fread(&len, 1, 1, ptr);
        char name[len + 1];
        fread(name, 1, len, ptr);
        name[len] = '\0';
        printf("  Chatted with: %s\n", name);
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
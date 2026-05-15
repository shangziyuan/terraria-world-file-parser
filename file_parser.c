#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    printf("CWD: %s\n", cwd);

    unsigned char buffer[20];
    FILE *ptr;

    ptr = fopen("Fleek_Canyon_of_Mercy.wld", "rb");
    if (!ptr) {
        perror("fopen");
        return 1;
    }

    fread(buffer, sizeof(buffer), 1, ptr);

    // check version
    uint32_t version;
    memcpy(&version, &buffer[0], 4);
    printf("Version: %u\n", version);  // little-endian 32-bit integer

    // check magic number
    if (memcmp(&buffer[0x04], "relogic", 7) != 0) {
        fprintf(stderr, "invalid file format\n");
        fclose(ptr);
        return 1;
    }

    // check file type
    unsigned char wldFile = 2;
    
    if (memcmp(&buffer[0x0B], &wldFile, 1) == 0) {
        printf("This is a world file\n");
    } else {
        printf("This is not a world file\n");
    }

    for(int i = 0; i<20; i++)
        if (buffer[i] >= 32 && buffer[i] <= 126)  // within ASCII range
            printf("%c", buffer[i]);  // prints as ASCII
        else
            printf(".");

    fclose(ptr);

    return 0;
}
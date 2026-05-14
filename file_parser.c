#include <stdio.h>
#include <unistd.h>

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

    for(int i = 0; i<20; i++)
        if (buffer[i] >= 32 && buffer[i] <= 126)  // within ASCII range
            printf("%c", buffer[i]);  // prints as ASCII
        else
            printf(".");

    fclose(ptr);

    return 0;
}
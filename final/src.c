#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void take_image(void);
void read_text(void);
void process_text(void);
void remove_linefeed(char *);

int main(void)
{
    take_image();
    process_text();
    read_text();
    return 0;
}

void process_text(void)
{
    FILE * fp;
    fp = popen("convert output.png -density 300 -units PixelsPerInch processed.png", "r");
    pclose(fp);
    printf("Change density\n");
    fp = popen("sudo ./textcleaner -g -o 3 -f 15 processed.png processed_2.png","r");
    pclose(fp);
    printf("Preprocessing complete\n\n");
}

void take_image() {
    FILE *fp;
    fp = popen("raspistill -o output.png", "r");
    pclose(fp);
    printf("Photo taken\n");
}

// Use tesseract to convert image to text
void read_text()
{
    char output[10000];
    FILE * fp;
    fp = popen("tesseract processed_2.png stdout", "r");
    while (fgets(output, sizeof(output), fp) != NULL)
    {
        remove_linefeed(output);
        printf("%s", output);

    }
    pclose(fp);
}


// Replace linefeed with newline
void remove_linefeed(char * str)
{
    int i = 0;
    while(str[i] != 0)
    {
        str[i] = (str[i] == 12)? 10: str[i];
        i++;
    }
}

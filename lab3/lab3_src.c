/*
* @author Alyssa Wardwell
* @author Joseph Albert
*
*/

#include<stdio.h>

int main(void)
{
    FILE * load_file;
    char line[256];

    load_file = fopen("/proc/loadavg", "r");

    fgets(line, sizeof line, load_file);

    printf("%s", line);

    fclose(load_file);
    

}


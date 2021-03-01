#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    for (int i = 0; i < 5; i++)
    {
        if (fork())
            printf("parent");
        else
            printf("child!");
    }

    return 0;
}

#include<stdio.h>

#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define PERI_BASE 0x3F000000    // page 6 of 2837 manual
#define GPIO_BASE (PERI_BASE + 0x200000) // page 90 2837 manual

#define BLOCK_SIZE (4 * 1024)

/* Declare Macros */
#define INP_GPIO(g) *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_SET *(gpio.addr + 7)
#define GPIO_CLR *(gpio.addr + 10)

// IO Access
struct peripheral {
    unsigned long addr_p;
    int mem_fd;
    void * map;
    volatile unsigned int * addr;
};

extern struct peripheral gpio;

int map_peripheral(struct peripheral * p);
void unmap_peripheral(struct peripheral * p);

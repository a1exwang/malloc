#pragma once

void *sbrk(int increment);
extern char brk[];
#define BRK_SIZE (1*1024*1024)
#define BRK_INIT_SIZE (1*1024)
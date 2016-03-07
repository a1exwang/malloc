#include "sbrk.h"

char brk[BRK_SIZE];
int current_size = BRK_INIT_SIZE;

void *sbrk(int increment) {
	current_size += increment;
}
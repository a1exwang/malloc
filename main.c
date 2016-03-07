#include "malloc.h"
#include <stdio.h>

/************************************************************************/
/* 因为是在Windows下跑的, 没有sbrk, 所以只能自己模拟一个sbrk            */
/************************************************************************/

int main() {
	void *blocks[1000];

	for (int i = 0; i < 10; ++i) {
		blocks[i] = mymalloc(2 << i);
		printf("malloc: size=%d, start=0x%08x\n", 2<<i, (unsigned)blocks[i]);
	}

	for (int i = 0; i < 10; ++i) {
		if (blocks[i])
			myfree(blocks[i]);
	}

	// free后重新alloc发现可以分配到之前free过的地址, 说明free成功了
	for (int i = 0; i < 10; ++i) {
		blocks[i] = mymalloc(2 << i);
		printf("malloc: size=%d, start=0x%08x\n", 2 << i, (unsigned)blocks[i]);
	}

	return 0;
}
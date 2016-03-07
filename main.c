#include "malloc.h"
#include <stdio.h>

/************************************************************************/
/* ��Ϊ����Windows���ܵ�, û��sbrk, ����ֻ���Լ�ģ��һ��sbrk            */
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

	// free������alloc���ֿ��Է��䵽֮ǰfree���ĵ�ַ, ˵��free�ɹ���
	for (int i = 0; i < 10; ++i) {
		blocks[i] = mymalloc(2 << i);
		printf("malloc: size=%d, start=0x%08x\n", 2 << i, (unsigned)blocks[i]);
	}

	return 0;
}
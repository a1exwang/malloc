#include <malloc.h>

int main() {
	void *blocks[1000];

	for (int i = 0; i < 10; ++i)
		blocks[i] = mymalloc(2 << i);

	for (int i = 0; i < 10; ++i) {
		myfree(blocks[i]);
	}

	return 0;
}
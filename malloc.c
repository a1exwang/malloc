#include "sbrk.h"

typedef struct list_node {
	int allocated;
	struct list_node *last, *next;
	void *start;
	int size;
	int free;
} list_node_t;

static list_node_t free_head = {
	0,
	&free_head, &free_head,
	brk,
	BRK_INIT_SIZE,
	1
};

#define NODE_STRUCT_COUNT 100
list_node_t nodes[NODE_STRUCT_COUNT] = { 0 };
void list_init(list_node_t *node);
list_node_t *list_alloc() {
	for (int i = 0; i < NODE_STRUCT_COUNT; ++i) {
		if (!nodes[i].allocated) {
			list_init(&nodes[i]);
			return &nodes[i];
		}
	}
	return 0;
}
void list_free(list_node_t *node) {
	for (int i = 0; i < NODE_STRUCT_COUNT; ++i) {
		if (&nodes[i] == node) {
			nodes[i].allocated = 0;
		}
	}
}
void list_init(list_node_t *node) {
	node->last = node->next = 0;
	node->start = (void*)0;
	node->size = 0;
	node->free = 1;
	node->allocated = 1;
}
void list_add(list_node_t *node, list_node_t *new_node) {
	new_node->last = node;
	new_node->next = node->next;
	node->next->last = new_node;
	node->next = new_node;
}
static int total_free_size = BRK_INIT_SIZE;

void *mymalloc(int size) {
	if (total_free_size < size) {
		sbrk(size);
	}
	list_node_t *current = free_head.next;
	do {
		// 找到一个足够大的块
		if (current->free && current->size >= size) {
			void *ret = current->start;
			current->free = 0;
			
			// 如果空闲块比需要的大则把剩余的部分新建成为一个块
			if (current->size != size) {
				list_node_t *new_node = list_alloc();
				new_node->free = 1;
				new_node->size = current->size - size;
				new_node->start = (char*)current->start + size;
				list_add(current, new_node);

				current->size = size;
			}
			total_free_size -= size;
			return ret;
		}
		current = current->next;
	} while (current != &free_head);
	return 0;
}
void myfree(void *mem) {
	list_node_t *current = free_head.next;
	while (current != &free_head) {
		if (current->start == mem) {
			current->free = 1;
			total_free_size += current->size;
			if (current->next->free && 
				(char*)current->start + current->size == (char*)current->next->start) {
				list_node_t *next = current->next;
				// 合并current和next
				current->size += next->size;

				// 释放next, 并且维护链表
				
				current->next = next->next;
				next->next = current;
				list_free(next);
			}
		}
		current = current->next;
	}
}

void *my_best_alloc(int size) {
	if (total_free_size < size) {
		sbrk(size);
	}
	list_node_t *current = free_head.next;
	int min = BRK_SIZE;
	list_node_t *min_node;
	do {
		// 找到最小且足够的块
		if (current->free && current->size >= size) {
			if (current->size < min) {
				min_node = current;
				min = current->size;
			}
		}
		current = current->next;
	} while (current != &free_head);

	if (min == BRK_SIZE) {
		return 0;
	}

	current = min_node;
	void *ret = current->start;
	current->free = 0;

	// 如果空闲块比需要的大则把剩余的部分新建成为一个块
	if (current->size != size) {
		list_node_t *new_node = list_alloc();
		new_node->free = 1;
		new_node->size = current->size - size;
		new_node->start = (char*)current->start + size;
		list_add(current, new_node);

		current->size = size;
	}
	total_free_size -= size;
	return ret;
}
void my_best_free(void *mem) {
	list_node_t *current = free_head.next;
	while (current != &free_head) {
		if (current->start == mem) {
			current->free = 1;
			total_free_size += current->size;
			if (current->next->free &&
				(char*)current->start + current->size == (char*)current->next->start) {
				list_node_t *next = current->next;
				// 合并current和next
				current->size += next->size;

				// 释放next, 并且维护链表

				current->next = next->next;
				next->next = current;
				list_free(next);
			}
		}
		current = current->next;
	}
}

#include "sbrk.h"

typedef struct list_node {
	int allocated;
	struct list_node *last, *next;
	void *start;
	int size;
	int free;
} list_node_t;

static list_node_t free_head = {
	&free_head, &free_head,
	brk,
	BRK_INIT_SIZE,
	1
};

#define NODE_STRUCT_COUNT 100
list_node_t nodes[NODE_STRUCT_COUNT] = { 0 };
list_node_t *list_alloc() {
	for (int i = 0; i < NODE_STRUCT_COUNT; ++i) {
		if (!nodes[i].allocated) {
			list_init(&nodes[i]);
			return &nodes[i];
		}
	}
	return 0;
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
	while (current != &free_head) {
		if (current->free && current->size >= size) {
			void *ret = current->start;
			current->free = 0;
				
			if (current->size != size) {
				list_node_t *new_node = list_alloc();
				new_node->free = 1;
				new_node->size = current->size - size;
				new_node->start = (char*)current->start + size;
				list_add(current, new_node);

				current->size = size;
			}
			return ret;
		}
	}
	return 0;
}


void myfree(void *mem) {
	list_node_t *current = free_head.next;
	while (current != &free_head) {
		if (current->start == mem) {
			void *ret = current->start;
			current->free = 0;
		}
	}
}
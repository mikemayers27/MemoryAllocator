#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

typedef char ALIGN[16];

union header 
{
	struct 
	{
		size_t size;
		unsigned is_free;
		union header *next;
	} s;
	
	ALIGN stub;
};
typedef union header header_t;

header_t *head = NULL, *tail = NULL;
pthread_mutex_t global_malloc_lock;

header_t *get_free_block(size_t size)
{
	header_t *curr = head;
	while(curr) {
		
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}

void free(void *block)
{
	header_t *header1, 
	header_t *temp;
	
	void *programbreak;

	if (!block)
		return;
	pthread_mutex_lock(&global_malloc_lock);
	header1 = (header_t*)block - 1;
	
	programbreak = sbrk(0);


	if ((char*)block + header->s.size == programbreak) {
		if (head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;
			while (temp) {
				if(temp->s.next == tail) {
					temp->s.next = NULL;
					tail = tmp;
				}
				tmp = temp->s.next;
			}
		}

		sbrk(0 - header1->s.size - sizeof(header_t));
	
		pthread_mutex_unlock(&global_malloc_lock);
		return;
	}
	header->s.is_free = 1;
	pthread_mutex_unlock(&global_malloc_lock);
}

void *malloc(size_t size)
{
	size_t totals;
	void *block;
	header_t *header;
	if (!size)
		return NULL;
	pthread_mutex_lock(&global_malloc_lock);
	header = get_free_block(size);
	if (header) 
	{

		header->s.is_free = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		return (void*)(header + 1);
	}

	totals = sizeof(header_t) + size;
	block = sbrk(total_size);
	if (block == (void*) -1) 
	{
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}
	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
	{
		head = header;
	}

	if (tail)
	{
		tail->s.next = header;
	}
	tail = header;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void*)(header + 1);
}

void *calloc(size_t num, size_t nsize)
{
	size_t size;
	void *block;
	if (!num || !nsize)
	{
		return NULL;
	}
	size = num * nsize;

	if (nsize != size / num)
		return NULL;
	block = malloc(size);
	if (!block)
	{
		return NULL;
	}
	memset(block, 0, size);
	return block;
}

void *realloc(void *block, size_t size)
{
	header_t *header;
	void *ret;
	if (!block || !size)
	{
		return malloc(size);
	}
	header = (header_t*)block - 1;
	if (header->s.size >= size)
	{
		return block;
	}
	ret = malloc(size);
	if (ret) 
	{

		memcpy(ret, block, header->s.size);

		free(block);
	}
	return ret;
}


void memorylistprint()
{
	header_t *current = head;
	printf("head = %p, tail = %p \n", (void*)head, (void*)tail);
	while(current) 
	{
		printf("addr = %p, size = %zu, is_free=%u, next=%p\n",
			(void*)current, current->s.size, current->s.is_free, (void*)current->s.next);
		current = current->s.next;
	}
}
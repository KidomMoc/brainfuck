#include "memory.h"

Pointer *malloc_P (size_t size)
{
	Pointer *p = malloc (sizeof (Pointer));
	p->rsize = 0;
	while (p->rsize <= size)
		p->rsize += BLOCK_SIZE;
	p->size = size;
	p->p = malloc (p->rsize);
	return p;
}

Pointer *realloc_P (Pointer *p, size_t size)
{
	if (size <= p->rsize)
	{
		p->size = size;
		return p;
	}
	else
	{
		Pointer *tmp = malloc (sizeof (Pointer));
		tmp->rsize = 0;
		while (tmp->rsize <= size)
			tmp->rsize += BLOCK_SIZE;
		tmp->p = malloc (tmp->rsize);
		tmp->p = memcpy (tmp->p, p->p, p->size);
		tmp->size = size;
		free_P (p);
		return tmp;
	}
}

Pointer *calloc_P (size_t size)
{
	Pointer *p = malloc (sizeof (Pointer));
	p->rsize = 0;
	while (p->rsize <= size)
		p->rsize += BLOCK_SIZE;
	p->size = size;
	p->p = malloc (p->rsize);
	for (size_t i = 0; i < p->rsize; ++i)
		((char*) p->p)[i] = 0;
	return p;
}

void free_P (Pointer *p)
{
	if (p)
	{
		if (p->p)
			free (p->p);
		free (p);
	}
}

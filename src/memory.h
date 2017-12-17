#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 256

/* Struct: Pointer 
 * p: pointer
 * size: used size
 * rsize: real size
 */

typedef struct pointer
{
	size_t size, rsize;
	void *p;
} Pointer;

/* Function: malloc_P
 * size: required size
 * return: a Pointer (size = required size, rsize = real size)
 */

Pointer *malloc_P (size_t size);

/* Function: realloc_P
 * p: original Pointer
 * size: new size
 * return: a Pointer (size = required size, rsize = real size)
 */

Pointer *realloc_P (Pointer *p, size_t size);

/* Function: calloc_P
 * size: required size
 * return a Pointer (size = required size, rsize = real size)
 */

Pointer *calloc_P (size_t size);

/* Function: free_P
 * p: the Pointer to free
 */

void free_P (Pointer *p);

#endif /* MEMORY_MANAGER_H */

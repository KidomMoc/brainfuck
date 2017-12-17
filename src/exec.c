#include "exec.h"

static Pointer *put (Pointer *p, int a);
static Pointer *pop (Pointer *p);

static Pointer *put (Pointer *p, int a)
{
	p = realloc_P (p, p->size + 1);
	((int*) p->p)[p->size - 1] = a;
	return p;
}

static Pointer *pop (Pointer *p)
{
	--p->size;
	return p;
}

int execute (char *code)
{
	Pointer *p = calloc_P (30000), *loop = malloc_P (0);
	int i = 15000;
	for (size_t j = 0; code[j] != '\0'; ++j)
		switch (code[j])
		{
			case '+':
				++((char*) p->p)[i];
				break;
			case '-':
				--((char*) p->p)[i];
				break;
			case '>':
				++i;
				break;
			case '<':
				--i;
				break;
			case ',':
				((char*) p->p)[i] = getc (stdin);
				while (getc (stdin) != '\n')
					;
				break;
			case '.':
				putc (((char*) p->p)[i], stdout);
				break;
			case '[':
				loop = put (loop, j);
				break;
			case ']':
				if (((char*) p->p)[i])
					j = ((int*) loop->p)[loop->size - 1];
				else
					loop = pop (loop);
				break;
			default:
				break;
		}

	free_P (p);
	free_P (loop);
}

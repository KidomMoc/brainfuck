#include "trans.h"

static Pointer *put (Pointer *p, char c);
static size_t count (const char *s, size_t *i, size_t len);
static char* toString (size_t n);
static Pointer *append (Pointer *org, const char *add);
static Pointer *generate_stack (const char *code);
static char *convert (const Pointer *stack);

static Pointer *put (Pointer *p, char c)
{
	p = realloc_P (p, p->size + 1);
	((char*) p->p)[p->size - 2] = c;
	((char*) p->p)[p->size - 1] = '\0';
	return p;
}

static size_t count (const char *s, size_t *i, size_t len)
{
	char c = s[*i];
	size_t co = 0;
	for ( ; *i < len && s[*i] == c; ++*i)
		++co;
	--*i;
	return co;
}

static char* toString (size_t n)
{
	char *s;
	size_t d, pow = 1;
	for (d = 1; n / (pow *= 10); ++d)
		;
	s = malloc (d + 1);
	for (size_t i = 0; i < d; ++i, n %= pow)
		s[i] = n / (pow /= 10) + 48;
	s[d] = '\0';
	return s;
}

static Pointer *append (Pointer *org, const char *add)
{
	size_t l = 0;
	for ( ; add[l] != '\0'; ++l)
		;

	org = realloc_P (org, org->size + l);
	for (size_t i = l; i > 0; --i)
	{
		((char*) org->p)[org->size - i - 1] = add[l - i];
	}
	((char*) org->p)[org->size - 1] = '\0';

	return org;
}

static Pointer *generate_stack (const char *code)
{
	Pointer *stack = malloc_P (1);
	((char*) stack->p)[0] = '\0';
	size_t nest = 0;
	for ( ; *code != '\0'; ++code)
		switch (*code)
		{
			case '+':
				if (stack->size - 1 && ((char*) stack->p)[stack->size - 2] == '-')
						--stack->size;
				else
					stack = put (stack, *code);
				break;
			case '-':
				if (stack->size - 1 && ((char*) stack->p)[stack->size - 2] == '+')
						--stack->size;
				else
					stack = put (stack, *code);
				break;
			case '>':
				if (stack->size - 1 && ((char*) stack->p)[stack->size - 2] == '<')
						--stack->size;
				else
					stack = put (stack, *code);
				break;
			case '<':
				if (stack->size - 1 && ((char*) stack->p)[stack->size - 2] == '>')
						--stack->size;
				else
					stack = put (stack, *code);
				break;
			case ',':
				stack = put (stack, *code);
				break;
			case '.':
				stack = put (stack, *code);
				break;
			case '[':
				++nest;
				stack = put (stack, *code);
				break;
			case ']':
				if (nest)
				{
					--nest;
					if (((char*) stack->p)[stack->size - 2] == '[')
						--stack->size;
					stack = put (stack, *code);
				}
				else
				{
					free_P (stack);
					return NULL;
				}
				break;
			default:
				break;
		}

	if (nest)
	{
		free_P (stack);
		return NULL;
	}
	return stack;
}

static char *convert (const Pointer *stack)
{
	Pointer *output = malloc_P (1);
	((char*) output->p)[0] = '\0';
	size_t nest = 0;
	char *tmp;
	for (size_t i = 0; ((char*) stack->p)[i] != '\0'; ++i)
		switch (((char*) stack->p)[i])
		{
			case '+':
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				output = append (output, "*p += ");
				tmp = toString (count ((char*) stack->p, &i, stack->size));
				output = append (output, tmp);
				free (tmp);
				output = append (output, ";\n");
				break;
			case '-':
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				output = append (output, "*p -= ");
				tmp = toString (count ((char*) stack->p, &i, stack->size));
				output = append (output, tmp);
				free (tmp);
				output = append (output, ";\n");
				break;
			case '>':
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				output = append (output, "p += ");
				tmp = toString (count ((char*) stack->p, &i, stack->size));
				output = append (output, tmp);
				free (tmp);
				output = append (output, ";\n");
				break;
			case '<':
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				output = append (output, "p -= ");
				tmp = toString (count ((char*) stack->p, &i, stack->size));
				output = append (output, tmp);
				free (tmp);
				output = append (output, ";\n");
				break;
			case ',':
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				output = append (output, "*p = getc ();\n");
				break;
			case '.':
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				output = append (output, "putc (*p);\n");
				break;
			case '[':
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				++nest;
				output = append (output, "while (*p)\n{\n");
				break;
			case ']':
				--nest;
				for (size_t j = 0; j < nest; ++j)
					output = append (output, "\t");
				output = append (output, "}\n");
				break;
			default:
				break;
		}
	return (char*) output->p;
}

char *translate (const char *code)
{
	Pointer *stack = generate_stack (code);
	if (!stack)
		return NULL;
	char *output = convert (stack);
	free_P (stack);
	return output;
}

#include "shell.h"
#include "trans.h"
#include "exec.h"
#include "memory.h"

static const char *EXEC = "execute";
static const char *TRANS = "translate";
static const char *HELP = "help";
static const char *EXIT = "exit";

static Pointer *strcat_P (Pointer *to, char *from);
static size_t get_command (char *input, char **command);
static void usage ();

Pointer *strcat_P (Pointer *to, char *from)
{
	size_t l, i;
	for (l = 0; from[l] != '\0'; ++l)
		;

	to = realloc_P (to, to->size + l);
	for (size_t i = l; i > 0; --i)
	{
		((char*) to->p)[to->size - i - 1] = from[l - i];
	}
	((char*) to->p)[to->size - 1] = '\0';

	return to;
}

static size_t get_command (char *input, char **command)
{
	size_t i;
	Pointer *com = malloc_P (1);
	char *tmp = malloc (2);
	tmp[1] = '\0';
	for (i = 0; input[i] != '\0' && i < 10 && input[i] != ' '; ++i)
	{
		tmp[0] = input[i];
		com = strcat_P (com, tmp);
	}
	*command = (char*) com->p;
	free (com);
	if (i != 10)
		return i + 1;
	else
	{
		*command[1] = '\0';
		return 0;
	}
}

void usage ()
{
	printf ("\
Usage: OPRATE CODE\n\n\
OPARATE:\n\
  exec    execute Brainfuck code\n\
  trans   translate Brainfuck code into C\n\
  help    display this help\n\
  exit    exit Shell (followed by nothing)\n");
}

void shell ()
{
	ssize_t len;
	size_t tmpsize, ind;
	while (1)
	{
		Pointer *s = malloc_P (1);
		((char*) s->p)[0] = '\0';
		char *tmp = malloc (1), *command, *code;
		tmp[0] = '\\';
		len = 1;
		printf (">> ");
		while (tmp[len - 1] == '\\')
		{
			tmp[len - 1] = '\0';
			s = strcat_P (s, tmp);
			len = getline (&tmp, &tmpsize, stdin);
			tmp[--len] = '\0';
		}
		s = strcat_P (s, tmp);

		ind = get_command ((char*) s->p, &command);
		code = (char*) s->p + ind;
		if (!strcmp (command, EXIT))
		{
			free (tmp);
			free (command);
			free_P (s);
			return;
		}
		else if (!strcmp (command, HELP))
			usage ();
		else if (!strcmp (command, TRANS))
			printf ("%s", translate (code));
		else
			execute (code);

		free (tmp);
		/*
		 * Warnning: lack of the next line will cause memory leak!
		free (command);
		*/
		free_P (s);
	}
}

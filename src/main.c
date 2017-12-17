#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "trans.h"
#include "exec.h"
#include "shell.h"
#include "memory.h"

#define VERSION "0.1"

static struct option const longopts[] = 
{
	{"execute", no_argument, NULL, 'e'},
	{"translate", no_argument, NULL, 't'},
	{"output", required_argument, NULL, 'o'},
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'v'},
	{NULL, 0, NULL, 0}
};

static void usage ();
static void version ();

static void usage ()
{
	printf ("\
Usage:  brainfuck\n\
  or:   brainfuck [OPTION]... FILE\n\
Execute or translate Brainfuck code. With no arguments,\n\
code shoule be enter in Shell Mode.\n\n\
OPTION:\n\
  -e, -execute      execute the Brainfuck code in FILE (default)\n\
  -t, -translate    translate the code in FILE into C\n\
  -h, -help         display this help and exit\n\
  -v, -version      display the version of the program and exit\n");
}

static void version ()
{
	printf ("brainfuck %s by Kidom", VERSION);
}

int main (int argc, char **argv)
{
	if (argc == 1)
		shell ();
	else
	{
		int exec = 0, trans = 0, out = 0, i;
		char c, *output = NULL;
		Pointer *input = malloc_P (1);
		((char*) input->p)[0] = '\0';
		FILE *is, *os;
		while ((c = getopt_long (argc, argv, "eto:", longopts, NULL)) != -1)
			switch (c)
			{
				case 0:
					break;
				case 'e':
					exec = 1;
					break;
				case 't':
					trans = 1;
					break;
				case 'o':
					output = optarg;
					break;
				case 'h':
					usage ();
					return 0;
				case 'v':
					version ();
					return 0;
				case '?':
					usage ();
					return 1;
				default:
					break;
			}

		if (optind == argc)
		{
			usage ();
			return 1;
		}

		is = fopen (argv[optind], "r");
		while ((c = getc (is)) != EOF)
		{
			input = realloc_P (input, input->size + 1);
			((char*) input->p)[input->size - 2] = c;
			((char*) input->p)[input->size - 1] = '\0';
		}
		fclose (is);

		if (trans)
		{
			if (!output)
				os = fopen ("output.c", "w");
			else
				os = fopen (output, "w");

			if (exec)
				execute ((char*) input->p);
			output = translate ((char*) input->p);

			for ( ; *output != '\0'; ++output)
				putc (*output, os);

			fclose (os);
		}
		else
			execute ((char*) input->p);

		free_P (input);
	}

	return 0;
}

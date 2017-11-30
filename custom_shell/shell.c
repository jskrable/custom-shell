/* 
 * Jack Skrable
 * Custom Shell 
 * CS 575
 * 10/31/17
 */

#define _GNU_SOURCE
#define TOK_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* list of built in commands, maintained manually */
char *builtin_cmds[] = 
{
	"cd",
	"help",
	"exit"
};

/* get count of built in command array */
int builtin_count = sizeof(*builtin_cmds) / sizeof(builtin_cmds[0]);

/* built in help function */
int shell_help(char **args)
{
	printf("This is Jack Skrable's custom shell\n");
	printf("Shoot me a command and see what I can do.\n");
	printf("The following programs are built in:\n");

	int i;
	for (i=0; i < builtin_count; i++) {
		printf(" %s\n", builtin_cmds[i]);
	}

	printf("Please use the standard man command for info \n");
	printf("on external programs.\n");
	return 1;
}

/* built in exit function */
int shell_exit(char **args)
{
	return 0;
}

/* reads user entered command line */
char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t read = getline(&line, &bufsize, stdin);
	if (read <= 0) {
		fprintf(stderr, "Issue reading command. Did you enter anything?\n");
		exit(EXIT_FAILURE);
	}	

	return line;
}

/* breaks down read_line string into arguments */
char **parse_line(char *line)
{
	int bufsize = TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "Memory allocation error.\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += TOK_BUFSIZE;
		}

	token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;

}

int shell_execute(char **args) 
{
	int i;
	
	if (args[0] == NULL) {
		return 1;
	}

	for (i = 0; i < builtin_count; i++) {
		if (strcmp(args[0], builtin_cmds[i]) == 0) {
		/* EXECUTE BUILT IN FUNCTIONS HERE */
		}
	}

	/* CALL EXTERNAL PROGRAMS HERE */
	return 0;
}

/* main loop to read, parse, execute */
void shell_loop(void)
{
	char *line;
	char **args;

	while(1)
	{
		printf("JS>");
		line = read_line();
		/* printf("You entered: %s", line); */
		args = parse_line(line);
		
	};


}

int main (int argc, char **argv) 
{

	shell_loop();

	return EXIT_SUCCESS;
			
}












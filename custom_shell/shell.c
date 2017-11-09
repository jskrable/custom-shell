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

char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t read = getline(&line, &bufsize, stdin);
	if (read < 0) {
		fprintf(stderr, "command read issue\n");
		exit(EXIT_FAILURE);
	}	

	return line;
}


char **parse_line(char *line)
{
	int bufsize = TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "memory allocation issue\n");
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


void shell_loop(void)
{
	char *line;
	char **args;

	printf("JS>");
	line = read_line();
	printf("You entered: %s", line);

	args = parse_line(line);



}

int main (int argc, char **argv) 
{

	shell_loop();

	return EXIT_SUCCESS;
			
}












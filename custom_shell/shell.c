/* 
 * Jack Skrable
 * Custom Shell 
 * CS 575
 * 10/31/17
 */

#define _GNU_SOURCE
/*#define TOK_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a" 
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

/* list of built in commands, maintained manually */
char *builtin_cmds[] = 
{
	"help",
	"exit",
	"cd",
	"dir"
};

/* get count of built in command array */
int builtin_count = sizeof(*builtin_cmds) / sizeof(builtin_cmds[0]);

/* built in change directory function */
int shell_cd(char **args)
{
	if (args[1] == NULL) {
		fprintf(stderr, "Expected an argument\n");
	} else {
		if (chdir(args[1]) != 0) {
		perror("chdir() error: ");
		}
	}
	return 1;
}

/* built in list working directory function */
int shell_dir(char **args)
{
	long size;
	char *buf;
	char *ptr;

	size = pathconf(".", _PC_PATH_MAX);

	if ((buf = (char *)malloc((size_t)size)) != NULL)
		ptr = getcwd(buf, (size_t)size);
		printf("%s\n", ptr);
	
	return 1;
}

/* built in help function
	CHANGE TO HARDCODED LIST OF FUNCTIONS AND DESCRIPTIONS
*/
int shell_help(char **args)
{
	printf("This is Jack Skrable's custom shell\n");
	printf("Shoot me a command and see what I can do.\n");
	printf("The following programs are built in:\n");

	int i;
	for (i = 0; i <= builtin_count; i++) {
		printf(" %s\n", builtin_cmds[i]);
	}

	printf("Please use the standard man command for\n");
	printf("info on external programs.\n");
	return 1;
}

/* built in exit function */
int shell_exit(char **args)
{
	exit(0);
}

/* built in history function 

int shell_history(char **args)
{
	struct node
	{	
		int data;
		struct node *next;
	};
	

}
*/

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

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **parse_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/*
char **parse_line(char *line)
{
	char delims[] = " \t\r\n\a";
	char **commands = malloc(100 * sizeof(char*));
	int counter = 0;
	char *place = strtok(line, delims);
	
	if (!commands) {
		fprintf(stderr, "There was an error during memory allocation.\n");
		exit(EXIT_FAILURE);
	}
	
	while (place != NULL) {
		commands[counter] = place;
		counter++;

		commands = realloc(commands, sizeof(char*));

		if (!commands) {
			fprintf(stderr, "There was an error during memory allocation.\n");
			exit(EXIT_FAILURE);
		}
	}

	commands[counter] = NULL;

	return commands;

}
*/

int exec_external(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			perror("execvp() error: ");
		}
		exit(EXIT_FAILURE);

	} else if (pid < 0) {
		perror("fork() error: ");
	} else {
		waitpid(pid, &status, WUNTRACED);
	}

	return 1;
		
}

int shell_execute(char **args) 
{
	int builtin;

	if (args[0] == NULL) {
		printf("Please enter a command next time.\n");
		return 1;
	} else if (strcmp(args[0], "exit") == 0) {
		shell_exit(args);
	} else if (strcmp(args[0], "help") == 0) {
		shell_help(args);
	} else if (strcmp(args[0], "cd") == 0) {
		shell_cd(args);
	} else if (strcmp(args[0], "dir") == 0) {
		shell_dir(args);
	} else {
		exec_external(args);
	}
	
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
		args = parse_line(line);
		/* printf("%s\n", (char)args[1]); */
		shell_execute(args);
		/* add cmd to history array */
	};


}

int main (int argc, char **argv) 
{

	shell_loop();

	return EXIT_SUCCESS;
			
}












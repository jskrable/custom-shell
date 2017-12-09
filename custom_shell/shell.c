/* 
	author: Jack Skrable
	project: custom_shell
	description: Custom shell built for unix-based systems as final project
	for CS 575 Operating Sys Concepts @ BU.
	version: 2.0
	date: 12/08/17
*/

#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>

/* intialize data structure for history linked list node */
struct Node 
{
	char *command;
	int counter;
	struct Node *next;
};

/* initialize head of history list and counter */
struct Node* head = NULL;
int hist_cnt = 1;

/* forward function declarations */
int history_execute(char *line);

/* built in help function, updated manually w/ descriptions */
int shell_help(char **args)
{
	printf("Welcome to Jack Skrable's custom shell\n");
	printf("--------------------------------------------------------------------\n");
	printf("Shoot me a command and see what I can do. These commands are built in:\n");
	printf("\n");
	printf("\"help\" brings up this display.\n");
	printf("\"cd\" changes directories. Be sure to pass in the full path as an argument.\n");
	printf("\"dir\" lists the current directory.\n");
	printf("\"history\" shows the command history\n");
	printf("\"!!\" executes the last entered command.\n");
	printf("\"!n\" executes the n-th command.\n");
	printf("\"exit\" seems self-explnatory, this one does.\n");
	printf("\n");
	printf("Please use the standard man command for info on external commands\n");
	return 1;
}

/* add a command to history data structure */
int history_commit(struct Node** head_ref, char *line, int hist_cnt)
{
	/* create new node and allocate mem */
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

	struct Node *last = *head_ref;

	/* add command input and counter to node */
	new_node->command = line;
	new_node->counter = hist_cnt;
	new_node->next = NULL;

	/* set node to head if first one */
	if (*head_ref == NULL)
	{
		*head_ref = new_node;
		return 1;
	}
	
	/* get to end of list */
	while (last->next != NULL) 
	{
		last = last->next;
	}

	last->next = new_node;
	
	hist_cnt++;
	return hist_cnt;
}

/* print out full command history for session */
int history_print(struct Node *node)
{
	if (node == NULL)
	{
		printf("There is no history yet. Try something.\n");
	}
	
	while (node != NULL) 
	{
		printf(" %d | %s \n", node->counter, node->command);
		node = node->next;
	}

	return 1;
}

/* execute the last command entered from history */
int execute_last(struct Node *node)
{
	if (node == NULL) 
	{
		printf("There is no last command to execute.\n");
	}

	/* traverse to final node */	
	while (node->next != NULL) 
	{
		node = node->next;
	}

	/* send final command to history_execute function */
	history_execute(node->command); 
	return 1;
}

/* execute the nth command entered from history */
int execute_nth(struct Node *node, char *line)
{
	char *str = line, *p = str;
	char *command;
	long n;
	
	/* loop through line and grab long int */
	while (*p) {
    	if (isdigit(*p)) 
		{
        		n = strtol(p, &p, 10);
    	} else 
		{
        		p++;
    	}
	}
	
	/* run through linked list and check if n matches a command number */
	while (node != NULL) 
	{
		if(n == node->counter) 
		{
			command = node->command;
		}
		node = node->next;
	}

	/* display error if n does not exist in history, otherwise execute */	
	if (command == NULL) 
	{
		printf("There is no command at location: %ld \n", n);
	} else
	{
		history_execute(command);
	}
	return 1;	
}


/* built in change directory function */
int shell_cd(char **args)
{
	if (args[1] == NULL) 
	{
		fprintf(stderr, "Please include path to change into as argument.\n");
	} else 
	{
		if (chdir(args[1]) != 0) 
		{
		/*fprintf(stderr, "Error changing directory\n");*/
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

/* built in exit function */
int shell_exit(char **args)
{
	exit(0);
}

/* reads user entered command line */
char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t read = getline(&line, &bufsize, stdin);
	if (read <= 0) 
	{
		fprintf(stderr, "Issue reading command. Did you enter anything?\n");
		exit(EXIT_FAILURE);
	}	

	return line;
}

/* SNAGGED PARSE LINE FUNCTION TO DROP */
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **parse_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) 
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) 
  {
    tokens[position] = token;
    position++;

    if (position >= bufsize) 
	{
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) 
	  {
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
*********** CUSTOM PARSE_LINE FUNCTION TO FIX **************

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

/* executes all external commands 
	ANNOTATE AND CHANGE???
*/
int exec_external(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0) 
	{
		if (execvp(args[0], args) == -1) 
		{
			perror("execvp() error: ");
		}
		exit(EXIT_FAILURE);

	} else if (pid < 0) 
	{
		perror("fork() error: ");
	} else {
		waitpid(pid, &status, WUNTRACED);
	}

	return 1;
		
}

/* conditional to check for built-in commands, or send to external execution */
int shell_execute(char **args, char *line) 
{

	if (args[0] == NULL) 
	{
		printf("Please enter a command next time.\n");
		return 1;
	} else if (strcmp(args[0], "exit") == 0) 
	{
		shell_exit(args);
	} else if (strcmp(args[0], "help") == 0) 
	{
		shell_help(args);
	} else if (strcmp(args[0], "cd") == 0) 
	{
		shell_cd(args);
	} else if (strcmp(args[0], "dir") == 0) 
	{
		shell_dir(args);
	} else if (strcmp(args[0], "history") == 0) 
	{
		history_print(head);
	} else if (strcmp(args[0], "!!") == 0) 
	{
		execute_last(head);
	} else if (strncmp(args[0], "!", 1) == 0) 
	{
		execute_nth(head, line);
	} else 
	{
		exec_external(args);
	}
	
	return 0;
}

/* bypass read line and pass historical command into execute */
int history_execute(char *line)
{
	char **args;

	args = parse_line(line);
	shell_execute(args, line);

	return 1;
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
		if (shell_execute(args, line) == 0)
		{
			hist_cnt = history_commit(&head, line, hist_cnt);
		}
	};


}

int main (int argc, char **argv) 
{
	shell_loop();
	return EXIT_SUCCESS;			
}













/*
 * This code implements a simple shell program
 * It supports the internal shell command "exit", 
 * backgrounding processes with "&", input redirection
 * with "<" and output redirection with ">".
 * However, this is not complete.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

extern char **getaline();

typedef struct pid_node_s {
	int pid;
	struct pid_node_s* next;
} pid_node_t;

pid_node_t* waitlist;

int internal_command(char** args);
int ampersand(char** args);
int redirect_input(char **args, char **input_filename);
int redirect_output(char **args, char **output_filename);
int redirect_pipe(char **args1, char ***args2);
void parse_command(char** args);
void do_command(char **args, int block, int input, char *input_filename, int output, char *output_filename, int pipe, int* fd);
void waitlist_push(pid_node_t* n, int pid);
void waitlist_wait(pid_node_t* n);

/*
 * Handle exit signals from child processes
 */
void sig_handler(int signal) {
	int status;
	int result = wait(&status);

	printf("Wait returned %d\n", result);
}

/*
 * The main shell function
 */ 
int main(int argc, char** argv) {
	int i;
	char **args; 

	// Set up the signal handler
	sigset(SIGCHLD, sig_handler);

	// Init waitlist
	waitlist = NULL;

	// Loop forever
	while(1) {

		// Print out the prompt and get the input
		printf("->");
		args = getaline();

		// No input, continue
		if(args[0] == NULL)
			continue;

		// Check for internal shell commands, such as exit
		if(internal_command(args))
			continue;

		parse_command(args);
	}
}

// FIXME: dumb name
// should we use number of args??
// TODO: add pipe output
void parse_command(char** args) {
	int result;
	int block;
	int output;
	int input;
	int pipe;
	int fd[2];
	char *output_filename;
	char *input_filename;
	char **pipeargs;

	// Check for an ampersand
	block = (ampersand(args) == 0);

	// Check for redirected input
	input = redirect_input(args, &input_filename);

	switch(input) {
	case -1:
		printf("Syntax error!\n");
		return;
		break;
	case 0:
		break;
	case 1:
		printf("Redirecting input from: %s\n", input_filename);
		break;
	}

	// Check for redirected output
	output = redirect_output(args, &output_filename);

	switch(output) {
	case -1:
		printf("Syntax error!\n");
		return;
		break;
	case 0:
		break;
	case 1:
		printf("Redirecting output to: %s\n", output_filename);
		break;
	}

	pipe = redirect_pipe(args, &pipeargs);

	if(pipe) {
		printf("PIPE!\n");	// TODO: replace with something less stupid
		// TODO: use pipe() and then pass the related file descriptors to do_command
		parse_command(pipeargs);	// recursive call to deal with piped commands TODO: add piped file descriptor
	}

	// Do the command
	do_command(args, block, 
			 input, input_filename, 
			 output, output_filename,
			 pipe, fd);

	waitlist_wait(waitlist);
}

/*
 * Check for ampersand as the last argument
 */
int ampersand(char **args) {
	int i;

	for(i = 1; args[i] != NULL; i++) ;

	if(args[i-1][0] == '&') {
		free(args[i-1]);
		args[i-1] = NULL;
		return 1;
	} else {
		return 0;
	}

	return 0;
}

/* 
 * Check for internal commands
 * Returns true if there is more to do, false otherwise 
 */
int internal_command(char **args) {
	if(strcmp(args[0], "exit") == 0) {
		exit(0);
	}

	return 0;
}

/* 
 * Do the command
 */
void do_command(char **args, int block,
		int input, char *input_filename,
		int output, char *output_filename,
		int pipe, int* fd) {

	int result;
	pid_t child_id;
	int status;

	// Fork the child process
	child_id = fork();

	// Check for errors in fork()
	switch(child_id) {
	case EAGAIN:
		perror("Error EAGAIN: ");
	case ENOMEM:
		perror("Error ENOMEM: ");
	}

	if(child_id == 0) {

		// Set up redirection in the child process
		if(input)
			freopen(input_filename, "r", stdin);

		// TODO: no append
		if(output)
			freopen(output_filename, "w+", stdout);

		// Execute the command
		result = execvp(args[0], args);

		exit(-1);
	}

	// Wait for the child process to complete, if necessary
	if(block) {
		waitlist_push(waitlist, child_id);
	}
}

/*
 * Check for input redirection
 */
int redirect_input(char **args, char **input_filename) {
	int i;
	int j;

	for(i = 0; args[i] != NULL; i++) {

		// Look for the <
		if(args[i][0] == '<') {
			free(args[i]);

			args[i] = NULL;

			// Read the filename
			if(args[i+1] != NULL) {
				*input_filename = args[i+1];
			} else {
				return -1;
			}

			// Adjust the rest of the arguments in the array
			// FIXME: what does this even do?
			for(j = i; args[j-1] != NULL; j++) {
				args[j] = args[j+2];
			}

			return 1;
		}
	}

	return 0;
}

/*
 * Check for output redirection
 */
int redirect_output(char **args, char **output_filename) {
	int i;
	int j;

	for(i = 0; args[i] != NULL; i++) {

		// Look for the >
		if(args[i][0] == '>') {
			free(args[i]);

			args[i] = NULL;

			// Get the filename 
			if(args[i+1] != NULL) {
				*output_filename = args[i+1];
			} else {
				return -1;
			}

			// Adjust the rest of the arguments in the array
			for(j = i; args[j-1] != NULL; j++) {
				args[j] = args[j+2];
			}

			return 1;
		}
	}

	return 0;
}

// FIXME: unfinished func
// TODO: pointer to second half of args?  
int redirect_pipe(char **args1, char ***args2) {
	int i;
	int j;

	for(i = 0; args1[i] != NULL; i++) {

		// Look for the >
		if(args1[i][0] == '|') {
			free(args1[i]);

			args1[i] = NULL;

			// Get the filename 
			if(args1[i+1] != NULL) {
				*args2 = &args1[i+1];
			} else {
				return -1;
			}

			return 1;
		}
	}

	return 0;
}

void waitlist_push(pid_node_t* n, int pid) {
	while (n != NULL)
		n = n->next;

	n = malloc(sizeof(pid_node_t));
	n->pid = pid;
	n->next = NULL;
}

void waitlist_wait(pid_node_t* n) {
	if(n != NULL)
		waitlist_wait(n->next);

	int status;

	printf("Waiting for child, pid = %d\n", n->pid);
	waitpid(n->pid, &status, 0);
}


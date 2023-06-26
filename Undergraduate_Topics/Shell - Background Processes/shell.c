//
//  a1.c
//  360_a1
//
//  Created by Kiefer Aland on 2020-05-21.
//  For CSC 360 @ University of Victoria.

//Includes -------------------------------------------
#include<sys/wait.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

//DEFS -----------------------------------------------
#ifndef MAX_PARAMS
#define MAX_PARAMS 16
#endif

#ifndef NUM_FUNCS
#define NUM_FUNCS 3
#endif

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 100
#endif

#ifndef MAX_PATH_LENGTH
#define MAX_PATH_LENGTH 256
#endif

#ifndef MAX_BGS
#define MAX_BGS 5
#endif

//Function Prototypes---------------------------------
int read_cmdLine();
int split_cmdLine(char* cmdLine);
int get_num_tokens(char** cmd_tokens);
int token_is_command(char* token);
int create_process(char** cmd_tokens);
int run_loop();
int get_cwd();
int change_directory(char**);
int list(char**);
int pwd(char**);
int split_hostname();
int getlogin_r(char *buf, size_t bufsize);
int check_bg_statuses();
int list_background_processes();
int add_process(pid_t pid, int status, char * cmd);
int kill_process();
int execute_cmd(char** cmd_tokens);
int check_cmd(char** cmd_tokens);
int take_five();

//structs
struct Process{
	int status;
	pid_t pid;
	bool is_stopped;
	bool is_complete;
	bool may_continue;
	char * cmd;
	bool is_set;
}p1, p2, p3, p4, p5;

//Global Vars
static char* cmd_tokens[16] = {NULL};
static char * cmd_names[] = {"cd", "pwd"};
static int num_cmds = sizeof(cmd_names)/sizeof(char*);
static int (*cmd_functions[2])(char**) = {&change_directory, &pwd};
static char* cwd;
static char hostname[HOST_NAME_MAX];
static char username[HOST_NAME_MAX];
static int cmd_num;
static char* cmdLine;
static int num_bgs = 0;
extern int errno;
struct Process* bg_array[5] = {&p1, &p2, &p3, &p4, &p5};

int signal_child(pid_t pid, int sig_num){
	int signal;
	if (!(sig_num == -1 || sig_num == 0 || sig_num == 1)) {
		printf("error: send_sig --> invalid arg: sig_num not member of set {-1,0,1} note: see comments in code\n");
		return -1;
	}else{
		switch (sig_num){
			case -1:
				signal = SIGTERM;
				break;
			case 0:
				signal = SIGSTOP;
				if (bg_array[atoi(cmd_tokens[1])] -> is_stopped == true){
					printf("error: process [%d] is already stopped.\n",bg_array[atoi(cmd_tokens[1])] -> pid);
					break;
				}
				break;
			case 1:
				signal = SIGCONT;
				//printf("may cont? : %s\n", bg_array[atoi(cmd_tokens[1])] -> may_continue ? "TRUE" : "FALSE");
				if (bg_array[atoi(cmd_tokens[1])] -> may_continue == true){
					printf("error: process [%d] is already running.\n",bg_array[atoi(cmd_tokens[1])] -> pid);
					break;
				}
				bg_array[atoi(cmd_tokens[1])] -> may_continue = true;
				break;
		}
	}
	kill(pid, signal);
	return 0;
}

int take_five(){
	sleep(1);
	printf("5\n");
	sleep(1);
	printf("4\n");
	sleep(1);
	printf("3\n");
	sleep(1);
	printf("2\n");
	sleep(1);
	printf("1\n");
	sleep(1);
	return 0;
}

int clean_processes(){
	int i;
	for(i = 0; i < 5; i++){
		if (bg_array[i] -> is_set == true){
			if(bg_array[i] -> is_complete == true){
				bg_array[i] -> status = 0;
				bg_array[i] -> pid = 0;
				bg_array[i] -> is_stopped = false;
				bg_array[i] -> is_complete = false;
				bg_array[i] -> may_continue = true;
				bg_array[i] -> cmd = NULL;
				bg_array[i] -> is_set = false;
				num_bgs--;
			}
		}
	}
	return 0;
}

int token_is_command(char* token){
	int i;
	if (!token){
		return -1;
	}
	for (i = 0; i < num_cmds; i++){
		if (strcmp(token, cmd_names[i]) == 0){
			cmd_num = i;
			return 0;
		}
	}
	return 1;
}

int check_cmd(char** cmd_tokens){
	if(strcmp(cmd_tokens[0], "cd") == 0){
		if (cmd_tokens[0] == NULL){
			printf("cd expects another argument [path]\n");
		}else{
			change_directory(cmd_tokens);
		}
		return 1;

	}else if(strcmp(cmd_tokens[0], "pwd") == 0){
		printf("%s\n", cwd);
		return 1;
	}
	return 1;
}

int get_num_tokens(char** cmd_tokens){
	int i = 0;
	while (cmd_tokens[i] != NULL){
		i++;
	}
	return i;
}

int get_cwd(){
	char* buf[256] = {NULL};
	size_t bufSize = sizeof(buf)/sizeof(char*);
	cwd = getcwd(buf[0], bufSize);
	return 1;
}

int read_cmdLine(){
	//check_bg_statuses();
	gethostname(hostname, HOST_NAME_MAX);
	getlogin_r(username, HOST_NAME_MAX);
	get_cwd();
	char* last_token_cwd = strrchr(cwd, '/');
	if (last_token_cwd != NULL) {
    	last_token_cwd++;
	}else{
		printf("error: cwd_last_token is null in read_cmdLine.\n");
		return -1;
	}
	split_hostname();
	char* prompt_string = strcat(hostname, ":");
	prompt_string = strcat(prompt_string, last_token_cwd);
	prompt_string = strcat(prompt_string, " ");
	prompt_string = strcat(prompt_string, username);
	prompt_string = strcat(prompt_string, ">");
	cmdLine = readline(prompt_string);
	if (strcmp(cmdLine, "") == 0){
		return -1;
	}
	return 0;
}

int reset_cmd_tokens(){
	int i;
	for (i = 0; i < MAX_PARAMS; i++){
		cmd_tokens[i] = NULL;
	}
	return 1;
}

int split_cmdLine(char* cmdLine){
	if(cmdLine == NULL){
		return -1;
	}
	int i = 0;
	//get the first token
	char* token = strtok(cmdLine, " ");
	//get the rest
	while(token != NULL){
		cmd_tokens[i] = token;
		token = strtok(NULL, " ");
		if (i >= 16){
			printf("Warning: There was more than 16 tokens!!! ---- not all args saved\n");
			return 1;
		}
		i++;
	}
	return 1;
}

int split_hostname(){
	if(!*hostname){
		printf("error: hostname is null\n");
		return -1;
	}
	*hostname = *strtok(hostname, ".");
	return 1;
}

int change_directory(char** cmd_tokens){
	char* home_directory = getenv("HOME");
	if (cmd_tokens[1] == NULL){
		printf("error: cd expects more args (ex: shell> [cd] [path])\n");
		return -1;
	}else if(chdir(cmd_tokens[1]) == 0){
		return 1;
	}else{
		if (*cmd_tokens[1] == '~'){
			chdir(home_directory);
			return 1;
		}
		printf("error: chdir failed for some reason\n");
		return -1;
	}
	return 1;
}

int pwd(char** cmd_tokens){
	if (cwd == NULL){
		printf("error: cwd is null and pwd was called\n");
		return -1;
	}
	printf("%s\n", cwd);
	return 1;
}

int list_background_processes(){
	int i;
	char* path = getenv("PATH");
	char* last_token_path = strrchr(path, '/');
	bool new_line = false;
	if (last_token_path != NULL) {
    	last_token_path++;
	}else{
		printf("error: path_last_token is null in read_cmdLine.\n");
		return -1;
	}
	for (i = 0; i < 5; i++){
		if(!new_line){
			printf("\n");
			new_line = true;
		}
		if(bg_array[i] -> is_set == true){
			printf("%d[%s]: %s [%d]\n", i, (((bg_array[i] -> is_stopped && bg_array[i] -> may_continue == false) || bg_array[i] -> is_complete) ? "S" : "R"), bg_array[i] -> cmd, bg_array[i] -> pid);
		}
	}
	printf("\nTotal Background Jobs: %d\n\n", num_bgs);
	return 0;
}

int get_updates(){
	int i;
	for (i = 0; i < 5; i++){
		if (bg_array[i] -> is_set == true){
			pid_t pid = bg_array[i] -> pid;
			int status = bg_array[i] -> status;
			pid_t ret_pid = waitpid(pid, &status, WNOHANG | WUNTRACED);
			bg_array[i] -> status = status;
			if (ret_pid == -1){
				if(errno != ECHILD){
					perror("error: get_updates");
				}
			}
			if (WIFEXITED(status)){
				if(WSTOPSIG(status) == 255){
					bg_array[i] -> is_complete = true;
					bg_array[i] -> may_continue = false;
					printf("process [%d] was garbage and error should have been caught elsewhere\n", pid);
				}else if (errno == ECHILD && ret_pid == -1){
					bg_array[i] -> is_complete = true;
					bg_array[i] -> may_continue = false;
					printf("process [%d] exited\n", pid);
				}
			}
			if (WIFSTOPPED(status)){
				bg_array[i] -> is_stopped = true;
				bg_array[i] -> may_continue = false;
				if (WSTOPSIG(status) == SIGCONT){
					bg_array[i] -> is_stopped = false;
					bg_array[i] -> may_continue = true;
					printf("process [%d] may continue\n", pid);
				}
			}
			if (WIFSIGNALED(status)){
				if (WTERMSIG(status) == SIGCONT){
						if(bg_array[i] -> is_complete == false){
							bg_array[i] -> is_stopped = false;
							bg_array[i] -> may_continue = true;
							printf("process [%d] will continue\n", pid);
						}
				}else if (WTERMSIG(status) == 15){
					bg_array[i] -> is_stopped = true;
					bg_array[i] -> is_complete = true;
					bg_array[i] -> may_continue = false;
					printf("process [%d] was killed\n", pid);
					
				}
			}
		}
	}
	return 0;
}

int execute_cmd(char** cmd_tokens){
	if (cmd_tokens[0] == NULL){
		return -1;
	}else{
		int x = execvp(cmd_tokens[0], cmd_tokens);
		if (x < 0){	
			printf("error: execution error\n");
			return -1;
		}
		return 1;
	}
}
int add_process(pid_t pid, int status, char * cmd){
	if (num_bgs >= 5){
		printf("error: add_bg_process --> cannot create sixth background process (max = 5)\n");
		return -1;
	}
	int i; 
	for (i = 0; i < 5; i++){
		if(bg_array[i] -> is_set == false){
			bg_array[i] -> pid = pid;
			bg_array[i] -> status = status;
			bg_array[i] -> cmd = cmd;
			bg_array[i] -> is_stopped = false;
			bg_array[i] -> is_complete = false;
			bg_array[i] -> may_continue = true;
			bg_array[i] -> is_set = true;
			return 0;
		}
	}
	return 1;
}
int create_process(char** cmd_tokens){
	if (strcmp(cmd_tokens[0], "bglist") == 0){
		list_background_processes();
		return 0;
	}else if (strcmp(cmd_tokens[0], "bgkill") == 0){
		const char* kill_num_string = cmd_tokens[1];
		int array_num_to_kill = atoi(kill_num_string);
		if(array_num_to_kill < 0 || array_num_to_kill > 4){
			printf("error: send_sig --> invalid arg: sig_num not member of set {0,1,2,3,4} note: see comments in code\n");
		}
		pid_t pid_to_kill = bg_array[array_num_to_kill] -> pid;
		signal_child(pid_to_kill, -1);
		return 0;
	}else if (strcmp(cmd_tokens[0], "stop") == 0){
		const char* stop_num_string = cmd_tokens[1];
		int array_num_to_stop = atoi(stop_num_string);
		pid_t pid_to_stop = bg_array[array_num_to_stop] -> pid;
		signal_child(pid_to_stop, 0);
		return 0;
	}else if (strcmp(cmd_tokens[0], "start") == 0){
		const char* start_num_string = cmd_tokens[1];
		int array_num_to_start = atoi(start_num_string);
		pid_t pid_to_start = bg_array[array_num_to_start] -> pid;
		signal_child(pid_to_start, 1);
		return 0;
	}else if(strcmp(cmd_tokens[0], "bg") == 0){
		if (num_bgs < MAX_BGS){
			pid_t pid = fork();
			if(pid < 0){
				perror("error: fork");
				return -1;
			}else if(pid == 0){
				int x = execute_cmd(cmd_tokens+1);
				if (x < 0){
					printf("error: cmd not recognized\n");
					exit(-1);
				}
				printf("error: child exited but was not caught by error handler god help you.\n");
				exit(0);
			}else if (pid > 0){
				int status;
				pid_t cpid = pid;
				waitpid(pid, &status, WNOHANG | WUNTRACED);
				add_process(cpid, status, cmd_tokens[1]);
				num_bgs++;
				return 0;
			}
		}else{
			printf("could not create sixth background process\n");
			return -1;
		}
	}else if(token_is_command(cmd_tokens[0]) == 0){
		if (check_cmd(cmd_tokens) < 0){
			cmd_functions[cmd_num](cmd_tokens);
		}
	}else if(token_is_command(cmd_tokens[0]) == 1){
		pid_t pid = fork();
		if(pid < 0){
			perror("error: fork");
			return -1;
		}else if(pid == 0){
			int x = execute_cmd(cmd_tokens);
			if (x < 0){
				printf("error: cmd not recognized\n");
				exit(1);
			}
			exit(-1);
		}else if(pid > 0){
			waitpid(pid, NULL, 0);
			return 0;
		}
	}else{
		printf("howd we get in this pickle?\n");
		return -1;
	}
	return 1;
}

int run_loop(){
	for (;;){
		reset_cmd_tokens();
		get_updates();
		clean_processes();
		int read_cmdLine_return = read_cmdLine();
		if (read_cmdLine_return < 0){
			continue;
		}
		if (split_cmdLine(cmdLine) < 0){
			printf("error: split_cmdLine failed in run_loop\n");
			return -1;
		}
		create_process(cmd_tokens);
	}
	return 0;
}

int main (void){	
    run_loop();
    return -1;
}
//end of file

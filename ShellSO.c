#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define	MAX_SIZE_CMD	256
#define	MAX_SIZE_ARG	16

char cmd[MAX_SIZE_CMD];				
char *argv[MAX_SIZE_ARG];			
pid_t pid;										
pid_t pidlista[256];							
char i;												
void get_cmd();								
void convert_cmd();						
void c_shell();							
void log_handle(int sig);			
void addpath(char *argv[]);
int main(){


	c_shell();

	return 0;
}

void c_shell(){
	while(1){


		get_cmd();

		if(!strcmp("", cmd)) continue;

        if(!strcmp("exit", cmd)) kill(0, SIGKILL);

		convert_cmd();


		pid = fork();
		if(-1 == pid){
			printf("failed to create a child\n");
		}
		else if(0 == pid){
			// printf("Filho");
			// execute a command
			if(!strcmp("path", argv[0]))addpath(argv);
			execvp(argv[0], argv);
		}
		else{
			// printf("Pai");
			if(NULL == argv[i]) waitpid(pid, NULL, 0);
		}
	}
}

void get_cmd(){
	printf("@: ");
	fgets(cmd, MAX_SIZE_CMD, stdin);

	if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n'))
        	cmd[strlen (cmd) - 1] = '\0';

}

void convert_cmd(){
	char *ptr;
	i = 0;
	ptr = strtok(cmd, " ");
	while(ptr != NULL){
		//printf("%s\n", ptr);
		argv[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}

	if(!strcmp("&", argv[i-1])){
	argv[i-1] = NULL;
	argv[i] = "&";
	}else{
	argv[i] = NULL;
	}
	//printf("%d\n", i);
}


void addpath(char *argv[]){

	const char *path_og = getenv("PATH");
	const char *path_ng = argv[1];
	char buf[4096];

	//printf("OLD PATH: %s\n", getenv("PATH"));

	 if (snprintf(buf, sizeof(buf), "%s:%s", path_og, path_ng) >= sizeof(buf)) {
        fprintf(stderr, "Erro: PATH muito comprido\n");
		return;

    }
	 if (setenv("PATH", buf, 1) != 0) {
        perror("setenv");
        return;
    }
	//printf("NOVO PATH: %s\n", getenv("PATH"));
	
}

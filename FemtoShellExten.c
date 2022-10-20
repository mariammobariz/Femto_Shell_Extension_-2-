#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

int parser(char *buf, char *argv[])
{

    int flag = 0;

    for (int i = 0; i < strlen(buf); i++) {
	if (buf[i] == '=') {
	    flag = 1;
	    i = strlen(buf);
	}

    }


    if (flag == 1) {

	char *ar;
	ar = strtok(buf, "=");

	for (int i = 0; ar != NULL; i++) {
	    argv[i] = ar;
	    ar = strtok(NULL, "=");
	}
	return 1;

    } else {
	char *ar;
	ar = strtok(buf, " ");

	for (int i = 0; ar != NULL; i++) {
	    argv[i] = ar;
	    ar = strtok(NULL, " ");
	}

	return 2;
    }



}



int main()
{

    char *varName[30];
    char *varValue[30];
    static int var_index = 0;
    while (1) {
	char buf[100];

	printf("please enter your command > ");
	fgets(buf, 100, stdin);
	int len = strlen(buf);
	buf[len - 1] = 0;

	if (strlen(buf) == 0) {
	    continue;
	}

	int ret_pid = fork();

	if (ret_pid < 0) {
	    printf("fork failed");
	}

	else if (ret_pid > 0) {
	    int status;
	    wait(&status);
	}

	else if (ret_pid == 0) {

	    char *newargv[30];
	    int parReturn = parser(buf, newargv);

	    if (parReturn == 1) {
		varName[var_index] = malloc(strlen(newargv[0]));
		strcat(varName[var_index], newargv[0]);

		varValue[var_index] = malloc(strlen(newargv[1]));
		strcat(varValue[var_index], newargv[1]);
		var_index++;

	    } else {
		if (strcmp(newargv[0], "set") == 0) {
		    for (int j = 0; j < var_index; j++) {
			printf("local_var[%d] : %s = %s\n", j, varName[j],
			       varValue[j]);
		    }

		} else if (strcmp(newargv[0], "export") == 0) {

		    int i = 0;
		    int flag = 0;
		    for (i = 0; i < var_index; i++) {
			if (strcmp(newargv[1], varName[i]) == 0) {
			    flag = 1;
			    break;
			}
		    }
		    if (flag == 1) {
			setenv(varName[i], varValue[i], 1);
		    } else {
			printf("variable doesn not exist\n");
		    }
		    flag = 0;
		} else {
		    execvpe(newargv[0], newargv, environ);
		    printf("exec failed\n");
		    return -1;
		}
	    }
	}
    }
    return 0;

}

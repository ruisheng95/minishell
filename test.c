#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() 
{
    pid_t pid = fork();

    if (pid == -1)
	{
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
	else if (pid == 0) 
	{
        // Child process
        char *cmd = "/bin/cat";
        char *args[] = { "cat", "INVALID", NULL };
        char *envp[] = { NULL };

        if (execve(cmd, args, envp) == -1) {
            perror("execve failed");
            exit(EXIT_FAILURE);
        }
    } 
	else 
	{
        int status = 0;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) 
		{
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } 
		else 
		{
            printf("Child did not exit successfully\n");
        }
    }

    return 0;
}

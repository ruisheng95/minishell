/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 23:43:29 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/15 23:43:31 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// int main() 
// {
//     pid_t pid = fork();

//     if (pid == -1)
// 	{
//         perror("fork failed");
//         exit(EXIT_FAILURE);
//     }
// 	else if (pid == 0) 
// 	{
//         // Child process
//         char *cmd = "/bin/cat";
//         char *args[] = { "cat", "INVALID", NULL };
//         char *envp[] = { NULL };

//         if (execve(cmd, args, envp) == -1) {
//             perror("execve failed");
//             exit(EXIT_FAILURE);
//         }
//     } 
// 	else 
// 	{
//         int status = 0;
//         waitpid(pid, &status, 0);

//         if (WIFEXITED(status)) 
// 		{
//             printf("Child exited with status %d\n", WEXITSTATUS(status));
//         } 
// 		else 
// 		{
//             printf("Child did not exit successfully\n");
//         }
//     }

//     return 0;
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ethanlim <ethanlim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 23:43:29 by ethanlim          #+#    #+#             */
/*   Updated: 2024/09/19 02:01:06 by ethanlim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// #include <stdio.h>
// #include <dirent.h>

// int main() {
//     DIR *dir;
//     struct dirent *entry;

//     // Open the directory
//     dir = opendir(".");
//     if (dir == NULL) {
//         perror("opendir");
//         return 1;
//     }

//     // Read and print directory entries
//     while ((entry = readdir(dir)) != NULL) {
//         printf("%s\n", entry->d_name);
//     }

//     // Close the directory
//     closedir(dir);
//     return 0;
// }

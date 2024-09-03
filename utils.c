#include "minishell.h"
#include <termios.h>

void	free_2d_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	print_2d_array(char **cmd)
{
	int i = 0;
	while(cmd[i])
	{
		printf("token %d = %s\n", i, cmd[i]);
		i++;
	}
}

int	exit_process()
{
	write(1,"exit\n", 5);
	exit(0);
	return 0;
}

int	set_terminos_echo(int enable)
{
	struct termios	terminos_p;

	if (tcgetattr(STDOUT_FILENO, &terminos_p) == -1)
		return (-1);
	if (enable)
		terminos_p.c_lflag |= ECHOCTL;
	else
	 	terminos_p.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &terminos_p) == -1)
		return (-1);
	return (0);
}
#include "minishell.h"

int	env(t_data *data)
{
	int i;

	i = 0;
	while(data->envp[i])
	{
		ft_putendl_fd(data->envp[i], 1);
		i++;
	}
	return 0;
}
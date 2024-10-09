/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:32:54 by edforte           #+#    #+#             */
/*   Updated: 2024/10/08 19:12:07 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	parsing(t_data *table, char **av)
{
	alloc_var(table);
	set_table(table, av);
	set_philos(table);
	set_forks(table);
}

long	ft_atoi(char *str)
{
	long	result;
	long	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	while (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
}

int	all_numbers(char **av)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (av[j])
	{
		while (av[j][i])
		{
			if (av[j][i] < '0' || av[j][i] > '9')
				return (1);
			i ++;
		}

		j ++;
	}
	return (0);
}

int	checker(t_data *table, int ac, char **av)
{
	if (ac != 5 || ac != 6)
		return (1);
	if (all_numbers(av) != 0)
		return (1);
	if (ft_atoi(av[1]) <= 1 || ft_atoi(av[1]) >= 200)
		return (1);
	if (ft_atoi(av[2]) > 2147483647 || fft_atoi(av[2]) <= 0 ||\
		ft_atoi(av[3]) > 2147483647 || ft_atoi(av[3]) <= 0 ||\
		ft_atoi(av[4]) > 2147483647 || ft_atoi(av[4]) <= 0)
		return (1);
	parsing(table, ac);
	return (0);
}

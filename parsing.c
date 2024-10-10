/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:32:46 by edforte           #+#    #+#             */
/*   Updated: 2024/10/08 19:16:41 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	set_forks(t_data *table)
{
	int	i;

	i = 0;
	while (i < table->philo_num)
	{
		pthread_mutex_init(&table->forks[i], NULL);
	}
	i = 0;
	table->philos[i].l_fork = &table->forks[i];
	table->philos[i].r_fork = &table->forks[table->philo_num - 1];
	i ++;
	while (i < table->philo_num)
	{
		table->philos[i].l_fork = &table->forks[i];
		table->philos[i].l_fork = &table->forks[i - 1];
		i ++;
	}
}

void	set_philos(t_data *table)
{
	int	i;

	i = 0;
	while (i < table->philo_num)
	{
		table->philos[i].table = table;
		table->philos[i].id = i;
		table->philos[i].eat_cont = 0;
		table->philos[i].status = 0;
		table->philos[i].eating = 0;
		table->philos[i].time_to_die = table->death_time;
		pthread_mutex_init(&table->philos[i].lock, NULL);
		i ++;
	}
}

void	set_table(t_data *table, char **av)
{
	table->philo_num = (int)ft_atoi(av[1]);
	table->death_time = (u_int64_t)ft_atoi(av[2]);
	table->eat_time = (u_int64_t)ft_atoi(av[3]);
	table->sleep_time = (u_int64_t)ft_atoi(av[4]);
	table->meals_nb = -1;
	if (av[5])
		table->meals_nb = (int)ft_atoi(av[5]);
	table->dead = 0;
	table->finished = 0;
	table->start_time = get_time();
	pthread_mutex_init(&table->write, NULL);
	pthread_mutex_init(&table->lock, NULL);
}

void	alloc_var(t_data *table)
{
	table->tid = malloc(sizeof(pthread_t) * table->philo_num);
	if (!table->tid)
		return (1);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->philo_num);
	if (!table->forks)
		return (1);
	table->philos = malloc(sizeof(t_philo) * table->philo_num);
	if (!table->philos)
		return (1);
	return (0);
}

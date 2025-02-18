/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:33:12 by edforte           #+#    #+#             */
/*   Updated: 2024/10/10 11:11:04 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *data_pointer)
{
	t_philo *philo;
	philo = data_pointer;
	
	while (philo->table->dead == 0 || philo->table->finished == 0)
	{
		routine_actions(philo);
	}
	if (philo->table->dead == 0 && philo->table->finished != 0)
		return ;
	else
		print_death(philo);
}


int	main(int ac, char **av)
{
	t_data	table;

	pthread_mutex_t	mutex;
	pthread_t	tid;
	void *data_pointer;

	if (checker(&table, ac, av) != 0)
		return (1);
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&tid, NULL, &routine, &data_pointer);	
}

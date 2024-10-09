/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:11:39 by edforte           #+#    #+#             */
/*   Updated: 2024/10/08 19:49:17 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/types.h>

typedef struct s_philo
{
	struct s_data	*table;
	pthread_t		t1;
	int				id;
	int				eat_cont;
	int				status;
	int				eating;
	u_int64_t		time_to_die;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
} t_philo;

typedef struct s_data
{
	t_philo			*philos;
	pthread_t		*tid;
	int				philo_num;
	int				meals_nb;
	int				dead;
	int				finished;
	u_int64_t		death_time;
	u_int64_t		eat_time;
	u_int64_t		sleep_time;
	u_int64_t		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
} t_data;

int			checker(t_data *table, int ac, char **av);
int			all_numbers(char **av);
long		ft_atoi(char *str);
void		parsing(t_data *table, char **av);
void		alloc_var(t_data *table);
void		set_table(t_data *table, char **av);
void		set_philos(t_data *table);
void		set_forks(t_data *table);
u_int64_t	get_time(void)

#endif
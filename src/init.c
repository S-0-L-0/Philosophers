#include "philo.h"

int	init_simulation(t_simulation *sim, int argc, char **argv)
{
	/* Parse and validate arguments */
	if (parse_arguments(argc, argv, sim) != 0)
		return (1);
	
	/* Initialize simulation state */
	sim->simulation_ended = 0;
	sim->death_occurred = 0;
	sim->start_time = 0;
	
	/* Initialize mutex and philosopher arrays */
	if (init_mutexes(sim) != 0)
		return (1);
	
	if (init_philosophers(sim) != 0)
		return (1);
	
	return (0);
}

int	init_mutexes(t_simulation *sim)
{
	int	i;

	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philos);
	if (!sim->forks)
	{
		return (print_error("Failed to allocate fork mutexes"), 1);
	}
	i = 0;
	while (i < sim->num_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
		{
			return (print_error("Failed to initialize fork mutex"), 1);
		}
		i++;
	}
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		return (print_error("Failed to initialize print mutex"), 1);
	}
	if (pthread_mutex_init(&sim->end_mutex, NULL) != 0)
	{
		return (print_error("Failed to initialize end mutex"), 1);
	}
	return (0);
}

static void	fill_table_struct(t_simulation *sim, int i)
{
	sim->philosophers[i].id = i + 1;
	sim->philosophers[i].meals_eaten = 0;
	sim->philosophers[i].last_meal_time = 0;
	sim->philosophers[i].state = THINKING;
	sim->philosophers[i].sim = sim;
	sim->philosophers[i].left_fork_id = i;
	sim->philosophers[i].right_fork_id = (i + 1) % sim->num_philos;
	sim->philosophers[i].left_fork = &sim->forks[i];
	sim->philosophers[i].right_fork = &sim->forks[(i + 1) % sim->num_philos];
}
int	init_philosophers(t_simulation *sim)
{
	int	i;

	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philos);
	if (!sim->philosophers)
	{
		return (print_error("Failed to allocate philosophers"), 1);
	}
	i = 0;
	while (i < sim->num_philos)
	{
		fill_table_struct(sim, i);
		if (pthread_mutex_init(&sim->philosophers[i].death_mutex, NULL) != 0)
		{
			while (--i >= 0)
			{
				pthread_mutex_destroy(&sim->philosophers[i].death_mutex);
			}
			return (print_error("Failed to initialize death mutex"), 1);
		}
		i++;
	}
	return (0);
}

void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	if (sim->philosophers)
	{
		i = 0;
		while (i < sim->num_philos)
		{
			pthread_mutex_destroy(&sim->philosophers[i].death_mutex);
			i++;
		}
		free(sim->philosophers);
		sim->philosophers = NULL;
	}
	if (sim->forks)
	{
		i = 0;
		while (i < sim->num_philos)
		{
			pthread_mutex_destroy(&sim->forks[i]);
			i++;
		}
		free(sim->forks);
		sim->forks = NULL;
	}
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->end_mutex);
}

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

	/* Allocate fork mutexes */
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->forks)
	{
		print_error("Failed to allocate fork mutexes");
		return (1);
	}
	
	/* Initialize fork mutexes */
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
		{
			print_error("Failed to initialize fork mutex");
			return (1);
		}
		i++;
	}
	
	/* Initialize global mutexes */
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		print_error("Failed to initialize print mutex");
		return (1);
	}
	
	if (pthread_mutex_init(&sim->end_mutex, NULL) != 0)
	{
		print_error("Failed to initialize end mutex");
		return (1);
	}
	
	return (0);
}

int	init_philosophers(t_simulation *sim)
{
	int	i;

	/* Allocate philosopher array */
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	if (!sim->philosophers)
	{
		print_error("Failed to allocate philosophers");
		return (1);
	}
	
	/* Initialize each philosopher */
	i = 0;
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].meals_eaten = 0;
		sim->philosophers[i].last_meal_time = 0;
		sim->philosophers[i].state = THINKING;
		sim->philosophers[i].sim = sim;
		
		/* ✅ FIX: Store fork INDICES instead of pointers for ordering */
		sim->philosophers[i].left_fork_id = i;
		sim->philosophers[i].right_fork_id = (i + 1) % sim->num_philosophers;
		
		/* Assign actual fork pointers */
		sim->philosophers[i].left_fork = &sim->forks[i];
		sim->philosophers[i].right_fork = &sim->forks[(i + 1) % sim->num_philosophers];
		
		/* Initialize philosopher-specific mutexes */
		if (pthread_mutex_init(&sim->philosophers[i].death_mutex, NULL) != 0)
		{
			print_error("Failed to initialize death mutex");
			/* ✅ FIX: Cleanup already initialized mutexes */
			while (--i >= 0)
			{
				pthread_mutex_destroy(&sim->philosophers[i].death_mutex);
				pthread_mutex_destroy(&sim->philosophers[i].meal_mutex);
			}
			return (1);
		}
		
		if (pthread_mutex_init(&sim->philosophers[i].meal_mutex, NULL) != 0)
		{
			print_error("Failed to initialize meal mutex");
			pthread_mutex_destroy(&sim->philosophers[i].death_mutex);
			/* Cleanup already initialized mutexes */
			while (--i >= 0)
			{
				pthread_mutex_destroy(&sim->philosophers[i].death_mutex);
				pthread_mutex_destroy(&sim->philosophers[i].meal_mutex);
			}
			return (1);
		}
		
		i++;
	}
	
	return (0);
}

void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	/* ✅ FIX: Safe cleanup with null checks */
	if (sim->philosophers)
	{
		i = 0;
		while (i < sim->num_philosophers)
		{
			pthread_mutex_destroy(&sim->philosophers[i].death_mutex);
			pthread_mutex_destroy(&sim->philosophers[i].meal_mutex);
			i++;
		}
		free(sim->philosophers);
		sim->philosophers = NULL;
	}
	
	if (sim->forks)
	{
		i = 0;
		while (i < sim->num_philosophers)
		{
			pthread_mutex_destroy(&sim->forks[i]);
			i++;
		}
		free(sim->forks);
		sim->forks = NULL;
	}
	
	/* Only destroy if they were initialized */
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->end_mutex);
}

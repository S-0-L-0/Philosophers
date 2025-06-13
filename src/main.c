#include "philo.h"

static int	start_simulation(t_simulation *sim)
{
	int	i;
	int	created_threads;

	sim->start_time = get_timestamp();
	created_threads = 0;
	i = 0;
	while (i < sim->num_philos)
	{
		sim->philosophers[i].last_meal_time = sim->start_time;
		i++;
	}
	
	/* Create philosopher threads */
	i = 0;
	while (i < sim->num_philos)
	{
		if (pthread_create(&sim->philosophers[i].thread, NULL, 
			philosopher_routine, &sim->philosophers[i]) != 0)
		{
			print_error("Failed to create philosopher thread");
			sim->simulation_ended = 1;
			/* Wait for already created threads */
			while (--created_threads >= 0)
				pthread_join(sim->philosophers[created_threads].thread, NULL);
			return (1);
		}
		usleep(1000);
		created_threads++;
		i++;
	}
	
	/* Create monitor thread */
	if (pthread_create(&sim->monitor_thread, NULL, 
		monitor_routine, sim) != 0)
	{
		print_error("Failed to create monitor thread");
		sim->simulation_ended = 1;
		/* Wait for all philosopher threads */
		i = 0;
		while (i < sim->num_philos)
		{
			pthread_join(sim->philosophers[i].thread, NULL);
			i++;
		}
		return (1);
	}
	
	return (0);
}

static void	join_threads(t_simulation *sim)
{
	int	i;

	/* Wait for monitor thread to finish */
	pthread_join(sim->monitor_thread, NULL);
	
	/* Wait for all philosopher threads to finish */
	i = 0;
	while (i < sim->num_philos)
	{
		pthread_join(sim->philosophers[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_simulation	sim;

	/* Initialize structure to zero */
	memset(&sim, 0, sizeof(t_simulation));
	
	/* Validate arguments and initialize simulation */
	if (init_simulation(&sim, argc, argv) != 0)
	{
		cleanup_simulation(&sim);  /* ✅ FIX: Always cleanup */
		return (1);
	}
	
	/* Handle special case: single philosopher */
	if (sim.num_philos == 1)
	{
		printf("0 1 %s\n", MSG_FORK);
		precise_usleep(sim.time_to_die * 1000);
		printf("%d 1 %s\n", sim.time_to_die, MSG_DIED);
		cleanup_simulation(&sim);
		return (0);
	}
	
	/* Start the simulation */
	if (start_simulation(&sim) != 0)
	{
		cleanup_simulation(&sim);
		return (1);
	}
	
	/* Wait for simulation to complete */
	join_threads(&sim);
	
	/* Cleanup and exit */
	cleanup_simulation(&sim);
	return (0);
}

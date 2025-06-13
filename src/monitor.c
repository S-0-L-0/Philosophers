#include "philo.h"

void	end_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->end_mutex);
	sim->simulation_ended = 1;
	pthread_mutex_unlock(&sim->end_mutex);
}

static void	print_death(t_philosopher *philo, long death_time)
{
	/* ✅ FIX: Stampa atomica della morte con stesso lock order di print_status */
	pthread_mutex_lock(&philo->sim->print_mutex);
	
	/* Check e set simulation_ended atomicamente durante la stampa */
	pthread_mutex_lock(&philo->sim->end_mutex);
	if (philo->sim->simulation_ended)
	{
		pthread_mutex_unlock(&philo->sim->end_mutex);
		pthread_mutex_unlock(&philo->sim->print_mutex);
		return;
	}
	
	/* Stampa la morte e termina simulazione atomicamente */
	printf("%ld %d %s\n", death_time, philo->id, MSG_DIED);
	philo->sim->death_occurred = 1;
	philo->sim->simulation_ended = 1;
	pthread_mutex_unlock(&philo->sim->end_mutex);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

int	check_philosopher_death(t_simulation *sim)
{
	int		i;
	long	current_time;
	long	time_since_meal;

	current_time = get_timestamp();
	i = 0;
	
	while (i < sim->num_philosophers)
	{
		pthread_mutex_lock(&sim->philosophers[i].death_mutex);
		time_since_meal = current_time - sim->philosophers[i].last_meal_time;
		
		if (time_since_meal > sim->time_to_die)
		{
			pthread_mutex_unlock(&sim->philosophers[i].death_mutex);
			
			/* Calculate precise death time */
			long death_time = get_time_diff(sim->start_time, 
				sim->philosophers[i].last_meal_time + sim->time_to_die);
			
			print_death(&sim->philosophers[i], death_time);
			return (1);
		}
		
		pthread_mutex_unlock(&sim->philosophers[i].death_mutex);
		i++;
	}
	
	return (0);
}

int	check_all_fed(t_simulation *sim)
{
	int	i;
	int	fed_count;

	if (!sim->has_target_meals)
		return (0);
	
	fed_count = 0;
	i = 0;
	
	while (i < sim->num_philosophers)
	{
		pthread_mutex_lock(&sim->philosophers[i].meal_mutex);
		if (sim->philosophers[i].meals_eaten >= sim->target_meals)
			fed_count++;
		pthread_mutex_unlock(&sim->philosophers[i].meal_mutex);
		i++;
	}
	
	if (fed_count == sim->num_philosophers)
	{
		end_simulation(sim);
		return (1);
	}
	
	return (0);
}

static int	simulation_ended(t_simulation *sim)
{
	int	ended;

	pthread_mutex_lock(&sim->end_mutex);
	ended = sim->simulation_ended;
	pthread_mutex_unlock(&sim->end_mutex);
	
	return (ended);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	
	/* Monitor loop - check every 1ms for responsiveness */
	while (!simulation_ended(sim))
	{
		/* Check for deaths first (higher priority) */
		if (check_philosopher_death(sim))
			break;
		
		/* Check if all philosophers have eaten enough */
		if (check_all_fed(sim))
			break;
		
		/* Small sleep to prevent excessive CPU usage */
		usleep(1000);
	}
	
	return (NULL);
}

#include "philo.h"

static int	simulation_should_end(t_philosopher *philo)
{
	int	should_end;

	pthread_mutex_lock(&philo->sim->end_mutex);
	should_end = philo->sim->simulation_ended;
	pthread_mutex_unlock(&philo->sim->end_mutex);
	
	return (should_end);
}

int	take_forks(t_philosopher *philo)
{
	/* ✅ FIX: Anti-deadlock strategy using fork INDICES, not pointers */
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(philo->left_fork);
		if (simulation_should_end(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (1);
		}
		print_status(philo, MSG_FORK);
		
		pthread_mutex_lock(philo->right_fork);
		if (simulation_should_end(philo))
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			return (1);
		}
		print_status(philo, MSG_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		if (simulation_should_end(philo))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
		print_status(philo, MSG_FORK);
		
		pthread_mutex_lock(philo->left_fork);
		if (simulation_should_end(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
		print_status(philo, MSG_FORK);
	}
	
	return (0);
}

void eat(t_philosopher *philo)
{
    print_status(philo, MSG_EAT);
    
    /* Eat for the specified time */
    precise_usleep(philo->sim->time_to_eat * 1000);
    
    /* Update last meal time and count AFTER eating completes */
    pthread_mutex_lock(&philo->death_mutex);
    philo->last_meal_time = get_timestamp();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->death_mutex);
}

void	release_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philosopher *philo)
{
	print_status(philo, MSG_SLEEP);
	precise_usleep(philo->sim->time_to_sleep * 1000);
}

void	think(t_philosopher *philo)
{
	print_status(philo, MSG_THINK);
	
	/* Small delay to prevent busy waiting and improve fairness */
	/* For odd number of philosophers, thinking time helps prevent starvation */
	if (philo->sim->num_philosophers % 2 == 1)
	{
		long	think_time;
		
		think_time = (philo->sim->time_to_eat - philo->sim->time_to_sleep) / 2;
		if (think_time > 0)
			precise_usleep(think_time * 1000);
	}
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    
    /* Staggered start for even distribution */
    if (philo->id % 2 == 0)
        precise_usleep(1000);
    
    /* ⭐ PRIMA MANGIATA PER RESETTARE DEATH TIMER */
    if (take_forks(philo) == 0) {
        eat(philo);
        release_forks(philo);
        
        if (!simulation_should_end(philo))
            philo_sleep(philo);
    }
    
    /* ⭐ POI CICLO NORMALE */
    while (!simulation_should_end(philo)) {
        think(philo);
        
        if (simulation_should_end(philo))
            break;
        
        if (take_forks(philo) == 0) {
            eat(philo);
            release_forks(philo);
            
            if (simulation_should_end(philo))
                break;
            
            philo_sleep(philo);
        }
        else {
            break;
        }
    }
    
    return (NULL);
}
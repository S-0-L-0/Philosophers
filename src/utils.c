#include "philo.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		print_error("gettimeofday failed");
		return (-1);
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_time_diff(long start, long current)
{
	return (current - start);
}

void	precise_usleep(long microseconds)
{
	long	start;
	long	elapsed;
	long	current;

	start = get_timestamp();
	if (start == -1)  /* ✅ FIX: Handle gettimeofday error */
		return;
	
	start *= 1000;  /* Convert to microseconds */
	
	while (1)
	{
		current = get_timestamp();
		if (current == -1)  /* ✅ FIX: Handle gettimeofday error */
			return;
		
		elapsed = (current * 1000) - start;
		if (elapsed >= microseconds)
			break;
		usleep(500);
	}
}

void	print_status(t_philosopher *philo, char *message)
{
	long	timestamp;

	/* ✅ FIX: Check atomico dentro print_mutex per prevenire race condition */
	pthread_mutex_lock(&philo->sim->print_mutex);
	
	/* Double-check simulation_ended dentro il mutex di stampa */
	pthread_mutex_lock(&philo->sim->end_mutex);
	if (philo->sim->simulation_ended)
	{
		pthread_mutex_unlock(&philo->sim->end_mutex);
		pthread_mutex_unlock(&philo->sim->print_mutex);
		return;
	}
	pthread_mutex_unlock(&philo->sim->end_mutex);
	
	/* Solo ora stampiamo, garantendo che nessuna morte può interrompere */
	timestamp = get_time_diff(philo->sim->start_time, get_timestamp());
	printf("%ld %d %s\n", timestamp, philo->id, message);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *str)
{
	long	result;		/* ✅ FIX: Use long to detect overflow */
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
		str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		/* ✅ FIX: Check for overflow */
		if (result > 2147483647)  /* INT_MAX */
			return (-1);
		i++;
	}
	
	return ((int)(result * sign));
}

static int	is_valid_number(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	
	i = 0;
	if (str[i] == '+')
		i++;
	
	if (!str[i])
		return (0);
	
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	
	return (1);
}

int	parse_arguments(int argc, char **argv, t_simulation *sim)
{
	int	i;  /* ✅ FIX: Declare outside for C89 compliance */

	if (argc < 5 || argc > 6)
	{
		print_error("Usage: ./philo num_philosophers time_to_die time_to_eat time_to_sleep [num_meals]");
		return (1);
	}
	
	/* Validate all arguments are numbers */
	i = 1;  /* ✅ FIX: C89 compliant loop */
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
		{
			print_error("All arguments must be positive integers");
			return (1);
		}
		i++;
	}
	
	/* Parse required arguments and check for overflow */
	sim->num_philosophers = ft_atoi(argv[1]);
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	
	/* ✅ FIX: Check for ft_atoi overflow */
	if (sim->num_philosophers == -1 || sim->time_to_die == -1 || 
		sim->time_to_eat == -1 || sim->time_to_sleep == -1)
	{
		print_error("Arguments too large (overflow)");
		return (1);
	}
	
	/* Parse optional argument */
	if (argc == 6)
	{
		sim->target_meals = ft_atoi(argv[5]);
		sim->has_target_meals = 1;
		if (sim->target_meals <= 0 || sim->target_meals == -1)
		{
			print_error("Number of meals must be positive");
			return (1);
		}
	}
	else
	{
		sim->target_meals = -1;
		sim->has_target_meals = 0;
	}
	return (0);
}

void	print_error(char *message)
{
	printf("Error: %s\n", message);
}

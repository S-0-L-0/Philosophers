#include "philo.h"

void	ft_eat(t_philo *philo)
{
	printf("%lu %i is eating\n", (get_time() - philo->table->start_time), philo->id);
}

void	ft_think(t_philo *philo)
{

}

void	ft_sleep(t_philo *philo)
{

}

void	routine_actions(t_philo *philo)
{
	if
		ft_eat(philo);
	else if
		ft_think(philo);
	else if
		ft_sleep(philo);
	else
	{
		philo->table->dead = 1;
		return ;
	}
}

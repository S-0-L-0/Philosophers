#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

/* States */
# define THINKING 0
# define HUNGRY 1
# define EATING 2
# define SLEEPING 3
# define DEAD 4

/* Messages */
# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "died"

/* Colors for debug (can be removed in final) */
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define RESET "\033[0m"

/* Forward declarations */
typedef struct s_simulation	t_simulation;
typedef struct s_philosopher	t_philosopher;

/* Philosopher structure */
typedef struct s_philosopher
{
	int					id;
	int					meals_eaten;
	long				last_meal_time;
	int					state;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	int					left_fork_id;		/* ✅ FIX: Store indices for ordering */
	int					right_fork_id;		/* ✅ FIX: Store indices for ordering */
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		meal_mutex;
	t_simulation		*sim;
}	t_philosopher;

/* Main simulation structure */
typedef struct s_simulation
{
	int					num_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					target_meals;
	int					has_target_meals;
	long				start_time;
	int					simulation_ended;
	int					death_occurred;
	pthread_t			monitor_thread;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		end_mutex;
	pthread_mutex_t		*forks;
	t_philosopher		*philosophers;
}	t_simulation;

/* Function prototypes */

/* main.c */
int		main(int argc, char **argv);

/* init.c */
int		init_simulation(t_simulation *sim, int argc, char **argv);
int		init_philosophers(t_simulation *sim);
int		init_mutexes(t_simulation *sim);
void	cleanup_simulation(t_simulation *sim);

/* philosophers.c */
void	*philosopher_routine(void *arg);
int		take_forks(t_philosopher *philo);
void	eat(t_philosopher *philo);
void	release_forks(t_philosopher *philo);
void	philo_sleep(t_philosopher *philo);
void	think(t_philosopher *philo);

/* monitor.c */
void	*monitor_routine(void *arg);
int		check_philosopher_death(t_simulation *sim);
int		check_all_fed(t_simulation *sim);
void	end_simulation(t_simulation *sim);

/* utils.c */
long	get_timestamp(void);
long	get_time_diff(long start, long current);
void	precise_usleep(long microseconds);
void	print_status(t_philosopher *philo, char *message);
int		parse_arguments(int argc, char **argv, t_simulation *sim);
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
void	print_error(char *message);

#endif

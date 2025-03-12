/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 14:37:25 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/11 16:33:45 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				meals_eaten;
	int				last_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				start_time;
	int				nb_philos;
	int				num_times_to_eat;
	int				dead;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
	int				*all_done;
}	t_philo;

typedef struct s_program
{
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	t_philo			*philos;
	int				all_done;
}	t_program;

void	*philo_purpose(void *arg);
int		ft_atoi(const char *str);
int		is_digit(const char *str);
int		get_timestamp(void);
void	print_status(t_philo *philo, char *status);
void	init_philo_values(t_philo *philo, char **av, int ac);
void	init_philosophers(t_program *program, int nb_philo, int ac, char **av);
int		take_forks(t_philo *philo);
void	death_reaper(t_philo *philos, int nb_philo);
void	cleanup(t_program *program, int nb_philo);
int		check_philo_death(t_philo *philos, int i);
int		check_all_meals(t_philo *philos, int nb_philo);
int		check_args(int ac, char **av);
void	create_threads(t_program *program, int nb_philo);
int		take_forks_even(t_philo *philo);
int		take_forks_odd(t_philo *philo);
void	handle_single_philo(t_philo *philo);
int		check_meals_helper(t_philo *philos, int nb_philo);
int		eat_action(t_philo *philo);
int		sleep_think(t_philo *philo);
void	precise_sleep(size_t ms);
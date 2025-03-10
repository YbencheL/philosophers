/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:41:50 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 23:51:52 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	join_threads(t_program *program, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
		pthread_join(program->philos[i++].thread, NULL);
	cleanup(program, nb_philo);
}

void	create_threads(t_program *program, int nb_philo)
{
	int			i;
	long long	start_time;

	program->all_done = 0;
	start_time = get_timestamp();
	i = 0;
	while (i < nb_philo)
	{
		program->philos[i].all_done = &program->all_done;
		program->philos[i].start_time = start_time;
		program->philos[i].last_meal = start_time;
		if (pthread_create(&program->philos[i].thread, NULL, philo_purpose,
				&program->philos[i]))
			return ;
		usleep(100);
		i++;
	}
	if (nb_philo > 1)
		death_reaper(program->philos, nb_philo);
	join_threads(program, nb_philo);
}

void	handle_single_philo(t_philo *philo)
{
	print_status(philo, "has taken a fork");
	usleep(philo->time_to_die * 1000);
	pthread_mutex_lock(philo->dead_lock);
	*philo->all_done = 1;
	philo->dead = 1;
	print_status(philo, "died");
	pthread_mutex_unlock(philo->dead_lock);
}


void	*philo_purpose(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->nb_philos == 1)
	{
		handle_single_philo(philo);
		return (NULL);
	}
	if (philo->id % 2)
		usleep(500);
	return (philo_routine(philo));
}

void	*philo_routine(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->dead_lock);
		if (*philo->all_done)
		{
			pthread_mutex_unlock(philo->dead_lock);
			return (NULL);
		}
		pthread_mutex_unlock(philo->dead_lock);
		if (!take_forks(philo) || !eat_action(philo) || !sleep_think(philo)
			|| check_all_meals(philo, philo->nb_philos))
			break ;
	}
	return (NULL);
}

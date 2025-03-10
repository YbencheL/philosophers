/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:41:11 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 21:27:48 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	cleanup(t_program *program, int nb_philo)
{
	int	i;

	i = -1;
	while (++i < nb_philo)
	{
		pthread_mutex_destroy(program->philos[i].left_fork);
		free(program->philos[i].left_fork);
	}
	pthread_mutex_destroy(&program->write_lock);
	pthread_mutex_destroy(&program->dead_lock);
	pthread_mutex_destroy(&program->meal_lock);
	free(program->philos);
}

int	take_forks(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
		return (0);
	if (philo->id % 2)
		usleep(100);
	if (philo->id % 2 == 0)
		return (take_forks_even(philo));
	else
		return (take_forks_odd(philo));
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(philo->write_lock);
	printf("%d %d %s\n", get_timestamp() - philo->start_time,
		philo->id + 1, status);
	pthread_mutex_unlock(philo->write_lock);
}

int	eat_sleep_think(t_philo *philo)
{
	if (!take_forks(philo))
		return (0);
	if (!eat_action(philo))
		return (0);
	if (!sleep_think(philo))
		return (0);
	return (1);
}

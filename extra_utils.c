/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:41:11 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/01 15:13:03 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	cleanup(t_program *program, int nb_philo)
{
	int	i;

	usleep(100);
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
	if (*philo->all_done)
		return (0);
	if (philo->id % 2)
		usleep(400);
	if (philo->id % 2 == 0)
		return (take_forks_even(philo));
	else
		return (take_forks_odd(philo));
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(philo->write_lock);
	printf("%zu %d %s\n", get_timestamp() - philo->start_time,
		philo->id + 1, status);
	pthread_mutex_unlock(philo->write_lock);
}

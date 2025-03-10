/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 20:08:07 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 21:27:48 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_all_done_flag(t_philo *philos)
{
	pthread_mutex_lock(philos[0].dead_lock);
	*philos[0].all_done = 1;
	pthread_mutex_unlock(philos[0].dead_lock);
}

int	check_done_flag(t_philo *philos)
{
	pthread_mutex_lock(philos[0].dead_lock);
	if (*philos[0].all_done)
	{
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	pthread_mutex_unlock(philos[0].dead_lock);
	return (0);
}

int	eat_action(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->dead_lock);
	pthread_mutex_lock(philo->meal_lock);
	done = *philo->all_done;
	if (done)
	{
		pthread_mutex_unlock(philo->meal_lock);
		pthread_mutex_unlock(philo->dead_lock);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	philo->last_meal = get_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	pthread_mutex_unlock(philo->dead_lock);
	print_status(philo, "is eating");
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

int	sleep_think(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
		return (0);
	print_status(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
		return (0);
	print_status(philo, "is thinking");
	return (1);
}

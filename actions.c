/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 20:08:07 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 22:27:54 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
		return (0);
	if (philo->id % 2 == 0)
		return (take_forks_even(philo));
	else
		return (take_forks_odd(philo));
}

int	eat_action(t_philo *philo)
{
	size_t	eating_start;
	int			done;

	eating_start = get_timestamp();
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = eating_start;
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	print_status(philo, "is eating");
	precise_sleep(philo->time_to_eat);
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
	precise_sleep(philo->time_to_sleep);
	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
		return (0);
	print_status(philo, "is thinking");
	usleep(200);
	return (1);
}

void	precise_sleep(size_t ms)
{
	size_t	start;
	size_t	elapsed;

	start = get_timestamp();
	while (1)
	{
		elapsed = get_timestamp() - start;
		if (elapsed >= ms)
			break ;
		if (ms - elapsed > 5)
			usleep(1000);
		else
			usleep(100);
	}
}

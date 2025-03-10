/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:23:37 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 23:30:30 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks_even(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
	{
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	print_status(philo, "has taken a fork");
	return (1);
}

int	take_forks_odd(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	if (done)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	print_status(philo, "has taken a fork");
	return (1);
}

void	death_reaper(t_philo *philos, int nb_philo)
{
	int	i;

	while (1)
	{
		if (check_all_meals(philos, nb_philo))
			return ;
		i = 0;
		while (i < nb_philo)
		{
			if (check_philo_death(philos, i))
				return ;
			i++;
			usleep(100);
		}
		usleep(1000);
	}
}

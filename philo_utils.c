/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:23:37 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/01 11:41:01 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks_even(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (*philo->all_done)
	{
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	if (*philo->all_done)
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
	pthread_mutex_lock(philo->left_fork);
	if (*philo->all_done)
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	if (*philo->all_done)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	print_status(philo, "has taken a fork");
	return (1);
}

int	eat_sleep_think(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	if (*philo->all_done)
	{
		pthread_mutex_unlock(philo->meal_lock);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	philo->last_meal = get_timestamp();
	philo->meals_eaten++;
	print_status(philo, "is eating");
	pthread_mutex_unlock(philo->meal_lock);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	if (*philo->all_done)
		return (0);
	print_status(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
	if (*philo->all_done)
		return (0);
	print_status(philo, "is thinking");
	usleep(100);
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
			usleep(100);
			i++;
		}
		usleep(100);
	}
}

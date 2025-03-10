/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:03:28 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 22:15:54 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_and_exit_if_done(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	return (done);
}

int	check_meals_helper(t_philo *philos, int nb_philo)
{
	int	i;
	int	all_ate;

	all_ate = 1;
	i = -1;
	while (++i < nb_philo)
	{
		if (philos[i].num_times_to_eat == 0
			|| philos[i].meals_eaten < philos[i].num_times_to_eat)
		{
			all_ate = 0;
			break ;
		}
	}
	return (all_ate);
}

int	check_all_meals(t_philo *philos, int nb_philo)
{
	int	all_ate;

	pthread_mutex_lock(philos[0].dead_lock);
	if (*philos[0].all_done)
	{
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	pthread_mutex_unlock(philos[0].dead_lock);
	pthread_mutex_lock(philos[0].meal_lock);
	all_ate = check_meals_helper(philos, nb_philo);
	pthread_mutex_unlock(philos[0].meal_lock);
	if (all_ate)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos[0].all_done = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
		return (write(2, "Error: Wrong number of arguments\n", 33), 1);
	i = 1;
	while (av[i])
	{
		if (!is_digit(av[i]))
			return (write(2, "Error: Invalid argument\n", 24), 1);
		i++;
	}
	if (ft_atoi(av[1]) <= 0)
		return (write(2, "Error: Invalid number of philosophers\n", 37), 1);
	if (ft_atoi(av[2]) < 0 || ft_atoi(av[3]) < 0 || ft_atoi(av[4]) < 0)
		return (write(2, "Error: Invalid time values\n", 27), 1);
	if (ac == 6 && ft_atoi(av[5]) < 0)
		return (write(2, "Error: Invalid number of meals\n", 30), 1);
	return (0);
}

int	check_philo_death(t_philo *philos, int i)
{
	int	current_time;
	int	last_meal;

	pthread_mutex_lock(philos[i].meal_lock);
	last_meal = philos[i].last_meal;
	pthread_mutex_unlock(philos[i].meal_lock);
	current_time = get_timestamp();
	if (current_time - last_meal >= philos[i].time_to_die)
	{
		pthread_mutex_lock(philos[i].dead_lock);
		if (!(*philos[i].all_done))
		{
			*philos[i].all_done = 1;
			philos[i].dead = 1;
			pthread_mutex_lock(philos[i].write_lock);
			printf("%d %d died\n", current_time - philos[i].start_time,
				philos[i].id + 1);
			pthread_mutex_unlock(philos[i].write_lock);
		}
		pthread_mutex_unlock(philos[i].dead_lock);
		return (1);
	}
	return (0);
}

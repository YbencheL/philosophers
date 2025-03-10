/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:03:28 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/05 21:29:32 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_philo_death(t_philo *philos, int i)
{
	pthread_mutex_lock(philos[i].meal_lock);
	pthread_mutex_lock(philos[i].dead_lock);
	if ((get_timestamp() - philos[i].last_meal) > philos[i].time_to_die
		&& !*philos[i].all_done)
	{
		*philos[i].all_done = 1;
		philos[i].dead = 1;
		print_status(&philos[i], "died");
		pthread_mutex_unlock(philos[i].dead_lock);
		pthread_mutex_unlock(philos[i].meal_lock);
		return (1);
	}
	pthread_mutex_unlock(philos[i].dead_lock);
	pthread_mutex_unlock(philos[i].meal_lock);
	return (0);
}

int	check_all_meals(t_philo *philos, int nb_philo)
{
	int	i;

	pthread_mutex_lock(philos[0].meal_lock);
	pthread_mutex_lock(philos[0].dead_lock);
	i = -1;
	while (++i < nb_philo)
	{
		if (philos[i].num_times_to_eat == 0
			|| philos[i].meals_eaten < philos[i].num_times_to_eat)
		{
			pthread_mutex_unlock(philos[0].dead_lock);
			pthread_mutex_unlock(philos[0].meal_lock);
			return (0);
		}
	}
	*philos[0].all_done = 1;
	pthread_mutex_unlock(philos[0].dead_lock);
	pthread_mutex_unlock(philos[0].meal_lock);
	return (1);
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

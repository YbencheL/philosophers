/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:41:50 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 21:50:12 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	create_threads(t_program *program, int nb_philo)
{
	int			i;

	program->all_done = 0;
	i = 0;
	while (i < nb_philo)
	{
		program->philos[i].all_done = &program->all_done;
		if (pthread_create(&program->philos[i].thread, NULL, philo_purpose,
				&program->philos[i]))
			return ;
		pthread_mutex_lock(&program->meal_lock);
		program->philos[i].last_meal = get_timestamp();
		pthread_mutex_unlock(&program->meal_lock);
		usleep(100);
		i++;
	}
	if (nb_philo > 1)
		death_reaper(program->philos, nb_philo);
	i = 0;
	while (i < nb_philo)
		pthread_join(program->philos[i++].thread, NULL);
	cleanup(program, nb_philo);
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

int	check_and_exit_if_done(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(philo->dead_lock);
	done = *philo->all_done;
	pthread_mutex_unlock(philo->dead_lock);
	return (done);
}

int	run_philo_routine(t_philo *philo)
{
	if (!eat_sleep_think(philo))
		return (0);
	if (check_all_meals(philo, philo->nb_philos))
		return (0);
	return (1);
}

void	*philo_purpose(void *arg)
{
	t_philo	*philo;
	int		done;

	philo = (t_philo *)arg;
	if (philo->nb_philos == 1)
	{
		handle_single_philo(philo);
		return (NULL);
	}
	if (philo->id % 2)
		usleep(300);
	while (1)
	{
		done = check_and_exit_if_done(philo);
		if (done)
			return (0);
		if (!run_philo_routine(philo))
			break ;
	}
	return (NULL);
}

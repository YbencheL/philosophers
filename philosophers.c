/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:41:50 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/02 17:11:56 by ybenchel         ###   ########.fr       */
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
		program->philos[i].last_meal = get_timestamp();
		usleep(500);
		i++;
	}
	if (nb_philo > 1)
		death_reaper(program->philos, nb_philo);
	i = 0;
	while (i < nb_philo)
		pthread_join(program->philos[i++].thread, NULL);
	cleanup(program, nb_philo);
}

static void	handle_single_philo(t_philo *philo)
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
		usleep(300);
	while (!(*philo->all_done))
	{
		if (!take_forks(philo))
			break ;
		if (!eat_sleep_think(philo))
			break ;
		if (check_all_meals(philo, philo->nb_philos))
			break ;
	}
	return (NULL);
}

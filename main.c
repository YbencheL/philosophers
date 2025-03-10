/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenchel <ybenchel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 14:37:55 by ybenchel          #+#    #+#             */
/*   Updated: 2025/03/10 23:25:21 by ybenchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_philo_values(t_philo *philo, char **av, int ac)
{
	philo->meals_eaten = 0;
	philo->time_to_die = ft_atoi(av[2]);
	philo->time_to_eat = ft_atoi(av[3]);
	philo->time_to_sleep = ft_atoi(av[4]);
	philo->nb_philos = ft_atoi(av[1]);
	philo->dead = 0;
	if (ac == 6)
		philo->num_times_to_eat = ft_atoi(av[5]);
	else
		philo->num_times_to_eat = 0;
}

void	init_utils(t_program *program, int i, int nb_philo)
{
	program->philos[i].nb_philos = nb_philo;
	program->philos[i].id = i;
	program->philos[i].write_lock = &program->write_lock;
	program->philos[i].dead_lock = &program->dead_lock;
	program->philos[i].meal_lock = &program->meal_lock;
	program->philos[i].start_time = get_timestamp();
	program->philos[i].last_meal = get_timestamp();
	program->philos[i].left_fork = malloc(sizeof(pthread_mutex_t));
	if (!program->philos[i].left_fork)
		return ;
}

void	init_philosophers(t_program *program, int nb_philo, int ac, char **av)
{
	int				i;

	program->philos = malloc(sizeof(t_philo) * nb_philo);
	if (!program->philos)
		return ;
	i = 0;
	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->dead_lock, NULL);
	pthread_mutex_init(&program->meal_lock, NULL);
	while (i < nb_philo)
	{
		init_utils(program, i, nb_philo);
		init_philo_values(&program->philos[i], av, ac);
		pthread_mutex_init(program->philos[i].left_fork, NULL);
		if (i > 0)
			program->philos[i].right_fork = program->philos[i - 1].left_fork;
		i++;
	}
	if (nb_philo > 1)
		program->philos[0].right_fork = program->philos[nb_philo - 1].left_fork;
}

int	main(int ac, char **av)
{
	int			nb_philo;
	t_program	program;

	if (check_args(ac, av))
		return (1);
	nb_philo = ft_atoi(av[1]);
	init_philosophers(&program, nb_philo, ac, av);
	create_threads(&program, nb_philo);
	return (0);
}

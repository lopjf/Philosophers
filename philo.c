/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:06:30 by loris             #+#    #+#             */
/*   Updated: 2023/02/09 14:16:24 by loris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialise_states(t_thread_data *dataptr)
{
	int	id;

	id = 0;
	while (id < dataptr->number_of_philosophers)
	{
		dataptr->info[id].philosopher_state = on;
		dataptr->info[id].fork = 0;
		dataptr->info[id].eat_counter = 0;
		gettimeofday(&dataptr->info[id].last_ate, NULL);
		id++;
	}
	dataptr->philosopher_id = 0;
}

int	launch_philosophers(t_thread_data *dataptr)
{
	int			i;
	pthread_t	*philosopher;

	i = 0;
	philosopher = malloc(sizeof(pthread_t) * dataptr->number_of_philosophers);
	if (!philosopher)
		return (0);
	initialise_states(dataptr);
	while (i < dataptr->number_of_philosophers)
	{
		if (pthread_create(&philosopher[i], NULL, &routine, (void *)dataptr))
			return (1);
		i++;
	}
	i = 0;
	while (i < dataptr->number_of_philosophers)
	{
		if (pthread_join(philosopher[i], NULL))
			return (2);
		i++;
	}
	free(philosopher);
	return (0);
}

int	check_if_valid_parameters(t_thread_data *dataptr, int ac)
{
	if (dataptr->number_of_philosophers < 1 || dataptr->time_to_die < 0)
		return (0);
	if (dataptr->time_to_eat < 0 || dataptr->time_to_sleep < 0)
		return (0);
	if (ac == 6 && dataptr->nb_of_times_each_philosopher_must_eat < 0)
		return (0);
	return (1);
}

int	main(int ac, char *av[])
{
	t_thread_data	*dataptr;

	if (ac < 5 || ac > 6)
		return (0);
	dataptr = (t_thread_data *)malloc(sizeof(t_thread_data));
	if (dataptr == NULL)
		return (0);
	gettimeofday(&dataptr->start, NULL);
	if (pthread_mutex_init(&dataptr->mutex, NULL))
		return (1);
	dataptr->philosopher_id = 0;
	dataptr->dead = 0;
	dataptr->number_of_philosophers = ft_atoi(av[1], 0, 0);
	dataptr->time_to_die = ft_atoi(av[2], 0, 0);
	dataptr->time_to_eat = ft_atoi(av[3], 0, 0);
	dataptr->time_to_sleep = ft_atoi(av[4], 0, 0);
	if (ac == 6)
		dataptr->nb_of_times_each_philosopher_must_eat = ft_atoi(av[5], 0, 0);
	else
		dataptr->nb_of_times_each_philosopher_must_eat = -1;
	if (check_if_valid_parameters(dataptr, ac) == 1)
		launch_philosophers(dataptr);
	pthread_mutex_destroy(&dataptr->mutex);
	free(dataptr);
	return (0);
}

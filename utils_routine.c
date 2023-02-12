/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 10:08:03 by loris             #+#    #+#             */
/*   Updated: 2023/02/09 14:11:35 by loris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	starve_alone(t_thread_data *dataptr, int id)
{
	dataptr->info[id].fork = 1;
	give_timestamp(dataptr, id, 0);
	usleep(dataptr->time_to_die * 1000);
	give_timestamp(dataptr, id, 4);
	return (0);
}

int	grab_fork_then_eat(t_thread_data *dataptr, int id, int id_up)
{
	if (id == id_up)
	{
		starve_alone(dataptr, id);
		return (0);
	}
	if (dataptr->dead == 1)
		return (0);
	pthread_mutex_lock(&dataptr->mutex);
	dataptr->info[id].fork = 1;
	give_timestamp(dataptr, id, 0);
	if (dataptr->dead == 1)
		return (0);
	dataptr->info[id_up].fork = 1;
	give_timestamp(dataptr, id, 0);
	pthread_mutex_unlock(&dataptr->mutex);
	gettimeofday(&dataptr->info[id].last_ate, NULL);
	if (dataptr->dead == 1)
		return (0);
	give_timestamp(dataptr, id, 1);
	usleep(dataptr->time_to_eat * 1000);
	dataptr->info[id].fork = 0;
	dataptr->info[id_up].fork = 0;
	dataptr->info[id].eat_counter++;
	return (1);
}

int	routine_helper(t_thread_data *dataptr, int id, int id_up)
{
	if (dataptr->info[id].fork == 0 && dataptr->info[id_up].fork == 0)
	{
		if (grab_fork_then_eat(dataptr, id, id_up) == 0)
			return (0);
		if (dataptr->dead == 1)
			return (0);
		give_timestamp(dataptr, id, 2);
		usleep(dataptr->time_to_sleep * 1000);
		if (dataptr->dead == 1)
			return (0);
		give_timestamp(dataptr, id, 3);
	}
	if (dataptr->info[id].eat_counter == \
	dataptr->nb_of_times_each_philosopher_must_eat)
		dataptr->info[id].philosopher_state = off;
	if (check_if_starving(dataptr, id) == 0)
	{
		dataptr->dead = 1;
		give_timestamp(dataptr, id, 4);
		return (0);
	}
	return (1);
}

void	*routine(void *ptr)
{
	int				id;
	int				id_up;
	int				flag;
	t_thread_data	*dataptr;

	dataptr = ptr;
	flag = 0;
	pthread_mutex_lock(&dataptr->mutex);
	id = dataptr->philosopher_id;
	dataptr->philosopher_id++;
	pthread_mutex_unlock(&dataptr->mutex);
	id_up = id + 1;
	if (id == dataptr->number_of_philosophers - 1)
		id_up = 0;
	while (dataptr->info[id].philosopher_state == on)
	{
		if (dataptr->info[id].eat_counter == 0 && id % 2 == 1 && flag == 0)
		{
			usleep(dataptr->time_to_eat * 1000);
			flag = 1;
		}
		if (routine_helper(dataptr, id, id_up) == 0)
			return (0);
	}
	return (0);
}

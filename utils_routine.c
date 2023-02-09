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

int	grab_fork_then_eat(t_thread_data *dataptr, int id, int id_up)
{
	if (check_if_dead(dataptr) == 0)
		return (0);
	pthread_mutex_lock(&dataptr->mutex);
	dataptr->info[id].fork = 1;
	give_timestamp(dataptr, id, 0);
	if (dataptr->info[id_up].fork == 1)
	{
		dataptr->info[id].philosopher_state = dead;
		give_timestamp(dataptr, id, 4);
		return (0);
	}
	dataptr->info[id_up].fork = 1;
	give_timestamp(dataptr, id, 0);
	pthread_mutex_unlock(&dataptr->mutex);
	gettimeofday(&dataptr->info[id].last_ate, NULL);
	if (check_if_dead(dataptr) == 0)
		return (0);
	give_timestamp(dataptr, id, 1);
	usleep(dataptr->time_to_eat * 1000);
	pthread_mutex_lock(&dataptr->mutex);
	dataptr->info[id].fork = 0;
	dataptr->info[id_up].fork = 0;
	pthread_mutex_unlock(&dataptr->mutex);
	dataptr->info[id].eat_counter++;
	return (1);
}

int	routine_helper(t_thread_data *dataptr, int id, int id_up)
{
	if (dataptr->info[id].fork == 0 && dataptr->info[id_up].\
		fork == 0 && ready_to_eat(dataptr, id) == 1)
	{
		if (grab_fork_then_eat(dataptr, id, id_up) == 0)
			return (0);
		if (check_if_dead(dataptr) == 0)
			return (0);
		give_timestamp(dataptr, id, 2);
		usleep(dataptr->time_to_sleep * 1000);
		if (check_if_dead(dataptr) == 0)
			return (0);
		give_timestamp(dataptr, id, 3);
	}
	if (dataptr->info[id].eat_counter == \
	dataptr->nb_of_times_each_philosopher_must_eat)
		dataptr->info[id].philosopher_state = off;
	if (check_if_starving(dataptr, id) == 0)
	{
		dataptr->info[id].philosopher_state = dead;
		give_timestamp(dataptr, id, 4);
	}
	return (1);
}

void	*routine(void *ptr)
{
	int				id;
	int				id_up;
	t_thread_data	*dataptr;

	dataptr = ptr;
	id = dataptr->philosopher_id;
	dataptr->philosopher_id++;
	id_up = id + 1;
	if (id == dataptr->number_of_philosophers - 1)
		id_up = 0;
	while (dataptr->info[id].philosopher_state == on)
	{
		if (routine_helper(dataptr, id, id_up) == 0)
			return (0);
	}
	return (0);
}

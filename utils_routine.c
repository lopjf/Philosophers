/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 10:08:03 by loris             #+#    #+#             */
/*   Updated: 2023/02/09 10:08:10 by loris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	give_timestamp(t_thread_data *dataptr, int id, int reason)
{
	struct timeval	gettime;
	long			current_timestamp;

	gettime = dataptr->info[id].gettime;
	gettimeofday(&gettime, NULL);
	current_timestamp = (gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - \
	(dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec / 1000);
	if (reason == 0)
		printf("%ld %i has taken a fork\n", current_timestamp, id);
	if (reason == 1)
		printf("%ld %i is eating\n", current_timestamp, id);
	if (reason == 2)
		printf("%ld %i is sleeping\n", current_timestamp, id);
	if (reason == 3)
		printf("%ld %i is thinking\n", current_timestamp, id);
	if (reason == 4)
		printf("%ld %i died\n", current_timestamp, id);
}

int	check_if_starving(t_thread_data *dataptr, int id)
{
	struct timeval	last_ate;
	struct timeval	gettime;

	last_ate = dataptr->info[id].last_ate;
	gettime = dataptr->info[id].gettime;
	gettimeofday(&gettime, NULL);
	if ((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - \
	(last_ate.tv_sec * 1000 + last_ate.tv_usec / 1000) >= dataptr->time_to_die)
		return (0);
	return (1);
}

int	check_if_dead(t_thread_data *dataptr)
{
	int	id;

	id = 0;
	while (id < dataptr->number_of_philosophers)
	{
		if (dataptr->info[id].philosopher_state == dead)
			return (0);
		id++;
	}
	return (1);
}

int	ready_to_eat(t_thread_data *dataptr, int id)
{
	struct timeval	last_ate;
	struct timeval	gettime;

	last_ate = dataptr->info[id].last_ate;
	gettime = dataptr->info[id].gettime;
	gettimeofday(&gettime, NULL);
	if ((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - (last_ate.tv_sec \
	* 1000 + last_ate.tv_usec / 1000) >= dataptr->time_to_die - 10)
		return (1);
	if (dataptr->info[id].eat_counter == 0)
		return (1);
	return (0);
}

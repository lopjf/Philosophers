/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 09:45:16 by loris             #+#    #+#             */
/*   Updated: 2023/02/09 14:11:04 by loris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_atoi(const char *nptr, int i, long long nbr)
{
	int			minus;

	minus = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			minus *= -1 ;
		i++;
	}
	while (nptr[i] != '\0' && nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr = (nbr * 10) + nptr[i] - 48;
		i++;
	}
	if (nptr[i] == '\0')
	{
		if (nbr > 2147483647 || nbr * minus < 0)
			return (-1);
		else
			return (nbr * minus);
	}
	return (-1);
}

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

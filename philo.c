/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:06:30 by loris             #+#    #+#             */
/*   Updated: 2023/02/07 10:07:54 by loris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Allowed external funcs:
// memset, printf, malloc, free, write,
// usleep, gettimeofday, pthread_create,
// pthread_detach, pthread_join, pthread_mutex_init,
// pthread_mutex_destroy, pthread_mutex_lock,
// pthread_mutex_unlock

// one of the biggest differences in between thread and processes:
// threads are sharing memory, proceses are not

#include "philo.h"

// I think the Makefile relink
void	give_timestamp(t_thread_data *dataptr, int id, int reason)
{
	struct timeval	gettime;

	gettime = dataptr->info[id].gettime;
	gettimeofday(&gettime, NULL);
	if (reason == 0)
	{
		printf("%ld %i has taken a fork\n", \
		((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - \
		(dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 1)
	{
		printf("%ld %i is eating\n", \
		((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - \
		(dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 2)
	{
		printf("%ld %i is sleeping\n", \
		((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - \
		(dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 3)
	{
		printf("%ld %i is thinking\n", \
		((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - \
		(dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 4)
	{
		printf("%ld %i died\n", \
		((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - \
		(dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec / 1000)), id);
		// not sure if we can use exit()
		exit(0);
	}
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
	if (check_if_dead(dataptr) == 0)
		return (0);
	return (1);
}

void	*routine(void *ptr)
{
	int			id;
	int			id_up;
	t_thread_data	*dataptr;

	dataptr = ptr;
	id = dataptr->philosopher_id;
	dataptr->philosopher_id++;
	if (id == dataptr->number_of_philosophers - 1)
		id_up = 0;
	else
		id_up = id + 1;
	while (dataptr->info[id].philosopher_state == on)
	{
		if (dataptr->info[id].fork == 0 && dataptr->info[id_up].\
		fork == 0 && ready_to_eat(dataptr, id) == 1)
		{
			if (grab_fork_then_eat(dataptr, id, id_up) == 0)
				return (0);
			give_timestamp(dataptr, id, 2);
			usleep(dataptr->time_to_sleep * 1000);
			if (check_if_dead(dataptr) == 0)
				break ;
			give_timestamp(dataptr, id, 3);
		}
		if (dataptr->info[id].eat_counter == \
		dataptr->number_of_times_each_philosopher_must_eat)
			dataptr->info[id].philosopher_state = off;
		if (check_if_starving(dataptr, id) == 0)
		{
			dataptr->info[id].philosopher_state = dead;
			give_timestamp(dataptr, id, 4);
		}
	}
	return (0);
}

long long	ft_atoi(const char *nptr)
{
	int			i;
	long long	nbr;
	int			minus;

	i = 0;
	nbr = 0;
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
	pthread_t	philosopher[dataptr->number_of_philosophers];

	i = 0;
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
	return (0);
}

int	check_if_valid_parameters(t_thread_data *dataptr, int ac)
{
	if (dataptr->number_of_philosophers < 1 || dataptr->time_to_die < 0)
		return (0);
	if (dataptr->time_to_eat < 0 || dataptr->time_to_sleep < 0)
		return (0);
	if (ac == 6 && dataptr->number_of_times_each_philosopher_must_eat < 0)
		return (0);
	return (1);
}

int	main(int ac, char *av[])
{
	t_thread_data	*dataptr;

	dataptr = (t_thread_data *)malloc(sizeof(t_thread_data));
	if (dataptr == NULL)
		return (0);
	gettimeofday(&dataptr->start, NULL);
	if (pthread_mutex_init(&dataptr->mutex, NULL))
		return (1);
	if (ac < 5 || ac > 6)
	{
		pthread_mutex_destroy(&dataptr->mutex);
		free(dataptr);
		return (0);
	}
	dataptr->number_of_philosophers = ft_atoi(av[1]);
	dataptr->time_to_die = ft_atoi(av[2]);
	dataptr->time_to_eat = ft_atoi(av[3]);
	dataptr->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		dataptr->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else
		dataptr->number_of_times_each_philosopher_must_eat = -1;
	if (check_if_valid_parameters(dataptr, ac) == 1)
		launch_philosophers(dataptr);
	pthread_mutex_destroy(&dataptr->mutex);
	free(dataptr);
	return (0);
}

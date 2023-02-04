/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:06:30 by loris             #+#    #+#             */
/*   Updated: 2023/02/04 19:55:26 by loris            ###   ########.fr       */
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
void give_timestamp(thread_data *dataptr, int id, int reason)
{
	struct timeval gettime;
	
	gettime = dataptr->info[id].gettime;
	gettimeofday(&gettime, NULL);
	if (reason == 0)
	{
		printf("%ld %i has taken a fork\n", ((gettime.tv_sec * 1000 +
		gettime.tv_usec / 1000) - (dataptr->start.tv_sec * 1000 +
		dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 1)
	{
		printf("%ld %i is eating\n", ((gettime.tv_sec * 1000 +
		gettime.tv_usec / 1000) - (dataptr->start.tv_sec * 1000 +
		dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 2)
	{
		printf("%ld %i is sleeping\n", ((gettime.tv_sec * 1000 +
		gettime.tv_usec / 1000) - (dataptr->start.tv_sec * 1000 +
		dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 3)
	{
		printf("%ld %i is thinking\n", ((gettime.tv_sec * 1000 +
		gettime.tv_usec / 1000) - (dataptr->start.tv_sec * 1000 +
		dataptr->start.tv_usec / 1000)), id);
	}
	if (reason == 4)
	{
		printf("%ld %i died\n", ((gettime.tv_sec * 1000 +
		gettime.tv_usec / 1000) - (dataptr->start.tv_sec * 1000 +
		dataptr->start.tv_usec / 1000)), id);
	}
}

int check_last_ate(thread_data *dataptr, int id)
{
	struct timeval last_ate;
	struct timeval gettime;
	
	last_ate = dataptr->info[id].last_ate;
	gettime = dataptr->info[id].gettime;
	gettimeofday(&gettime, NULL);
	if ((gettime.tv_sec * 1000 + gettime.tv_usec / 1000) - (last_ate.tv_sec * 1000 + last_ate.tv_usec / 1000) >= dataptr->time_to_die)
		return (0);
	return (1);
}

int check_if_dead(thread_data *dataptr)
{
	int id;

	id = 0;
	while (id < dataptr->number_of_philosophers)
	{
		if (dataptr->info[id].philosopher_state == dead)
			return (0);
		id++;
	}
	return (1);
}

void *routine(void *ptr)
{
		// To prevent philosophers from duplicating forks, you should protect the forks state with a mutex for each of them
	int id;
	int id_up;

	thread_data *dataptr = ptr;
	// maybe put a mutex here to avoid race for giving ids
	id = dataptr->philosopher_id;
	dataptr->philosopher_id++;
	if (id == dataptr->number_of_philosophers)
		id_up = 0;
	else
		id_up = id + 1;
	while (dataptr->info[id].philosopher_state == on)
	{
		if (dataptr->info[id].fork == 0 && dataptr->info[id_up].fork == 0)
		{
			// if (check_if_dead(dataptr) == 0)
			// 	break;
			pthread_mutex_lock(&dataptr->mutex);
			dataptr->info[id].fork = 1;
			give_timestamp(dataptr, id, 0);
			dataptr->info[id_up].fork = 1;
			give_timestamp(dataptr, id, 0);
			pthread_mutex_unlock(&dataptr->mutex);
			// if (check_if_dead(dataptr) == 0)
			// 	break;
			give_timestamp(dataptr, id, 1);
			usleep(dataptr->time_to_eat * 1000);
			pthread_mutex_lock(&dataptr->mutex);
			dataptr->info[id].fork = 0;
			dataptr->info[id_up].fork = 0;
			pthread_mutex_unlock(&dataptr->mutex);
			gettimeofday(&dataptr->info[id].last_ate, NULL);
			dataptr->info[id].eat_counter++;
			// if (check_if_dead(dataptr) == 0)
			// 	break;
			give_timestamp(dataptr, id, 2);
			usleep(dataptr->time_to_sleep * 1000);
			// if (check_if_dead(dataptr) == 0)
			// 	break;
			give_timestamp(dataptr, id, 3);
		}
		if (dataptr->info[id].eat_counter == dataptr->number_of_times_each_philosopher_must_eat)
			dataptr->info[id].philosopher_state = off;
		else if (check_last_ate(dataptr, id) == 0)
		{
			dataptr->info[id].philosopher_state = dead;
			give_timestamp(dataptr, id, 4);
		}
	}
	return (0);
}
// Any of the threads in the process calls exit(3), or the  main  thread
// performs  a  return  from main().  This causes the termination of all
// threads in the process.

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
	return (nbr * minus);
}

void initialise_states(thread_data *dataptr)
{
	int i;

	i = 0;
	while (i < dataptr->number_of_philosophers)
	{
		dataptr->info[i].philosopher_state = on;
		dataptr->info[i].fork = 0;
		dataptr->info[i].eat_counter = 0;
		i++;
	}
	dataptr->philosopher_id = 0;
}

int launch_philosophers(thread_data *dataptr)
{
	
	int i;

	i = 0;
	initialise_states(dataptr);
	pthread_t philosopher[dataptr->number_of_philosophers];
	while (i < dataptr->number_of_philosophers)
	{
		if (pthread_create(&philosopher[i], NULL, &routine, (void *)dataptr))	// if(0), le if s'arrete
			return (1);
		i++;
	}
	i = 0;
	while (i < dataptr->number_of_philosophers)
	{
		// permet d'attendre la fin de l'execution du thread avant d'aller plus loin: return(0);
		if (pthread_join(philosopher[i], NULL))
			return (2);
		i++;
	}
	return (0);
}

int	main(int ac, char *av[])
{
	// need to malloc the struct
	thread_data *dataptr;
	dataptr = (thread_data *)malloc(sizeof(thread_data));
	if (dataptr == NULL)
		return (0);
	// start timer
	gettimeofday(&dataptr->start, NULL);
	// init mutex	
	if (pthread_mutex_init(&dataptr->mutex, NULL))
		return (1);
	// grab the inputs
	if (ac < 5 || ac > 6)
		return (0);
	dataptr->number_of_philosophers = ft_atoi(av[1]);
	dataptr->time_to_die = ft_atoi(av[2]);
	dataptr->time_to_eat = ft_atoi(av[3]);
	dataptr->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		dataptr->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else
		dataptr->number_of_times_each_philosopher_must_eat = -1;

	launch_philosophers(dataptr);

	// test gettime
	// gettimeofday(&gettime, NULL);
	// printf("%ld milliseconds\n", ((gettime.tv_sec * 1000 + gettime.tv_usec) - (dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec)));
	// sleep(1);
	// gettimeofday(&gettime, NULL);
	// printf("%ld milliseconds\n", ((gettime.tv_sec * 1000 + gettime.tv_usec) - (dataptr->start.tv_sec * 1000 + dataptr->start.tv_usec)));
	for (int i = 0; i < dataptr->number_of_philosophers; i++)
		printf("%i has eaten %i times\n", i, dataptr->info[i].eat_counter);

	pthread_mutex_destroy(&dataptr->mutex);
	free(dataptr);
	return (0);
}

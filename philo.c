/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:06:30 by loris             #+#    #+#             */
/*   Updated: 2023/01/19 21:14:52 by loris            ###   ########.fr       */
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

void *routine(void *ptr)
{
	thread_data *dataptr = ptr;
	// get activated when calling pthread_join
	pthread_mutex_lock(&dataptr->mutex);
	printf("okkkk%i\n", dataptr->number_of_forks);
	pthread_mutex_unlock(&dataptr->mutex);
	return 0;
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

int	main(int ac, char *av[])
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	pthread_t p1;
	thread_data *dataptr;

	if (ac < 5 || ac > 6)
	return (0);
	number_of_philosophers = ft_atoi(av[1]);
	time_to_die = ft_atoi(av[2]);
	time_to_eat = ft_atoi(av[3]);
	time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else
		number_of_times_each_philosopher_must_eat = 0;
		
	// need to malloc the struct
	dataptr = (thread_data *)malloc(sizeof(thread_data));
	if (dataptr == NULL)
		return (0);
	dataptr->number_of_forks = number_of_philosophers;
	// dataptr->mutex = malloc(sizeof(pthread_mutex_t));
	
	if (pthread_mutex_init(&dataptr->mutex, NULL))
		return (1);

	printf("%i\n", number_of_philosophers);
	printf("%i\n", time_to_die);
	printf("%i\n", time_to_eat);
	printf("%i\n", time_to_sleep);
	printf("%i\n", number_of_times_each_philosopher_must_eat);
	// on peut faire communiquer les threads en leur donnant en argument la meme variable
	if (pthread_create(&p1, NULL, &routine, (void *)dataptr))	// if(0), le if s'arrete
		return (1);
	usleep(1);
	pthread_mutex_destroy(&dataptr->mutex);
		// permet d'attendre la fin de l'execution du thread avant d'aller plus loin: return(0);
	if (pthread_join(p1, NULL))
		return (2);
	free(dataptr);
	return (0);
}

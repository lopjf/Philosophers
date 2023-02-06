/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:06:33 by loris             #+#    #+#             */
/*   Updated: 2023/02/06 14:50:04 by loris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

// struct that will enable to send severable variable in one argument through pthread_create()
typedef struct 
{
	int fork;	// 0 free - 1 used
	enum State { on, off, dead} philosopher_state;
	int eat_counter;
	struct timeval last_ate;
	struct timeval gettime;
} thread_state;
	
typedef struct
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	pthread_mutex_t mutex;
	struct timeval start;
	int philosopher_id;
	thread_state info[];
} thread_data;

#endif
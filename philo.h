/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loris <loris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:06:33 by loris             #+#    #+#             */
/*   Updated: 2023/02/09 09:33:20 by loris            ###   ########.fr       */
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

typedef enum e_state
{
	on,
	off,
	dead
}				t_state;

typedef struct s_philo_struct
{
	t_state			philosopher_state;
	int				fork;
	int				eat_counter;
	struct timeval	last_ate;
	struct timeval	gettime;
}	t_thread_philo;

typedef struct s_data_struct
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	mutex;
	struct timeval	start;
	int				philosopher_id;
	t_thread_philo	info[];
}	t_thread_data;

#endif
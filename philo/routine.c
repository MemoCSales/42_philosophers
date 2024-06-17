/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:28:56 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/17 12:06:31 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	*routine(void *arg)
// {
// 	t_philo			*philo;
// 	pthread_mutex_t *first_fork;
// 	pthread_mutex_t	*second_fork;
// 	int	i;
	
// 	i = 0;
// 	philo = (t_philo *)arg;
	
// 	while (i < 10)
// 	{
// 		if (philo->id % 2 == 0)
// 		{
// 			first_fork = &philo->fork_left;
// 			second_fork = &philo->fork_right;
// 		}
// 		else
// 		{
// 			first_fork = &philo->fork_right;	
// 			second_fork = &philo->fork_left;
// 		}
// 		pthread_mutex_lock(first_fork);
// 		pthread_mutex_lock(second_fork);
		
// 		if (philo->id == 0)
// 			printf(RED"Philosopher is eating\n"DEFAULT);
// 		if (philo->id == 1)
// 			printf(GREEN"Philosopher is eating\n"DEFAULT);
// 		if (philo->id == 2)
// 			printf(LIGHT_CYAN"Philosopher is eating\n"DEFAULT);
// 		if (philo->id == 3)
// 			printf(YELLOW"Philosopher is eating\n"DEFAULT);
// 		if (philo->id == 4)
// 			printf(ORANGE"Philosopher is eating\n"DEFAULT);
// 		i++;

// 		pthread_mutex_unlock(second_fork);
// 		pthread_mutex_unlock(first_fork);
// 	}
// 	return (NULL);
// }

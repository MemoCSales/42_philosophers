/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:34:46 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/26 20:41:52 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_data(char **argv)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->data = malloc(sizeof(t_data));
	if (!philo->data)
	{
		free(philo);
		return (NULL);
	}
	philo->data->num_philos = ft_atoi(argv[1]);
	philo->data->time_to_die = (int64_t)ft_atoi(argv[2]);
	philo->data->time_to_eat = (int64_t)ft_atoi(argv[3]);
	philo->data->time_to_sleep = (int64_t)ft_atoi(argv[4]);
	philo->data->dead = 0;
	philo->data->fed = 0;
	if (mutex_init(philo) != 0)
		return (NULL);
	if (argv[5] != NULL)
		philo->data->number_of_meals = ft_atoi(argv[5]);
	else
		philo->data->number_of_meals = -1;
	return (philo);
}

int	mutex_init(t_philo *philo)
{
	if (pthread_mutex_init(&philo->data->mutex_dead, NULL) != 0
		|| pthread_mutex_init(&philo->data->mutex_meal, NULL) != 0
		|| pthread_mutex_init(&philo->data->mutex_start, NULL) != 0
		|| pthread_mutex_init (&philo->data->mutex_time, NULL) != 0)
	{
		free(philo->data);
		free(philo);
		return (-1);
	}
	return (0);
}

int	init_philo(t_data *data)
{
	t_philo	*philo_thread;
	int		i;

	i = 0;
	philo_thread = malloc(data->num_philos * sizeof(t_philo));
	if (!philo_thread)
	{
		printf("Error allocating philo struct\n");
		return (0);
	}
	if (!philos_initialization(philo_thread, data))
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		philo_thread[i].fork_right = philo_thread[(i - 1 + data->num_philos) % data->num_philos].fork_left;
		// printf("Philosopher %d has fork %p to his left and fork %p to his right\n", philo_thread[i].id, philo_thread[i].fork_left, philo_thread[i].fork_right);
		i++;
	}
	data->philos = philo_thread;
	init_threads(philo_thread, data);
	return (1);
}

int	philos_initialization(t_philo *philo_thread, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philo_thread[i].id = i;
		philo_thread[i].last_meal = ft_time();
		philo_thread[i].meals = 0;
		philo_thread[i].dead = 0;
		philo_thread[i].data = data;
		philo_thread[i].fork_left = malloc(sizeof(pthread_mutex_t));
		if (!philo_thread[i].fork_left || pthread_mutex_init(philo_thread[i].fork_left, NULL) != 0)
		{
			while (i-- > 0)
			{
				pthread_mutex_destroy(philo_thread[i].fork_left);
				free(philo_thread[i].fork_left);
			}
			free(philo_thread);
			return (0);
		}
		// printf("Philosopher %d initialized with last meal time: %d\n", philo_thread[i].id, philo_thread[i].last_meal);
		i++;
	}
	return (1);
}

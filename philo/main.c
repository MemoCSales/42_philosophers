/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/27 14:05:11 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Checks if a philosopher has died.
 *
 * @param philo Pointer to the philosopher struct.
 * @return 1 if the philosopher has died, 0 otherwise.
 */
int	philo_died(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->data->mutex_dead);
	result = philo->data->dead;
	pthread_mutex_unlock(&philo->data->mutex_dead);
	return (result);
}

/**
 * @brief Initializes threads for each philosopher.
 *
 * @param philo Array of philosopher structures.
 * @param data Shared data among philosophers.
 */
void	init_threads(t_philo *philo, t_data *data)
{
	int	i;

	philo->data->time = ft_time();
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&philo[i].th, NULL, routine, &philo[i]) != 0)
			return ;
		// printf("Philosopher thread [%d] is created\n", philo[i].id);
		i++;
	}
}

/**
 * @brief Determines if the philosopher has died.
 *
 * @param philo Pointer to the philosopher's structure.
 * @return 1 if dead, otherwise 0.
 */
int	is_dead(t_philo *philo)
{
	int	i;
	int	current_time;

	pthread_mutex_lock(&philo->data->mutex_meal);
	current_time = ft_time();
	i = 0;
	while (i < philo->data->num_philos)
	{
		if (current_time > philo->data->philos[i].last_meal
			+ philo->data->time_to_die)
		{
			pthread_mutex_unlock(&philo->data->mutex_meal);
			message(&philo[i], DIED);
			pthread_mutex_lock(&philo->data->mutex_dead);
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->data->mutex_dead);
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&philo->data->mutex_meal);
	return (0);
}

/**
 * @brief Checks if all philosophers have reached their meal quota.
 *
 * @param philo Pointer to philosopher structure.
 * @return 1 if all are full, 0 otherwise.
 */
int	is_full(t_philo *philo)
{
	int	i;
	int	full;

	pthread_mutex_lock(&philo->data->mutex_meal);
	if (philo->data->number_of_meals == -1)
	{
		pthread_mutex_unlock(&philo->data->mutex_meal);
		return (0);
	}
	i = 0;
	full = 1;
	while (i < philo->data->num_philos)
	{
		if (philo->data->philos[i].meals < philo->data->number_of_meals)
		{
			full = 0;
			break ;
		}
		i++;
	}
	if (full)
		philo->data->fed = 1;
	pthread_mutex_unlock(&philo->data->mutex_meal);
	return (full);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;

	if (error_check(argc, argv))
	{
		printf(RED "Double check the arguments passed to the command line.\n"
			DEFAULT);
		return (1);
	}
	philos = init_data(argv);
	if (check_initialization(philos))
		return (1);
	check_if_dead_or_full(philos);
	if (join_and_destroys(philos))
		return (1);
	return (0);
}

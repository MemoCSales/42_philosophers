/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:38:43 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/26 20:31:34 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Synchronizes meal time for a philosopher.
 *
 * Locks the meal mutex, increments the philosopher's meal count, and updates
 * their last meal time to current time plus time to die, ensuring meal times
 * are synchronized with simulation requirements.
 *
 * @param philo Pointer to philosopher structure.
 */
void	sync_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_meal);
	philo->meals += 1;
	philo->last_meal = ft_time();
	// printf("Philosopher %d Meals %d\n", philo->id, philo->meals);
	pthread_mutex_unlock(&philo->data->mutex_meal);
}

/**
 * @brief Initializes the eating process for a philosopher.
 *
 * This function sets up the necessary conditions for a philosopher to start
 * eating. It involves acquiring forks, setting the eating flag, and updating
 * the philosopher's last meal time.
 *
 * @param philo Pointer to the philosopher structure.
 * @return int Returns 0 on success, or a non-zero error code on failure.
 */
int	starting(t_philo *philo)
{
	if (philo->data->num_philos == 1)
		return (handle_1_philo(philo));
	if (philo->id % 2 == 1 && philo->meals == 0)
	{
		message(philo, THINKING);
		ft_usleep(philo->data->time_to_eat / 2);
	}
	if (philo->id % 2 == 0)
	{
		if (take_fork_right(philo, FIRST_FORK))
			return (1);
		if (take_fork_left(philo, SECOND_FORK))
			return (pthread_mutex_unlock(philo->fork_right), 1);
	}
	else
	{
		if (take_fork_left(philo, FIRST_FORK))
			return (1);
		if (take_fork_right(philo, SECOND_FORK))
			return (pthread_mutex_unlock(philo->fork_left), 1);
	}
	return (0);
}

/**
 * @brief Attempts to take the right fork for a philosopher.
 *
 * This function tries to acquire the right fork for the philosopher specified
 * by the philo parameter. If successful, it logs the action using the provided
 * message.
 *
 * @param philo Pointer to the philosopher structure.
 * @param msg Message to log upon successful fork acquisition.
 * @return int Returns 0 if the fork was successfully taken, otherwise returns
 * a non-zero error code.
 */
int	take_fork_right(t_philo *philo, char *msg)
{
	if (philo_died(philo))
		return (1);
	pthread_mutex_lock(philo->fork_right);
	message(philo, msg);
	return (0);
}

/**
 * @brief Attempts to take the left fork for a philosopher.
 *
 * This function tries to acquire the left fork for the philosopher specified
 * by the philo parameter. If successful, it logs the action using the provided
 * message.
 *
 * @param philo Pointer to the philosopher structure.
 * @param msg Message to log upon successful fork acquisition.
 * @return int Returns 0 if the fork was successfully taken, otherwise returns
 * a non-zero error code.
 */
int	take_fork_left(t_philo *philo, char *msg)
{
	if (philo_died(philo))
		return (1);
	pthread_mutex_lock(philo->fork_left);
	message(philo, msg);
	return (0);
}

/**
 * @brief Checks if all philosophers have finished eating.
 *
 * This function iterates through all philosophers and checks if each one has
 * finished their required number of meals. It is used to determine if the
 * simulation can end.
 *
 * @param philo Pointer to the first philosopher in the array.
 * @return int Returns 1 if all philosophers have finished eating, otherwise 0.
 */
int	all_eaten(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->data->mutex_meal);
	result = philo->data->fed;
	pthread_mutex_unlock(&philo->data->mutex_meal);
	return (result);
}

int	handle_1_philo(t_philo *philo)
{
	message(philo, THINKING);
	take_fork_left(philo, FIRST_FORK);
	// ft_usleep(philo->data->time_to_die);
	sync_meal_time(philo);
	pthread_mutex_unlock(philo->fork_left);
	return (1);
}
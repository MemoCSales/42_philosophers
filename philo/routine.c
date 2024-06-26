/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:28:56 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/26 16:14:13 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief This is the routine function for each philosopher in the simulation.
 *
 * @param arg A void pointer that should be cast to a pointer to a t_philo
 *            structure. This structure contains the philosopher's ID, the
 *            mutexes for their left and right forks, and a pointer to a
 *            shared data structure.
 *
 * @return This function always returns NULL. It is intended to be used as a
 *         thread routine, so the return value is ignored.
 *
 * The function first waits for the start_flag in the shared data structure to
 * be set. This ensures that all philosophers start at the same time.
 */
void	*routine(void *arg)
{
	t_philo			*philo;
	int				start;

	philo = (t_philo *)arg;
	start = 0;

	philo->last_meal = ft_time();
	while (!start)
	{
		pthread_mutex_lock(&philo->data->mutex_start);
		if (philo->data->start_flag)
			start = 1;
		pthread_mutex_unlock(&philo->data->mutex_start);
	}
	// printf("Philosopher %d last meal time before update: %d\n", philo->id, philo->last_meal);
	pthread_mutex_lock(&philo->data->mutex_meal);
	philo->last_meal = ft_time() + philo->data->time_to_die;
	pthread_mutex_unlock(&philo->data->mutex_meal);
	// printf("Philosopher %d last meal time after update: %d\n", philo->id, philo->last_meal);
	while (1)
	{
		if (eat(philo))
			break;
		if (ft_sleep(philo))
			break;
		if (think(philo))
			break;
	}
	return (NULL);
}

/**
 * @brief Manages the eating action of a philosopher.
 *
 * This function initiates the eating process for a philosopher. It first
 * checks if the philosopher can start eating by calling `starting(philo)`.
 * If the philosopher can start, it proceeds to log the eating action with
 * `message(philo, EATING)`, synchronizes the meal time, and then makes the
 * philosopher sleep for `philo->data->time_to_eat` milliseconds to simulate
 * eating duration. After eating, it unlocks both the left and right forks.
 *
 * @param philo Pointer to the philosopher structure.
 * @return Returns 1 if the philosopher cannot start eating, otherwise 0.
 */
int	eat(t_philo *philo)
{
	if (starting(philo))
		return (1);
	message(philo, EATING);
	sync_meal_time(philo);
	ft_usleep(philo->data->time_to_eat);
	// printf("Philosopher: %d releases left fork\n", philo->id);
	pthread_mutex_unlock(philo->fork_left);
	// printf("Philosopher: %d releases right fork\n", philo->id);
	pthread_mutex_unlock(philo->fork_right);
	return (0);
}

/**
 * @brief Manages the sleeping action of a philosopher.
 *
 * After eating, a philosopher will call this function to simulate sleeping.
 * It first checks if the philosopher has died or if all philosophers have
 * eaten the required number of meals. If neither condition is met, it logs
 * the sleeping action with `message(philo, SLEEPING)` and then makes the
 * philosopher sleep for `philo->data->time_to_sleep` milliseconds.
 *
 * @param philo Pointer to the philosopher structure.
 * @return Returns 1 if the philosopher has died or all have eaten, otherwise 0.
 */
int	ft_sleep(t_philo *philo)
{
	if (philo_died(philo) || all_eaten(philo))
		return (1);
	message(philo, SLEEPING);
	ft_usleep(philo->data->time_to_sleep);
	return (0);
}

/**
 * @brief Manages the thinking action of a philosopher.
 *
 * This function is called when a philosopher is in the thinking state. It
 * checks if the philosopher has died or if all philosophers have eaten the
 * required number of meals. If neither condition is met, it logs the thinking
 * action with `message(philo, THINKING)`. This function prepares the philosopher
 * for the next cycle of actions (eating, sleeping, thinking).
 *
 * @param philo Pointer to the philosopher structure.
 * @return Returns 1 if the philosopher has died or all have eaten, otherwise 0.
 */
int	think(t_philo *philo)
{
	if (philo_died(philo) || all_eaten(philo))
		return (1);
	message(philo, THINKING);
	return (0);
}

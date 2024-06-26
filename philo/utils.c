/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:18:58 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/27 14:25:33 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	number;

	sign = 0;
	number = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' && *(str + 1) != '-')
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		number *= 10;
		number += *str - 48;
		str++;
	}
	if (sign % 2)
		return (-number);
	else
		return (number);
}

int	error_check(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (1);
	if (ft_atoi(argv[1]) < 0)
		return (1);
	if (ft_atoi(argv[2]) < 60)
		return (1);
	if (ft_atoi(argv[3]) < 60)
		return (1);
	if (ft_atoi(argv[4]) < 60)
		return (1);
	if (argc == 6 && ft_atoi(argv[5]) < 1)
		return (1);
	return (0);
}

/**
 * @brief Prints a message for a philosopher's action.
 *
 * This function locks the mutex_meal and mutex_dead to ensure thread safety
 * before checking if the philosopher is not dead and not all philosophers
 * have eaten the required number of meals. If these conditions are met,
 * it prints a timestamp, the philosopher's ID, and the specified message.
 *
 * @param philo Pointer to the philosopher structure.
 * @param msg The message to print.
 */
void	message(t_philo *philo, char *msg)
{
	int	time;

	time = ft_time() - get_start_time(philo->data);
	pthread_mutex_lock(&philo->data->mutex_meal);
	pthread_mutex_lock(&philo->data->mutex_dead);
	if (!philo->data->dead && philo->data->fed == 0)
		printf("%d %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->mutex_dead);
	pthread_mutex_unlock(&philo->data->mutex_meal);
}

void	print_philo_struct(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		printf("i: %d\n", i);
		printf("Philosopher ID: %d\n", philos[i].id);
		printf("Philosopher last meal time: %d\n", philos[i].last_meal);
		printf("Philosopher # of meals: %d\n", philos[i].meals);
		i++;
	}
}

int	join_and_destroys(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->data->num_philos)
	{
		if (pthread_join(philos->data->philos[i].th, NULL) != 0)
			return (printf("Error joining\n"), 1);
		i++;
	}
	i = 0;
	while (i < philos->data->num_philos)
	{
		pthread_mutex_destroy(philos->data->philos[i].fork_right);
		free(philos->data->philos[i].fork_right);
		i++;
	}
	pthread_mutex_destroy(&philos->data->mutex_dead);
	pthread_mutex_destroy(&philos->data->mutex_meal);
	pthread_mutex_destroy(&philos->data->mutex_start);
	pthread_mutex_destroy(&philos->data->mutex_time);
	free(philos->data->philos);
	free(philos->data);
	free(philos);
	return (0);
}

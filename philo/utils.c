/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:18:58 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/13 16:02:53 by mcruz-sa         ###   ########.fr       */
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

void	print_data_struct(t_data *data)
{
	printf("Number of philosophers: %d\n", data->num_philos);
    printf("Time to die: %d\n", data->time_to_die);
    printf("Time to eat: %d\n", data->time_to_eat);
    printf("Time to sleep: %d\n", data->time_to_sleep);
    printf("Number of meals: %d\n", data->number_of_meals);
}

void	print_philo_struct(t_data *data, t_philo *philos)
{
	int	i = 0;
	while (i < data->num_philos)
	{
		printf("i: %d\n", i);
		printf("Philosopher ID: %d\n", philos[i].id);
		printf("Philosopher die time: %d\n", philos[i].die);
		printf("Philosopher eat time: %d\n", philos[i].eat);
		printf("Philosopher sleep time: %d\n", philos[i].sleep);
		printf("Philosopher last meal time: %d\n", philos[i].last_meal);
		printf("Total of philos & forks: %d\n", philos[i].total);
		// printf("Philosopher status: %d\n", philos[i].status);
		// printf("Philosopher # of meals: %d\n", philos[i].goal);
		// printf("Philosopher [%d] has right fork %d\n", philos[i].id, philos[i].fork_right);
		// printf("Philosopher [%d] has left fork %d\n", philos[i].id, philos[i].fork_left);
		i++;
	}
}

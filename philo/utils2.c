/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:57:05 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/27 14:00:21 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_initialization(t_philo *philos)
{
	if (!philos)
		return (1);
	if (!init_philo(philos->data))
	{
		free(philos->data);
		free(philos);
		return (1);
	}
	if (!philos->data)
	{
		printf("Data structure not initialized.\n");
		free(philos);
		return (1);
	}
	return (0);
}

void	check_if_dead_or_full(t_philo *philos)
{
	ft_usleep(100);
	while (1)
	{
		if (is_dead(philos->data->philos) || is_full(philos->data->philos))
			break ;
	}
}

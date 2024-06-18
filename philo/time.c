/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:26:53 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/18 12:33:51 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Gets the current time in milliseconds.
 *
 * This function uses the gettimeofday function to get the current time, which is
 * then converted to milliseconds. If gettimeofday encounters an error, an error
 * message is printed.
 *
 * @return The current time in milliseconds.
 */
int	ft_time(void)
{
	struct	timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf("Error with gettimeofday\n");
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

/**
 * @brief Pauses the execution of the program for a specified number of milliseconds.
 *
 * This function uses a while loop to pause the program until the specified number
 * of milliseconds has passed. It uses the get_time function to get the current time
 * and usleep to pause the program.
 *
 * @param milisec The number of milliseconds to pause the program.
 * @return Always returns 0.
 */
int	ft_usleep(int milisec)
{
	int	start;

	start = ft_time();
	while (ft_time() < start + milisec)
		usleep(1000);
	return (0);
}

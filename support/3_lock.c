/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_lock.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:44:34 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/07 18:06:13 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t	mutex;

void	*routine(void *arg)
{
	int	*input;

	input = (int *)arg;
	pthread_mutex_lock(&mutex);
	printf("Input value: %d\n", *input);
	printf("Got lock\n");
	sleep(1);
	pthread_mutex_unlock(&mutex);
	return (NULL);
}

int	main(void)
{
	pthread_t th[4];
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < 4; i++)
	{
		int *arg = malloc(sizeof(*arg));
		if (arg == NULL)
		{
			perror("Failed to allocate memory");
			exit(1);
		}
		*arg = i;
		if (pthread_create(&th[i], NULL, &routine, arg) != 0)
		{
			perror("Error at creating thread");
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("Error at joining thread");
		}
	}
	pthread_mutex_destroy(&mutex);
	return (0);
}
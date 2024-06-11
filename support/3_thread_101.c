/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_thread_101.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:19:25 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/11 14:38:20 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


void	*print_hello(void *arg)
{
	int	i;
	(void)arg;

	i = 0;
	while (i < 100)
	{
		printf("Hello\n");
		i++;
		usleep(100);
	}
	return (NULL);
}

void	*print_world(void *arg)
{
	int	i;
	(void)arg;
	
	i = 0;
	while (i < 100)
	{
		printf("\tWorld\n");
		i++;
		usleep(100);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	int	*num;
	int	i;

	num = (int *)arg;
	i = 0;
	while (i < 100000)
	{
		(*num)++;
		i++;
	}
	return (NULL);	
}

//Thread creation and waiting
void    example()
{
	pthread_t	t1;
	pthread_t	t2;

	if (pthread_create(&t1, NULL, print_hello, NULL))
		exit(1);
	if (pthread_create(&t2, NULL, print_world, NULL))
		exit(2);
	
	if (pthread_join(t1, NULL))
		exit(3);
	if (pthread_join(t2, NULL))
		exit(4);
}

void	example_3()
{
	pthread_t	t1;
	pthread_t	t2;
	int			*num;
	
	num = malloc(sizeof(int));
	if (!num)
		exit(1);
	*num = 0;
	if (pthread_create(&t1, NULL, routine, num))
		exit(1);
	if (pthread_create(&t2, NULL, routine, num))
		exit(2);
	
	if (pthread_join(t1, NULL))
		exit(3);
	if (pthread_join(t2, NULL))
		exit(4);

	printf("NUM: %d\n", *num);
}

int main(void)
{
	// example();
	example_3();
	return (0);
}
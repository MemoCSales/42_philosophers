/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/13 18:38:07 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_data_and_philos(char **argv)
{
	t_philo	*philo;

	philo = malloc(1 * sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->data = malloc (1 * sizeof(t_data));
	if (!philo->data)
	{
		free(philo);
		return (NULL);
	}
	philo->data->num_philos = ft_atoi(argv[1]);
	philo->data->time_to_die = ft_atoi(argv[2]);
	philo->data->time_to_eat = ft_atoi(argv[3]);
	philo->data->time_to_sleep = ft_atoi(argv[4]);
	philo->data->mutex_stop = malloc(sizeof(pthread_mutex_t));
	if (!philo->data->mutex_stop)
	{
		free(philo->data);
		free(philo);
		return (NULL);
	}
	if (pthread_mutex_init(philo->data->mutex_stop, NULL))
	{
		free(philo->data->mutex_stop);
		free(philo->data);
		free(philo);
		return (NULL);
	}
	philo->data->num_started = 0;
	if (argv[5] != NULL)
		philo->data->number_of_meals = ft_atoi(argv[5]);
	else
		philo->data->number_of_meals = 0;
	if (!init_philo(philo->data))
	{
		free(philo->data);
		free(philo);
		return (NULL);
	}
	return (philo);
}

int	init_philo(t_data *data)
{
	t_philo *philo_thread;
	int		i;

	i = 0;
	philo_thread = malloc(data->num_philos * sizeof(t_philo));
	if (!philo_thread)
	{
		printf("Error allocating philo struct\n");
		return (0);
	}
	while (i < data->num_philos)
	{
		philo_thread[i].id = i;
		philo_thread[i].die = 0;
		philo_thread[i].eat = 0;
		philo_thread[i].sleep = 0;
		philo_thread[i].last_meal = 0;
		philo_thread[i].total = data->num_philos;
		philo_thread[i].data = data;
		if (pthread_mutex_init(&philo_thread[i].fork_left, NULL))
		{
			free(philo_thread);
			return (0);
		}
		if (pthread_mutex_init(&philo_thread[i].fork_right, NULL))
		{
			free(philo_thread);
			return (0);
		}
		i++;
	}
	init_threads(philo_thread);
	return (1);
}	

void	init_threads(t_philo *philo)
{
	int	i;
	
	i = 0;
	philo->data->time_start = ft_time();
	while (i < philo->data->num_philos)
	{
		if (pthread_create(&philo[i].th, NULL, &routine, &philo[i]) != 0)
			return ;
		printf("Philosopher thread [%d] is created\n", philo[i].id);
		i++;
	}
}

/*
* Gets the current time in miliseconts. Uses the gettimeofday functoin to get the curren time.
* tv_sec -> is the number of seconds since the Epoch
* tv_usec -> is the number of microseconds past the time in tv_sec
* @return the current time in miliseconds
*/
int	ft_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("Error in ft_time\n");
		return (-1);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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

int main(int argc, char **argv)
{
	t_data	data;
	t_philo *philos;
	int		i;

	if (error_check(argc, argv))
	{
		printf("Double check the arguments passed to the command line.\n");
		return (1);
	}
	philos = init_data_and_philos(argv);
	print_data_struct(&data);		
	// philos = init_philo(&data);
	if (philos == NULL)
		return (1);
	// pthread_mutex_init(philos->mutex_stop, NULL);
	// print_philo_struct(&data, philos);
	// i = 0;
	// while (i < data.num_philos)
	// {
	// 	if (pthread_create(&philos[i].th, NULL, &routine, &philos[i]) != 0)
	// 		return (2);
	// 	printf("Philosopher thread [%d] is created\n", philos[i].id);
	// 	i++;
	// }
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_join(philos[i].th, NULL) != 0)
			return (3);
		printf("Philosopher thread [%d] is finishing execution\n", philos[i].id);
		i++;
	}
	// pthread_mutex_destroy(philos->mutex_stop);
	printf("Playing: %d\n", philos->total);
	pthread_mutex_destroy(philos->data->mutex_stop);
	free(philos->data->mutex_stop);
	return (0);
}

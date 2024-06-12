/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/12 19:57:10 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **argv)
{
	int	ms;

	ms = 1000;
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]) * ms;
	data->time_to_eat = ft_atoi(argv[3]) * ms;
	data->time_to_sleep = ft_atoi(argv[4]) * ms;
	if (argv[5] != NULL)
		data->number_of_meals = ft_atoi(argv[5]);
	else
		data->number_of_meals = 0;
}

t_philo	*init_philo(t_data *data)
{
	int		i;
	t_philo *philo;

	i = 0;
	philo = malloc(data->num_philos * sizeof(t_philo));
	if (!philo)
	{
		printf("Error allocating philo struct\n");
		return (NULL);
	}
	while (i < data->num_philos)
	{
		philo[i].id = i;
		philo[i].die = 0;
		philo[i].eat = 0;
		philo[i].sleep = 0;
		philo[i].last_meal = 0;
		philo[i].status = -1;
		philo[i].goal = 0;
		philo[i].total = data->num_philos;
		pthread_mutex_init(&philo[i].fork_left, NULL);
		pthread_mutex_init(&philo[i].fork_right, NULL);
		// philo[i].fork_left = -1;
		// philo[i].fork_right = -1;
		// philo[i].stop = 0;
		// philo[i].final_goal = 0;
		i++;
	}
	return (philo);
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
		printf("Philosopher status: %d\n", philos[i].status);
		printf("Philosopher # of meals: %d\n", philos[i].goal);
		printf("Total of philos & forks: %d\n", philos[i].total);
		// printf("Philosopher [%d] has right fork %d\n", philos[i].id, philos[i].fork_right);
		// printf("Philosopher [%d] has left fork %d\n", philos[i].id, philos[i].fork_left);
		i++;
	}
}

int	error_check(int argc)
{
	if (argc < 5)
	{
		printf("Error: Not enough arguments to run the program\n");
		return (1);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t *first_fork;
	pthread_mutex_t	*second_fork;
	int	i;

	i = 0;
	philo = (t_philo *)arg;
	while (i < 10)
	{
		if (philo->id % 2 == 0)
		{
			first_fork = &philo->fork_left;
			second_fork = &philo->fork_right;
		}
		else
		{
			first_fork = &philo->fork_right;	
			second_fork = &philo->fork_left;
		}
		pthread_mutex_lock(first_fork);
		pthread_mutex_lock(second_fork);
		
		philo->total++;
		printf("Philosopher is eating\n");
		i++;

		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	t_data	data;
	t_philo *philos;
	pthread_mutex_t	stop;
	int		i;

	if (!error_check(argc))
	{
		init_data(&data, argv);
		print_data_struct(&data);		
	}
	philos = init_philo(&data);
	if (philos == NULL)
		return (1);
	pthread_mutex_init(&stop, NULL);
	// print_philo_struct(&data, philos);
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_create(&philos[i].th, NULL, &routine, &philos[i]) != 0)
			return (2);
		printf("Philosopher thread [%d] is created\n", philos[i].id);
		i++;
	}
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_join(philos[i].th, NULL) != 0)
			return (3);
		printf("Philosopher thread [%d] is finishing execution\n", philos[i].id);
		i++;
	}
	pthread_mutex_destroy(&stop);
	printf("Playing: %d\n", philos->total);
	return (0);
}

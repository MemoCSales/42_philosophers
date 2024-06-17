/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/14 19:46:08 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		data->number_of_meals = ft_atoi(argv[5]);
	else
		data->number_of_meals = 0;
	pthread_mutex_init(&data->mutex_dead, NULL);
	pthread_mutex_init(&data->mutex_meal, NULL);
	pthread_mutex_init(&data->mutex_print,NULL);
	pthread_mutex_init(&data->mutex_start, NULL);
	data->start_flag = 0;
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
		philo[i].last_meal = 0;
		philo[i].status = -1;
		philo[i].meals= 0;
		philo[i].total = data->num_philos;
		philo[i].data = data;
		pthread_mutex_init(&philo[i].fork_left, NULL);
		pthread_mutex_init(&philo[i].fork_right, NULL);
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
		printf("Philosopher last meal time: %d\n", philos[i].last_meal);
		printf("Philosopher status: %d\n", philos[i].status);
		printf("Philosopher # of meals: %d\n", philos[i].meals);
		printf("Total of philos & forks: %d\n", philos[i].total);
		// printf("Philosopher die time: %d\n", philos[i].die);
		// printf("Philosopher eat time: %d\n", philos[i].eat);
		// printf("Philosopher sleep time: %d\n", philos[i].sleep);
		// printf("Philosopher [%d] has right fork %d\n", philos[i].id, philos[i].fork_right);
		// printf("Philosopher [%d] has left fork %d\n", philos[i].id, philos[i].fork_left);
		i++;
	}
}

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
 *
 */
void	*routine(void *arg)
{
	t_philo			*philo;

	int	i;
	int	start;

	i = 0;
	start = 0;
	philo = (t_philo *)arg;
	while (!start)
	{
		pthread_mutex_lock(&philo->data->mutex_start);
		if (philo->data->start_flag)
			start = 1;
		pthread_mutex_unlock(&philo->data->mutex_start);
	}
	while (i < philo->data->number_of_meals)
	{
		printf("Philosopher %d is thinking\n", philo->id);
		pthread_mutex_lock(&philo->fork_left);
		pthread_mutex_lock(&philo->fork_right);
		printf("Philosoppher %d is eating\n", philo->id);
		pthread_mutex_unlock(&philo->fork_left);
		pthread_mutex_unlock(&philo->fork_right);
		printf("Philosopher %d is sleeping\n", philo->id);
		i++;
	}
	return (NULL);
}

void	init_thread(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&philo[i].th, NULL, &routine, &philo[i]) != 0)
			return ;
		printf("Philosopher thread [%d] is created\n", philo[i].id);
		i++;
	}
	pthread_mutex_lock(&data->mutex_start);
	data->start_flag = 1;
	pthread_mutex_unlock(&data->mutex_start);
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
	init_data(&data, argv);
	print_data_struct(&data);		
	philos = init_philo(&data);
	if (philos == NULL)
		return (1);
	init_thread(philos, &data);
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_join(philos[i].th, NULL) != 0)
			return (3);
		printf("Philosopher thread [%d] is finishing execution\n", philos[i].id);
		i++;
	}
	printf("Playing: %d\n", philos->total);
	return (0);
}

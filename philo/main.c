/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/17 12:09:41 by mcruz-sa         ###   ########.fr       */
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
	if (argv[5] != NULL)
		philo->data->number_of_meals = ft_atoi(argv[5]);
	else
		philo->data->number_of_meals = 0;
	print_data_struct(philo->data);
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
		philo_thread[i].last_meal = 0;
		philo_thread[i].status = -1;
		philo_thread[i].meals= 0;
		philo_thread[i].total = data->num_philos;
		philo_thread[i].data = data;
		pthread_mutex_init(&philo_thread[i].fork_left, NULL);
		pthread_mutex_init(&philo_thread[i].fork_right, NULL);
		i++;
	}
	init_threads(philo_thread, data);
	return (1);
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

void	init_threads(t_philo *philo, t_data *data)
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
	t_philo *philos;
	int		i;

	if (error_check(argc, argv))
	{
		printf("Double check the arguments passed to the command line.\n");
		return (1);
	}
			
	philos = init_data_and_philos(argv);
	i = 0;
	while (i < philos->data->num_philos)
	{
		if (pthread_join(philos[i].th, NULL) != 0)
			return (3);
		printf("Philosopher thread [%d] is finishing execution\n", philos[i].id);
		i++;
	}
	printf("Playing: %d\n", philos->total);
	pthread_mutex_destroy(philos->data->mutex_stop);
	free(philos->data->mutex_stop);
	return (0);
}

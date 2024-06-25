/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/25 15:44:40 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_data(char **argv)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->data = malloc(sizeof(t_data));
	if (!philo->data)
	{
		free(philo);
		return (NULL);
	}
	philo->data->num_philos = ft_atoi(argv[1]);
	philo->data->time_to_die = ft_atoi(argv[2]);
	philo->data->time_to_eat = ft_atoi(argv[3]);
	philo->data->time_to_sleep = ft_atoi(argv[4]);
	philo->data->dead = 0;
	philo->data->fed = 0;
	if (pthread_mutex_init(&philo->data->mutex_dead, NULL) != 0 ||
		pthread_mutex_init(&philo->data->mutex_meal, NULL) != 0 ||
		pthread_mutex_init(&philo->data->mutex_start, NULL) != 0)
	{
		free(philo->data);
		free(philo);
		return (NULL);
	}
	if (argv[5] != NULL)
		philo->data->number_of_meals = ft_atoi(argv[5]);
	else
		philo->data->number_of_meals = -1;
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
		philo_thread[i].meals = 0;
		philo_thread[i].dead = &data->dead;
		philo_thread[i].data = data;
		philo_thread[i].fork_left = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(philo_thread[i].fork_left, NULL) != 0)
		{
			free(philo_thread);
			return (0);
		}
		// printf("Philosopher %d initialized with last meal time: %d\n", philo_thread[i].id, philo_thread[i].last_meal);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		philo_thread[i].fork_right = philo_thread[(i - 1 + data->num_philos) % data->num_philos].fork_left;
		// printf("Philosopher %d has fork %p to his left and fork %p to his right\n", philo_thread[i].id, philo_thread[i].fork_left, philo_thread[i].fork_right);
		i++;
	}
	// printf("Before init thread\n");
	data->philos = philo_thread;
	init_threads(philo_thread, data);
	return (1);
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
	// int				start;

	philo = (t_philo *)arg;
	// start = 0;
	// printf("Philosopher %d started\n", philo->id);
	// printf("Start_flag: %d\n", philo->data->start_flag);
	philo->last_meal = ft_time() + philo->data->time_to_die;
	// while (!start)
	// {
	// 	pthread_mutex_lock(&philo->data->mutex_start);
	// 	// printf("philo->data->start_flag: %d\n", philo->data->start_flag);
	// 	if (philo->data->start_flag)
	// 		start = 1;
	// 	pthread_mutex_unlock(&philo->data->mutex_start);
	// 	ft_usleep(100);
	// }
	// printf("Philosopher %d last meal time before update: %d\n", philo->id, philo->last_meal);
	pthread_mutex_lock(&philo->data->mutex_meal);
	philo->last_meal = ft_time() + philo->data->time_to_die;
	pthread_mutex_unlock(&philo->data->mutex_meal);
	// printf("Philosopher %d last meal time after update: %d\n", philo->id, philo->last_meal);
	while (1)
	{
		if (eat(philo))
			break;
		if (ft_sleep(philo))
			break;
		if (think(philo))
			break;
	}
	return (NULL);
}

int	think(t_philo *philo)
{
	if (philo_died(philo) || all_eaten(philo))
		return (1);
	message(philo, THINKING);
	return (0);
}

int	ft_sleep(t_philo *philo)
{
	if (philo_died(philo) || all_eaten(philo))
		return (1);
	message(philo, SLEEPING);
	ft_usleep(philo->data->time_to_sleep);
	return (0);
}

int	all_eaten(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->data->mutex_meal);
	result = philo->data->fed;
	pthread_mutex_unlock(&philo->data->mutex_meal);
	return(result);
}

int	eat(t_philo *philo)
{
	if (starting(philo))
		return (1);
	message(philo, EATING);
	sync_meal_time(philo);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
	return (0);
}

void	sync_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_meal);
	philo->meals += 1;
	philo->last_meal = ft_time() + philo->data->time_to_die;
	// printf("Philosopher %d Meals %d\n", philo->id, philo->meals);
	// philo->last_meal = ft_time();
	// printf("Meal time: %d\n", philo->last_meal);
	pthread_mutex_unlock(&philo->data->mutex_meal);
}

int	starting(t_philo *philo)
{
	if (philo->id % 2 == 1 && philo->meals == 0)
	{
		message(philo, THINKING);
		ft_usleep(philo->data->time_to_eat / 2);
	}
	if (philo->id % 2 == 0)
	{
		if (take_fork_right(philo, FIRST_FORK))
			return (1);
		if (take_fork_left(philo, SECOND_FORK))
			return (pthread_mutex_unlock(philo->fork_right), 1);
	}
	else
	{
		if (take_fork_left(philo, FIRST_FORK))
			return (1);
		if (take_fork_right(philo, SECOND_FORK))
			return (pthread_mutex_unlock(philo->fork_left), 1);
	}
	return (0);
}

int	philo_died(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->data->mutex_dead);
	result = philo->data->dead;
	pthread_mutex_unlock(&philo->data->mutex_dead);
	return (result);
}

int	take_fork_right(t_philo *philo, char *msg)
{
	if (philo_died(philo))
		return (1);
	pthread_mutex_lock(philo->fork_right);
	message(philo, msg);
	return (0);
}

int	take_fork_left(t_philo *philo, char *msg)
{
	if (philo_died(philo))
		return (1);
	pthread_mutex_lock(philo->fork_left);
	message(philo, msg);
	return (0);
}

void	message(t_philo *philo, char *msg)
{
	int	time;

	// printf("Philo time: %d\n", philo->id);
	// exit(1);
	time = (ft_time() - philo->data->time);
	// pthread_mutex_lock(&philo->data->mutex_stop);
	pthread_mutex_lock(&philo->data->mutex_meal);
	pthread_mutex_lock(&philo->data->mutex_dead);
	if (!philo->data->dead && philo->data->fed == 0)
	// if (!philo_died(philo) && !all_eaten(philo))
		printf("%d %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->mutex_dead);
	pthread_mutex_unlock(&philo->data->mutex_meal);
	// pthread_mutex_unlock(&philo->data->mutex_stop);
}

void	init_threads(t_philo *philo, t_data *data)
{
	int	i;

	philo->data->time = ft_time();
	// printf("Philo time: %d\n", philo->data->time);
	// pthread_mutex_lock(&data->mutex_start);
	// data->start_flag = 1;
	// pthread_mutex_unlock(&data->mutex_start);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&philo[i].th, NULL, &routine, &philo[i]) != 0)
			return ;
		// printf("Philosopher thread [%d] is created\n", philo[i].id);
		i++;
	}
}

/*
Review all accesses to philo->data->dead and philo->data->fed: 
Ensure every access to these variables, both read and write, 
is protected by the same mutexes (mutex_dead and mutex_stop). 
It's crucial that the same locks are used consistently across 
the entire program for accessing these shared variables.*/

int	is_dead(t_philo *philo)
{
	int	i;
	
	// printf("Last meal time: %d\n", philo[i].last_meal);
	pthread_mutex_lock(&philo->data->mutex_meal);
	i = 0;
	while(i < philo->data->num_philos)
	{
		if (ft_time() >= philo->data->philos[i].last_meal)
		{
			pthread_mutex_unlock(&philo->data->mutex_meal);
			message(&philo[i], DIED);
			pthread_mutex_lock(&philo->data->mutex_dead);
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->data->mutex_dead);
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&philo->data->mutex_meal);
	return (0);
}

// int	is_full(t_philo *philo)
// {
// 	int	i;

// 	i = 0;
// 	pthread_mutex_lock(&philo->data->mutex_meal);
// 	if (philo->meals == -1)
// 	{
// 		printf("Exiting\n");
// 		return (pthread_mutex_unlock(&philo->data->mutex_meal), 0);
// 	}
// 	pthread_mutex_unlock(&philo->data->mutex_meal);
// 	while (i < philo->data->num_philos)
// 	{
// 		pthread_mutex_lock(&philo->data->mutex_meal);
// 		if (philo->data->philos[i].meals < philo->data->number_of_meals)
// 			return (pthread_mutex_unlock(&philo->data->mutex_meal), 0);
// 		pthread_mutex_unlock(&philo->data->mutex_meal);
// 		i++;
// 	}
// 	philo->data->fed = 1;
// 	return (1);
	
// }

int	is_full(t_philo *philo)
{
	int	i;
	int	full;

	pthread_mutex_lock(&philo->data->mutex_meal);
	if (philo->data->number_of_meals == -1)
	{
		pthread_mutex_unlock(&philo->data->mutex_meal);
		return (0);
	}
	i = 0;
	full = 1;
	while (i < philo->data->num_philos)
	{
		// pthread_mutex_lock(&philo->data->mutex_meal);
		if (philo->data->philos[i].meals < philo->data->number_of_meals)
		{
			full = 0;
			break;
			// return (pthread_mutex_unlock(&philo->data->mutex_meal), 0);
		}
		// pthread_mutex_unlock(&philo->data->mutex_meal);
		i++;
	}
	if (full)
	{
		philo->data->fed = 1;
	}
	pthread_mutex_unlock(&philo->data->mutex_meal);
	return (full);
}

int main(int argc, char **argv)
{
	t_philo *philos;
	int		i;

	if (error_check(argc, argv))
	{
		printf(RED"Double check the arguments passed to the command line.\n"DEFAULT);
		return (1);
	}
			
	philos = init_data(argv);
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
	// exit(1);
	ft_usleep(100);
	// printf("Philos: %p\n", philos);
	// printf("debugging: %d\n", philos->data->number_of_meals);
	while (1)
	{
		if (is_dead(philos->data->philos) || is_full(philos->data->philos))
			break ;
	}
	// i = 0;
	// while (i < philos->data->num_philos)
	// {
	// 	while(1)
	// 	{
	// 		if(philos->data->dead == 1)
	// 			break;
	// 	}
	// }
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
	free(philos->data->philos);
	free(philos->data);
	free(philos);
	return (0);
}

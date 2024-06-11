/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/11 18:41:18 by mcruz-sa         ###   ########.fr       */
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

t_philo	*init_philo(t_philo *philo, t_data *data)
{
	int		i;

	i = 0;
	philo = malloc((data->num_philos + 1) * sizeof(t_philo));
	if (!philo)
	{
		printf("Error allocating philo struct\n");
		return (NULL);
	}
	while (i < data->num_philos)
	{
		philo[i].id = 0;
		philo[i].die = 0;
		philo[i].eat = 0;
		philo[i].sleep = 0;
		philo[i].last_meal = 0;
		philo[i].status = -1;
		philo[i].goal = 0;
		philo[i].total = 0;
		philo[i].fork_left = -1;
		philo[i].fork_right = -1;
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

int	error_check(int argc)
{
	if (argc < 5)
	{
		printf("Error: Not enough arguments to run the program\n");
		return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_data	data;
	t_philo *philos;

	if (!error_check(argc))
	{
		init_data(&data, argv);
		print_data_struct(&data);		
	}
	philos = NULL;
	philos = init_philo(philos, &data);
	int	i = 0;
	while (i < data.num_philos)
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
		printf("Philosopher [%d] has right fork %d\n", philos[i].id, philos[i].fork_right);
		printf("Philosopher [%d] has left fork %d\n", philos[i].id, philos[i].fork_left);
		i++;
	}
	
	return (0);
}
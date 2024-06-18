/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:33 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/18 17:13:01 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


// typedef struct s_philo
// {
// 	pthread_t				th;
// 	int 					id;        // philo id
// 	// int 					die;       // ms it takes to die
// 	// int 					eat;       // ms it takes to eat
// 	// int 					sleep;     // ms it takes to sleep
// 	int 					last_meal; // time from the last meal of the philo
// 	int 					status;    // indicates what the philo has to do
// 	int 					meals;      // indicates the # of meals the philo has to eat
// 	int 					total;     // total # of philos and forks
// 	int 					*final_goal; // pointer to the shared final_goal var
// 	pthread_mutex_t			*fork_left;
// 	pthread_mutex_t			*fork_right;
// 	t_data					*data;
// }					t_philo;

// typedef struct s_data
// {
// 	int				num_philos;
// 	int				time_to_die;
// 	int				time_to_eat;
// 	int				time_to_sleep;
// 	int				number_of_meals;
// 	int				start_flag;
// 	pthread_mutex_t	mutex_start;
// 	pthread_mutex_t	mutex_dead;
// 	pthread_mutex_t	mutex_meal;
// 	pthread_mutex_t	mutex_print;
// 	pthread_mutex_t	mutex_stop;
// }					t_data;

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
	if (pthread_mutex_init(&philo->data->mutex_stop, NULL) != 0 
	|| pthread_mutex_init(&philo->data->mutex_meal, NULL) != 0
	|| pthread_mutex_init(&philo->data->mutex_start, NULL) != 0)
	{
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
		philo_thread[i].fork_left = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(philo_thread[i].fork_left, NULL) != 0)
			return (1);
		// printf("Philosopher %d initialized with last meal time: %d\n", philo_thread[i].id, philo_thread[i].last_meal);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		philo_thread[i].fork_right = philo_thread[(i - 1 + data->num_philos) % data->num_philos].fork_left;
		printf("Philosopher %d has fork %p to his left and fork %p to his right\n", philo_thread[i].id, philo_thread[i].fork_left, philo_thread[i].fork_right);
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
	// int				i;
	int				start;

	// i = 0;
	start = 0;
	philo = (t_philo *)arg;
	// printf("Philosopher %d started\n", philo->id);
	// printf("Start_flag: %d\n", philo->data->start_flag);
	while (!start)
	{
		pthread_mutex_lock(&philo->data->mutex_start);
		// printf("philo->data->start_flag: %d\n", philo->data->start_flag);
		if (philo->data->start_flag)
		{
			start = 1;
		}
		pthread_mutex_unlock(&philo->data->mutex_start);
		ft_usleep(100);
	}
	printf("Philosopher %d last meal time before update: %d\n", philo->id, philo->last_meal);
	pthread_mutex_lock(&philo->data->mutex_meal);
	philo->last_meal = ft_time() + philo->data->time_to_die;
	printf("Last meal time: %d\n", philo->last_meal);
	pthread_mutex_unlock(&philo->data->mutex_meal);
	printf("Philosopher %d last meal time after update: %d\n", philo->id, philo->last_meal);
exit(1);
	while (1)
	{
		printf("Philosopher %d is thinking\n", philo->id);
		pthread_mutex_lock(philo->fork_left);
		printf("Philosoppher %d is eating\n", philo->id);
		pthread_mutex_unlock(philo->fork_left);
		printf("Philosopher %d is sleeping\n", philo->id);
	}
	return (NULL);
}

void	init_threads(t_philo *philo, t_data *data)
{
	int	i;

	philo->data->time = ft_time();
	pthread_mutex_lock(&data->mutex_start);
	data->start_flag = 1;
	pthread_mutex_unlock(&data->mutex_start);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&philo[i].th, NULL, &routine, &philo[i]) != 0)
			return ;
		printf("Philosopher thread [%d] is created\n", philo[i].id);
		i++;
	}
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
		while(1)
		{
			if(philos->data->dead == 1)
				break;
		}
	}
	printf("Playing: %d\n", philos->total);
	pthread_mutex_destroy(&philos->data->mutex_stop);
	pthread_mutex_destroy(&philos->data->mutex_meal);
	// free(philos->data->mutex_stop);
	return (0);
}

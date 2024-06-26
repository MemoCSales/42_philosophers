/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:43 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/27 14:24:46 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define LIGHT_CYAN "\033[1;36m"
# define YELLOW "\033[1;33m"
# define ORANGE "\033[38;5;214m"
# define DEFAULT "\033[0m"

# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define EATING "is eating"
# define FIRST_FORK "has taken a fork"
# define SECOND_FORK "has taken a fork"
# define DIED "died"

typedef struct s_data	t_data;

typedef struct s_philo
{
	pthread_t			th;
	int					id;
	int					*dead;
	int					last_meal;
	int					meals;
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		*fork_right;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					time;
	int					number_of_meals;
	int					dead;
	int					fed;
	int					num_philos;
	int					start_flag;
	pthread_mutex_t		mutex_dead;
	pthread_mutex_t		mutex_meal;
	pthread_mutex_t		mutex_start;
	pthread_mutex_t		mutex_time;
	t_philo				*philos;
}						t_data;

// eating.c
void					sync_meal_time(t_philo *philo);
int						starting(t_philo *philo);
int						take_fork_left(t_philo *philo, char *msg);
int						take_fork_right(t_philo *philo, char *msg);
int						all_eaten(t_philo *philo);

// init_data.c
t_philo					*init_data(char **argv);
int						mutex_init(t_philo *philo);
int						init_philo(t_data *data);
int						philos_initialization(t_philo *philo_thread,
							t_data *data);
int						handle_1_philo(t_philo *philo);

// main.c
int						philo_died(t_philo *philo);
void					init_threads(t_philo *philo, t_data *data);
int						is_dead(t_philo *philo);
int						is_full(t_philo *philo);

// routine.c
void					*routine(void *arg);
int						eat(t_philo *philo);
int						ft_sleep(t_philo *philo);
int						think(t_philo *philo);

// time.c
int						ft_time(void);
int						ft_usleep(int milisec);
int						get_start_time(t_data *data);

// utils.c
int						ft_atoi(const char *str);
int						error_check(int argc, char **argv);
void					message(t_philo *philo, char *msg);
void					print_philo_struct(t_data *data, t_philo *philos);
int						join_and_destroys(t_philo *philos);

// utils2.c
int						check_initialization(t_philo *philos);
void					check_if_dead_or_full(t_philo *philos);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:47:43 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/14 18:51:19 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define LIGHT_CYAN "\033[1;36m"
# define YELLOW "\033[1;33m"
# define ORANGE "\033[38;5;214m"
# define DEFAULT "\033[0m"

typedef struct s_data t_data;

typedef struct s_philo
{
	pthread_t				th;
	int 					id;        // philo id
	// int 					die;       // ms it takes to die
	// int 					eat;       // ms it takes to eat
	// int 					sleep;     // ms it takes to sleep
	int 					last_meal; // time from the last meal of the philo
	int 					status;    // indicates what the philo has to do
	int 					meals;      // indicates the # of meals the philo has to eat
	int 					total;     // total # of philos and forks
	pthread_mutex_t			fork_right;
	pthread_mutex_t			fork_left;
	int 					*stop;       // pointer to the shared stop variable
	int 					*final_goal; // pointer to the shared final_goal var
	pthread_mutex_t			*fork_mutex;
	t_data					*data;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_meals;
	int				start_flag;
	pthread_mutex_t	mutex_start;
	pthread_mutex_t	mutex_dead;
	pthread_mutex_t	mutex_meal;
	pthread_mutex_t	mutex_print;
}					t_data;


t_philo	*init_philo(t_data *data);

// utils.c
int					ft_atoi(const char *str);

int	error_check(int argc, char **argv);


#endif

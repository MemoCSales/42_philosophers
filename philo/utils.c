/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcruz-sa <mcruz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:18:58 by mcruz-sa          #+#    #+#             */
/*   Updated: 2024/06/18 17:25:26 by mcruz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Converts the initial portion of the string pointed to by str to int.
 *
 * This function interprets an integer value in a byte string pointed to by str.
 * Discards any whitespace characters until the first non-whitespace character is
 * found, then takes as many characters as possible to form a valid integer number
 * representation and converts them to an integer value. The valid integer consists
 * of an optional plus or minus sign followed by at least one digit.
 *
 * @param str A pointer to the byte string to be interpreted.
 * @return The converted integer value from the string.
 */
int	ft_atoi(const char *str)
{
	int	sign;
	int	number;

	sign = 0;
	number = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' && *(str + 1) != '-')
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		number *= 10;
		number += *str - 48;
		str++;
	}
	if (sign % 2)
		return (-number);
	else
		return (number);
}


/**
 * @brief Checks the command line arguments for errors.
 *
 * This function checks the command line arguments for the following conditions:
 * - The number of arguments is either 5 or 6.
 * - The first argument (after the program name) is a non-negative integer.
 * - The second, third, and fourth arguments are integers greater than or equal to 60.
 * - If a fifth argument is provided, it is an integer greater than or equal to 1.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return Returns 0 if no errors are found, 1 otherwise.
 */
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

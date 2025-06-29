/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/06/29 21:59:58 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "philo.h"

int	exit_message(t_philo philo)
{
	write(2, "invalid number of philosophers\n", \
		(philo.nbr_of_philosophers <= 0) * 32);
	write(2, "invalid time to die\n", (philo.time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (philo.time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (philo.time_to_sleep <= 0) * 23);
	write(2, "invalid number of times each philosopher must eat \n", \
		(philo.notepme <= 0) * 52);
	return ((philo.nbr_of_philosophers <= 0) + (philo.time_to_die <= 0) + \
		(philo.time_to_eat <= 0) + (philo.time_to_sleep <= 0) + \
		(philo.notepme <= 0));
}

void	*run_code(void *)
{
	printf("Current thread ID: %lu\n", (unsigned long)pthread_self());
	return (NULL);
}

int	init_philosophers(t_philo philo)
{
	pthread_t	*nof;
	int			ind;

	ind = -1;
	printf("nbr_of_philosophers %d %ld\n", philo.nbr_of_philosophers, sizeof(pthread_t));
	nof = malloc(philo.nbr_of_philosophers * sizeof(pthread_t));
	while (++ind < philo.nbr_of_philosophers)
		pthread_create(nof + ind, NULL, run_code, NULL);
	while (--ind >= 0)
		pthread_join(nof[ind], NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_philo	philo;

	if (ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	philo.nbr_of_philosophers = ft_atoi(av[1]);
	philo.time_to_die = ft_atof(av[2]);
	philo.time_to_eat = ft_atof(av[3]);
	philo.time_to_sleep = ft_atof(av[4]);
	philo.notepme = ft_atoi(av[5]);
	if (exit_message(philo))
		return (1);
	init_philosophers(philo);
}

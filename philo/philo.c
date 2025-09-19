/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:24:44 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_info(int ac, char **av, t_info *info)
{
	info->nbr_of_philo = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]) * KILO;
	info->time_to_eat = ft_atoi(av[3]) * KILO;
	info->time_to_sleep = ft_atoi(av[4]) * KILO;
	info->notepme = -1;
	if (ac > 5)
		info->notepme = ft_atoi(av[5]);
	info->all_alive = 1;
	info->talk_perms = 1;
	return (0);
}

int exit_message(t_info *info, int ac)
{
	write(2, "invalid number of philosophers\n",
		  (info->nbr_of_philo <= 0) * 32);
	write(2, "invalid time to die\n", (info->time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (info->time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (info->time_to_sleep <= 0) * 23);
	if (ac > 5)
		write(2, "invalid number of times each philosopher must eat \n",
			  (info->notepme <= 0) * 52);
	return ((info->nbr_of_philo <= 0) + (info->time_to_die <= 0) +
				(info->time_to_eat <= 0) + (info->time_to_sleep <= 0) +
				(info->notepme <= 0) &&
			ac > 5);
}

void *run_code(void *var)
{
	t_philo *philo;

	philo = (t_philo *)var;
	gettimeofday(&philo->lta, NULL);
	// pthread_mutex_lock(philo->alive_mutex);
	// pthread_mutex_unlock(philo->alive_mutex);
	// printf("%ld exiting\n", total_time() / KILO);
	// return (NULL);
	if (philo->nbr % 2 == 0)
		usleep(20);
	while (all_alive(philo))
	{
		if (go_eat(philo))
			return (NULL);
		// if (philo->notepme != -1 && check_times_ate(philo, philo->notepme))
			// return (NULL);
		if (go_sleep(philo))
			return (NULL);
		if (go_think(philo))
			return (NULL);
	}
	return (NULL);
}

int init_infosophers(t_info *info)
{
	int			ind;
	t_philo		*philo;
	pthread_t	nof[200];

	philo = init_philo_and_mutex(info->nbr_of_philo, info);
	if (philo == NULL)
		return (1);
	ind = -1;
	total_time();
	while (++ind < info->nbr_of_philo)
	{
		if (pthread_create(nof + ind, NULL, run_code, philo))
			return (ft_philoclear(philo), 1);
		philo = philo->right;
	}
	usleep(20);
	hypervise(philo);
	while (ind-- > 0)
		pthread_join(nof[ind], NULL);
	return (ft_philoclear(philo), 0);
}

int main(int ac, char **av)
{
	t_info info;

	if (ac != 5 && ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (init_info(ac, av, &info))
		return (1);
	if (exit_message(&info, ac))
		return (1);
	if (pthread_mutex_init(&info.talk_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&info.alive_mutex, NULL))
		return (pthread_mutex_destroy(&info.talk_mutex), 1);
	init_infosophers(&info);
	pthread_mutex_destroy(&info.talk_mutex);
	pthread_mutex_destroy(&info.alive_mutex);
	return (0);
}

/* 	printf("nbr_of_philo:%d\n", info.nbr_of_philo);
	printf("time_to_die:%d\n", info.time_to_die / KILO);
	printf("time_to_eat:%d\n", info.time_to_eat / KILO);
	printf("time_to_sleep:%d\n", info.time_to_sleep / KILO);
	printf("notepme:%d\n", info.notepme);
	printf("\nstarting now\n"); */
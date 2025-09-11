/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/11 18:25:05 by jlima-so         ###   ########.fr       */
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
	int time_to_die;
	int notepme;

	philo = (t_philo *)var;
	if (philo->info->nbr_of_philo == 1)
	{
		usleep(philo->info->time_to_die);
		return (printf("%ld 1 died\n", total_time() / KILO), NULL);
	}
	pthread_mutex_lock(philo->info_mutex);
	notepme = philo->info->notepme;
	time_to_die = philo->info->time_to_die;
	pthread_mutex_unlock(philo->info_mutex);
	gettimeofday(&philo->lta, NULL);
	while (all_alive(philo))
	{
		if (go_eat(philo, time_to_die))
			return (NULL);
		if (notepme != -1 && check_times_ate(philo, notepme))
			return (NULL);
		if (go_sleep(philo, time_to_die) || go_think(philo, time_to_die))
			return (NULL);
	}
	return (NULL);
}

int init_infosophers(t_info *info, pthread_mutex_t *info_mutex)
{
	int ind;
	t_philo *philo;
	pthread_t *nof;

	nof = malloc((info->nbr_of_philo + 1) * sizeof(pthread_t));
	if (nof == NULL)
		return (1);
	philo = init_philo_and_mutex(info->nbr_of_philo, info, info_mutex);
	if (philo == NULL)
		return (free(nof), 1);
	ind = -1;
	total_time();
	while (++ind < info->nbr_of_philo)
	{
		if (pthread_create(nof + ind, NULL, run_code, philo))
			return (ft_philoclear(philo), free(nof), 1);
		philo = philo->right;
	}
	hypervise(philo, info, info_mutex);
	while (ind-- > 0)
		pthread_join(nof[ind], NULL);
	return (free(nof), ft_philoclear(philo), 0);
}

int main(int ac, char **av)
{
	pthread_mutex_t info_mutex;
	t_info info;

	if (ac != 5 && ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (init_info(ac, av, &info))
		return (1);
	if (exit_message(&info, ac))
		return (1);
	if (pthread_mutex_init(&info_mutex, NULL))
		return (1);
	init_infosophers(&info, &info_mutex);
	pthread_mutex_destroy(&info_mutex);
	return (0);
}

/* 	printf("nbr_of_philo:%d\n", info.nbr_of_philo);
	printf("time_to_die:%d\n", info.time_to_die / KILO);
	printf("time_to_eat:%d\n", info.time_to_eat / KILO);
	printf("time_to_sleep:%d\n", info.time_to_sleep / KILO);
	printf("notepme:%d\n", info.notepme);
	printf("\nstarting now\n"); */
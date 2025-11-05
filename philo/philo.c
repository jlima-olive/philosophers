/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:24:31 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:00:52 by jlima-so         ###   ########.fr       */
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
	info->init = 0;
	info->dead = 0;
	info->end_sim = 0;
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
			(info->notepme <= 0) && ac > 5);
}

void	*run_code(void *var)
{
	t_philo	*philo;

	philo = (t_philo *)var;
	if (philo->nbr_of_philo == 1)
	{
		usleep(philo->time_to_die);
		printf("%d 1 died\n", philo->time_to_die / KILO);
		return (NULL);
	}
	while (philo->init == 0)
		usleep(50);
	pthread_mutex_lock(&philo->gettime);
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->gettime);
	if (philo->nbr % 2 == 0)
		usleep(200);
	while (1)
	{
		if (go_eat(philo) || *philo->end_sim == philo->notepme)
			return (NULL);
		if (go_think(philo) || go_sleep(philo))
			return (NULL);
	}
	return (NULL);
}

int	init_infosophers(t_info *info)
{
	int			ind;
	int			nbr;
	t_philo		*philo;
	t_philo		*walk;
	pthread_t	nof[200];

	philo = init_philo_and_mutex(info);
	if (philo == NULL)
		return (1);
	ind = -1;
	walk = philo;
	nbr = info->nbr_of_philo;
	total_time();
	while (++ind < nbr)
	{
		if (pthread_create(nof + ind, NULL, run_code, walk))
			return (ft_philoclear(walk), 1);
		walk = walk->right;
	}
	if (philo->nbr_of_philo != 1)
		hypervise(philo, info->time_to_die, &info->init, &info->end_sim);
	while (ind-- > 0)
		pthread_join(nof[ind], NULL);
	return (ft_philoclear(philo), 0);
}

int main(int ac, char **av)
{
	t_info 			info;

	if (ac != 5 && ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (init_info(ac, av, &info))
		return (1);
	if (exit_message(&info, ac))
		return (1);
	if (pthread_mutex_init(&info.dead_mutex, NULL))
		return (1);
	init_infosophers(&info);
	pthread_mutex_destroy(&info.dead_mutex);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:56:58 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	go_eat(t_philo *philo)
{
	sem_wait(philo->info);
	gettimeofday(&philo->lta, NULL);
	sem_post(philo->info);
	grab_spoon(philo);
	sem_wait(philo->talk_perms);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d is eating\n", total_time() / KILO, philo->nbr);
	sem_post(philo->talk_perms);
	better_sleep(philo, philo->time_to_eat, 1);
	drop_spoon(philo);
	sem_wait(philo->info);
	gettimeofday(&philo->lta, NULL);
	sem_post(philo->info);
	better_sleep(philo, 10, 0);
	philo->ammout_eaten++;
}

void	go_think(t_philo *philo)
{
	// long time;
// 
	// time = philo->time_to_eat - last_time_ate(philo);
	// if (time > 0)
	// {
		sem_wait(philo->talk_perms);
		printf("%ld %d is thinking\n", total_time() / KILO, philo->nbr);
		// printf("%ld %d is time thinking\n", time, philo->nbr);
		sem_post(philo->talk_perms);
		// better_sleep(philo, time, 0);
	// }
}

void	go_sleep(t_philo *philo)
{
	sem_wait(philo->talk_perms);
	printf("%ld %d is sleeping\n", total_time() / KILO, philo->nbr);
	sem_post(philo->talk_perms);
	// printf("%d %d is time sleeping\n", philo->time_to_sleep, philo->nbr);
	better_sleep(philo, philo->time_to_sleep, 0);
}

void grab_spoon(t_philo *philo)
{
	sem_wait(philo->getting_spoons);
	sem_wait(philo->spoons);
	sem_wait(philo->spoons);
	// printf("%d grabbed spoons\n", philo->nbr);
}

void drop_spoon(t_philo *philo)
{
	sem_post(philo->spoons);
	sem_post(philo->spoons);
	sem_post(philo->getting_spoons);
	// printf("%d dropped spoons\n", philo->nbr);
}

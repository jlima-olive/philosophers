/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/12 17:05:54 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	go_eat(t_philo *philo)
{
	if (last_time_ate(philo) >= philo->time_to_die)
	{
		sem_wait(philo->talk_perms);
		printf("%ld %d he died here\n", last_time_ate(philo), philo->time_to_die);
		printf("%ld %d died\n", total_time() / KILO, philo->nbr);
		exit(sem_post(philo->dead));
	}
	grab_spoon(philo);
	sem_wait(philo->info);
	gettimeofday(&philo->lta, NULL);
	sem_post(philo->info);
	sem_wait(philo->talk_perms);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
	printf("%ld %d is eating\n", total_time() / KILO, philo->nbr);
	sem_post(philo->talk_perms);
	better_sleep(philo, philo->time_to_eat);
	drop_spoon(philo);
	sem_wait(philo->info);
	gettimeofday(&philo->lta, NULL);
	sem_post(philo->info);
	usleep(1);
	philo->ammout_eaten++;
}

void	go_think(t_philo *philo)
{
	long time;

	time = philo->time_to_eat - last_time_ate(philo);
	sem_wait(philo->talk_perms);
	printf("%ld %d is thinking\n", total_time() / KILO, philo->nbr);
	sem_post(philo->talk_perms);
	usleep(time);
}

void	go_sleep(t_philo *philo)
{
	sem_wait(philo->talk_perms);
	printf("%ld %d is sleeping\n", total_time() / KILO, philo->nbr);
	sem_post(philo->talk_perms);
	usleep(philo->time_to_sleep);
}

void grab_spoon(t_philo *philo)
{
	sem_wait(philo->getting_spoons);
	sem_wait(philo->spoons);
	sem_wait(philo->spoons);
}

void drop_spoon(t_philo *philo)
{
	sem_post(philo->spoons);
	sem_post(philo->spoons);
	sem_post(philo->getting_spoons);
}

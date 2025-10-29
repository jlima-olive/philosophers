/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 16:56:42 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	any_dead(t_philo *philo)
{
	int	ret;
	
	pthread_mutex_lock(philo->dead_mutex);
	ret = *philo->dead;
	pthread_mutex_unlock(philo->dead_mutex);
	return (ret);
}

int	go_eat(t_philo *philo)
{
	grab_spoon(philo);
	if (last_time_ate(philo) > philo->time_to_die)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		return (drop_spoon(philo), 1);
	}
	pthread_mutex_lock(&philo->eating_mutex);
	philo->eating = 1;
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->eating_mutex);
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_lock(philo->talk_mutex);
		printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
		printf("%ld %d has taken a fork\n", total_time() / KILO, philo->nbr);
		printf("%ld %d is eating\n", total_time() / KILO, philo->nbr);
		pthread_mutex_unlock(philo->talk_mutex);
	}
	else
		return (drop_spoon(philo), pthread_mutex_unlock(philo->dead_mutex), 1);
	pthread_mutex_lock(&philo->eating_mutex);
	philo->eating = 0;
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->eating_mutex);
	if (better_usleep(philo, philo->time_to_eat))
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		return (drop_spoon(philo), 1);
	}
	drop_spoon(philo);
	philo->times_ate++;
	return (0);
}

int	go_sleep(t_philo *philo)
{
	pthread_mutex_lock(philo->talk_mutex);
	pthread_mutex_lock(philo->dead_mutex);
	if (last_time_ate(philo) > philo->time_to_die || *philo->dead)
	{
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	if (*philo->dead)
		printf("%ld %d is sleeping\n", total_time() / KILO, philo->nbr);
	else
		return (pthread_mutex_unlock(philo->talk_mutex), 1);
	pthread_mutex_unlock(philo->talk_mutex);
	if (better_usleep(philo, philo->time_to_sleep))
		return (1);
	return (0);
}

int go_think(t_philo *philo)
{
	long time;

	time = philo->time_to_eat - last_time_ate(philo);
	pthread_mutex_lock(philo->talk_mutex);
	pthread_mutex_lock(philo->dead_mutex);
	if (last_time_ate(philo) > philo->time_to_die || *philo->dead)
	{
		if (*philo->dead == 0)
			*philo->dead = 0;
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_unlock(philo->talk_mutex);
		return (1);
	}
	if (time > 0 && better_usleep(philo, time))
	{
		printf("%ld %d is thinking\n", total_time() / KILO, philo->nbr);
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_unlock(philo->talk_mutex);
		return (1);
	}
	else
		pthread_mutex_unlock(philo->talk_mutex);
	return (0);
}

void grab_spoon(t_philo *philo)
{
	pthread_mutex_lock(philo->spoon1);
	pthread_mutex_lock(philo->spoon2);
	// printf("%d grabbed the spoons\n", philo->nbr);
}

void drop_spoon(t_philo *philo)
{
	pthread_mutex_unlock(philo->spoon1);
	pthread_mutex_unlock(philo->spoon2);
	// printf("%d dropped the spoons\n", philo->nbr);
}

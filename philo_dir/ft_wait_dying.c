/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait_dying.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:58:08 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/11 18:02:30 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void start_dying(t_philo *philo)
{
	pthread_mutex_lock(philo->info_mutex);
	if (philo->info->all_alive == 0)
	{
		pthread_mutex_unlock(philo->info_mutex);
		return;
	}
	philo->info->all_alive = 0;
	while (philo->info->talk_perms == 0)
	{
		pthread_mutex_unlock(philo->info_mutex);
		pthread_mutex_lock(philo->info_mutex);
	}
	pthread_mutex_unlock(philo->info_mutex);
	printf("%ld %d died\n", total_time() / KILO, philo->nbr);
	return;
}

int wait_to_eat(t_philo *philo, long time_to_die)
{
	pthread_mutex_lock(&philo->eat_mutex);
	while (philo->waiting_to_eat)
	{
		pthread_mutex_unlock(&philo->eat_mutex);
		if (last_time_ate(philo) > time_to_die)
			return (start_dying(philo), 1);
		if (all_alive(philo) == 0)
			return (start_dying(philo), 1);
		pthread_mutex_lock(&philo->eat_mutex);
	}
	philo->waiting_to_eat = 1;
	pthread_mutex_unlock(&philo->eat_mutex);
	return (0);
}

int wait_to_talk(t_philo *philo, long time_to_die)
{
	pthread_mutex_lock(philo->info_mutex);
	while (philo->info->talk_perms == 0)
	{
		pthread_mutex_unlock(philo->info_mutex);
		// usleep(20);
		if (last_time_ate(philo) > time_to_die)
			return (start_dying(philo), 1);
		if (all_alive(philo) == 0)
			return (start_dying(philo), 1);
		pthread_mutex_lock(philo->info_mutex);
	}
	philo->info->talk_perms = 0;
	pthread_mutex_unlock(philo->info_mutex);
	return (0);
}

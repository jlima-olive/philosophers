/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait_dying.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:58:08 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:28:15 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void start_dying(t_philo *philo)
{
	pthread_mutex_lock(philo->alive_mutex);
	if (*philo->all_alive == 0)
	{
		pthread_mutex_unlock(philo->alive_mutex);
		return ;
	}
	*philo->all_alive = 0;
	pthread_mutex_lock(philo->talk_mutex);
	while (*philo->talk_perms == 0)
	{
		pthread_mutex_unlock(philo->talk_mutex);
		pthread_mutex_lock(philo->talk_mutex);
	}	
	*philo->talk_perms = 0;
	pthread_mutex_unlock(philo->talk_mutex);
	printf("%ld %d died\n", total_time() / KILO, philo->nbr);
}

int wait_to_talk(t_philo *philo)
{
	pthread_mutex_lock(philo->talk_mutex);
	while (*philo->talk_perms == 0)
	{
		pthread_mutex_unlock(philo->talk_mutex);
		if (last_time_ate(philo) > philo->time_to_die)
			return (/* start_dying(philo),  */1);
		if (all_alive(philo) == 0)
			return (/* start_dying(philo),  */1);
		pthread_mutex_lock(philo->talk_mutex);
	}
	*philo->talk_perms = 0;
	pthread_mutex_unlock(philo->talk_mutex);
	return (0);
}

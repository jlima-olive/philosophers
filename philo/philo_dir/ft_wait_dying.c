/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait_dying.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:58:08 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 16:28:27 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/* void start_dying(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->any_dead == 0)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return ;
	}
	*philo->any_dead = 0;
	pthread_mutex_lock(philo->talk_mutex);
	while (*philo->talk_perms == 0)
	{
		pthread_mutex_unlock(philo->talk_mutex);
		pthread_mutex_lock(philo->talk_mutex);
	}	
	*philo->talk_perms = 0;
	pthread_mutex_unlock(philo->talk_mutex);
	printf("%ld %d died\n", total_time() / KILO, philo->nbr);
} */
// 
// int wait_to_talk(t_philo *philo)
// {
	// pthread_mutex_lock(philo->talk_mutex);
	// while (*philo->talk_perm == 0 && *philo->dead)
	// {
		// pthread_mutex_unlock(philo->talk_mutex);
		// if (last_time_ate(philo) > philo->time_to_die)
			// return (1);
		// if (*philo->dead == 0)
			// return (1);
		// pthread_mutex_lock(philo->talk_mutex);
	// }
	// pthread_mutex_unlock(philo->talk_mutex);
	// *philo->talk_perm = 0;
	// return (0);
// }
// 
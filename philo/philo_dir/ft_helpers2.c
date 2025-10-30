/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:01:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/30 04:26:12 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int better_usleep(t_philo *philo, long time_to_sleep)
{
	long time;

	time = total_time();
	while (total_time() - time < time_to_sleep - 10)
	{
		usleep(10);
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->dead)
			return (pthread_mutex_unlock(philo->dead_mutex), 1);
		pthread_mutex_unlock(philo->dead_mutex);
		if (last_time_ate(philo) > philo->time_to_die)
			return (1);
	}
	time_to_sleep = time_to_sleep - (total_time() - time);
	if (time_to_sleep > 0)
		usleep(time_to_sleep);
	return (0);
}

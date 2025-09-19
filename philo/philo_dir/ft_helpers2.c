/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:01:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:28:09 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void let_other_talk(t_philo *philo)
{
	pthread_mutex_lock(philo->talk_mutex);
	*philo->talk_perms = 1;
	pthread_mutex_unlock(philo->talk_mutex);
}

int better_usleep(t_philo *philo, long time_to_sleep)
{
	long time;

	time = total_time();
	while (total_time() - time < time_to_sleep - 10)
	{
		usleep(10);
		if (all_alive(philo) == 0)
			return (/* start_dying(philo),  */1);
		if (last_time_ate(philo) > philo->time_to_die)
			return (/* start_dying(philo),  */1);
	}
	time_to_sleep = time_to_sleep - (total_time() - time);
	if (time_to_sleep > 0)
		usleep(time_to_sleep);
	return (0);
}

/* int check_times_ate(t_philo *philo, int value)
{
	int ret;

	while (philo->nbr != 1)
		philo = philo->left;
	philo = philo->left;
	pthread_mutex_lock(&philo->left_eat_mutex);
	pthread_mutex_lock(&philo->eat_mutex);
	ret = (philo->times_ate >= value) && (philo->left->times_ate >= value);
	pthread_mutex_unlock(&philo->left_eat_mutex);
	pthread_mutex_unlock(&philo->eat_mutex);
	return (ret);
} */

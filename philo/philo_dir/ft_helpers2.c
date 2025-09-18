/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42lisba.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:01:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/18 14:11:43 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void let_other_talk(t_philo *philo)
{
	pthread_mutex_lock(philo->info_mutex);
	philo->info->talk_perms = 1;
	pthread_mutex_unlock(philo->info_mutex);
}

int better_usleep(t_philo *philo, long time_to_sleep, int flag)
{
	long time;

	time = total_time();
	while (total_time() - time < time_to_sleep - 10)
	{
		usleep(10);
		if (all_alive(philo) == 0)
			return (1);
		if (flag)
		{
			pthread_mutex_lock(&philo->eat_mutex);
			gettimeofday(&philo->lta, NULL);
			pthread_mutex_unlock(&philo->eat_mutex);
		}
		if (flag == 0 && last_time_ate(philo) > philo->info->time_to_die)
			start_dying(philo);
	}
	time_to_sleep = time_to_sleep - (total_time() - time);
	if (time_to_sleep > 0)
		usleep(time_to_sleep);
	return (0);
}

int check_times_ate(t_philo *philo, int value)
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
}

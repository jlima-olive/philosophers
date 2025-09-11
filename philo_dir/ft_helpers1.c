/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:52:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/11 18:06:35 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long total_time(void)
{
	static int flag;
	static struct timeval start;
	struct timeval curr;
	long ret;

	if (flag == 0)
	{
		gettimeofday(&start, NULL);
		flag++;
	}
	gettimeofday(&curr, NULL);
	ret = MEGA * (curr.tv_sec - start.tv_sec) + curr.tv_usec - start.tv_usec;
	return (ret);
}

long last_time_ate(t_philo *philo)
{
	struct timeval curr;
	long ret;

	gettimeofday(&curr, NULL);
	ret = MEGA * curr.tv_sec + curr.tv_usec;
	ret = ret - (MEGA * philo->lta.tv_sec + philo->lta.tv_usec);
	return (ret);
}

void increment_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_mutex);
	philo->times_ate++;
	pthread_mutex_unlock(&philo->eat_mutex);
}

int all_alive(t_philo *philo)
{
	int ret;

	pthread_mutex_lock(philo->info_mutex);
	ret = *philo->all_alive;
	pthread_mutex_unlock(philo->info_mutex);
	return (ret);
}

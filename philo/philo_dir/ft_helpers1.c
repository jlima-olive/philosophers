/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:52:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:50:10 by namejojo         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->eating_mutex);
	ret = ret - (MEGA * philo->lta.tv_sec + philo->lta.tv_usec);
	pthread_mutex_unlock(&philo->eating_mutex);
	return (ret);
}

int all_alive(t_philo *philo)
{
	int ret;

	pthread_mutex_lock(philo->alive_mutex);
	ret = *philo->all_alive;
	pthread_mutex_unlock(philo->alive_mutex);
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:52:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:39:45 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	total_time(void)
{
	_Atomic static long		flag;
	static struct timeval	start;
	struct timeval			curr;
	long					ret;

	if (++flag == 1)
		gettimeofday(&start, NULL);
	gettimeofday(&curr, NULL);
	ret = MEGA * (curr.tv_sec - start.tv_sec) + curr.tv_usec - start.tv_usec;
	return (ret);
}

long	last_time_ate(t_philo *philo)
{
	struct timeval	curr;
	long			ret;

	if (philo->eating)
		return (0);
	gettimeofday(&curr, NULL);
	ret = MEGA * curr.tv_sec + curr.tv_usec;
	pthread_mutex_lock(&philo->gettime);
	ret = (ret - (MEGA * philo->lta.tv_sec
				+ philo->lta.tv_usec)) * (philo->eating == 0);
	pthread_mutex_unlock(&philo->gettime);
	return (ret);
}

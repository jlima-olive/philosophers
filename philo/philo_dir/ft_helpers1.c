/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:52:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 19:11:56 by jlima-so         ###   ########.fr       */
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
		flag = 1;
	}
	gettimeofday(&curr, NULL);
	ret = MEGA * (curr.tv_sec - start.tv_sec) + curr.tv_usec - start.tv_usec;
	return (ret);
}

long last_time_ate(t_philo *philo)
{
	struct timeval	curr;
	long			ret;

	gettimeofday(&curr, NULL);
	ret = MEGA * curr.tv_sec + curr.tv_usec;
	pthread_mutex_lock(&philo->eating_mutex);
	ret = (ret - (MEGA * philo->lta.tv_sec + philo->lta.tv_usec)) * (philo->eating == 0);
	pthread_mutex_unlock(&philo->eating_mutex);
	// printf("ret is %ld %d %d\n", ret, philo->time_to_die, philo->nbr);
	return (ret);
}
/* 
int any_dead(t_philo *philo)
{
	int ret;

	pthread_mutex_lock(philo->dead_mutex);
	ret = *philo->any_dead;
	pthread_mutex_unlock(philo->dead_mutex);
	return (ret);
}
 */
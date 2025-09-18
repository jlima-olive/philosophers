/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42lisba.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:52:36 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/18 12:48:45 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

long	total_time(void)
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

long	last_time_ate(t_philo *philo)
{
	struct timeval curr;
	long ret;

	sem_wait(philo->info);
	gettimeofday(&curr, NULL);
	sem_post(philo->info);
	ret = MEGA * curr.tv_sec + curr.tv_usec;
	sem_wait(philo->info);
	ret = ret - (MEGA * philo->lta.tv_sec + philo->lta.tv_usec);
	sem_post(philo->info);
	return (ret);
}

int better_sleep(long time_to_sleep)
{
	long time;

	time = total_time();
	while (total_time() - time < time_to_sleep - 10)
		better_sleep(10);
	time_to_sleep = time_to_sleep - (total_time() - time);
	if (time_to_sleep > 0)
		better_sleep(time_to_sleep);
	return (0);
}

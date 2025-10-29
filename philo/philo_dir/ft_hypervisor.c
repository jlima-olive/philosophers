/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 02:51:06 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	hypervise(t_philo *philo, long ttd)
{
	int	*alive;

	if (philo->nbr_of_philo == 1)
		return ;
	alive = philo->alive;
	ttd = philo->time_to_die;
	while (*philo->init == 0);
	usleep(1000);	
	while (1)
	{
		if (last_time_ate(philo) > ttd)
		{
			printf("philo%d\n\t\t\t%ld > %ld\n",philo->nbr, last_time_ate(philo), ttd);
			pthread_mutex_unlock(&philo->eating_mutex);
			pthread_mutex_lock(philo->alive_mutex);
			*alive = 0;
			pthread_mutex_unlock(philo->alive_mutex);
			pthread_mutex_lock(philo->talk_mutex);
			printf("%ld %d died\n", total_time() / KILO, philo->nbr);
			pthread_mutex_unlock(philo->talk_mutex);
			return ;
		}
		philo = philo->right;
	}
}

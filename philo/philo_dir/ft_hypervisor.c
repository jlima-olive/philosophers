/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 21:37:53 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	hypervise(t_philo *philo, long ttd)
{
	int	print;

	ttd = philo->time_to_die;
	while (*philo->init == 0);
	usleep(1000);	
	while (1)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (last_time_ate(philo) > ttd)
		{
			*philo->dead = philo->nbr;
			print = *philo->dead;
			pthread_mutex_lock(philo->talk_mutex);
			pthread_mutex_unlock(philo->dead_mutex);
			ft_putnbr_fd(total_time() / KILO, 1);
			write(1, " ", 1);
			ft_putnbr_fd(print, 1);
			ft_putstr_fd(" is dead\n", 1);
			pthread_mutex_unlock(philo->talk_mutex);
			return ;
		}
		pthread_mutex_unlock(philo->dead_mutex);
		if (philo->nbr == philo->nbr_of_philo)
		philo = philo->right;
	}
}

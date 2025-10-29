/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 19:44:14 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	any_dead(t_philo *philo)
{
	int	ret;
	
	pthread_mutex_lock(philo->dead_mutex);
	ret = *philo->dead;
	pthread_mutex_unlock(philo->dead_mutex);
	return (ret);
}

int	go_eat(t_philo *philo)
{
	if (grab_spoon(philo))
		return (1);
	if (last_time_ate(philo) > philo->time_to_die)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		return (drop_spoon(philo), 1);
	}
	pthread_mutex_lock(&philo->eating_mutex);
	philo->eating = 1;
	pthread_mutex_unlock(&philo->eating_mutex);
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_lock(philo->talk_mutex);
		ft_putnbr_fd(total_time() / KILO, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philo->nbr, 1);
		ft_putstr_fd(" is eating\n", 1);
		pthread_mutex_unlock(philo->talk_mutex);
	}
	else
		return (drop_spoon(philo), pthread_mutex_unlock(philo->dead_mutex), 1);
	if (better_usleep(philo, philo->time_to_eat))
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_lock(&philo->eating_mutex);
		philo->eating = 0;
		pthread_mutex_unlock(&philo->eating_mutex);
		return (drop_spoon(philo), 1);
	}
	pthread_mutex_lock(&philo->eating_mutex);
	philo->eating = 0;
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->eating_mutex);
	// printf("\t\t\tlta %ld\n", last_time_ate(philo));
	drop_spoon(philo);
	philo->times_ate++;
	return (0);
}

int	go_sleep(t_philo *philo)
{
	if (last_time_ate(philo) > philo->time_to_die)
		return (1);
	// {
		// pthread_mutex_lock(philo->dead_mutex);
		// if (*philo->dead == 0)
			// *philo->dead = philo->nbr;
		// pthread_mutex_unlock(philo->dead_mutex);
		// return (1);
	// }
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		pthread_mutex_lock(philo->talk_mutex);
		ft_putnbr_fd(total_time() / KILO, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philo->nbr, 1);
		ft_putstr_fd(" is sleeping\n", 1);
		pthread_mutex_unlock(philo->talk_mutex);
		pthread_mutex_unlock(philo->dead_mutex);
	}
	else
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	if (better_usleep(philo, philo->time_to_sleep))
		return (1);
	return (0);
}

int go_think(t_philo *philo)
{
	long time;

	time = philo->time_to_eat - last_time_ate(philo);
	if (last_time_ate(philo) > philo->time_to_die)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	pthread_mutex_lock(philo->dead_mutex);
	if (time > 0 && *philo->dead == 0)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_lock(philo->talk_mutex);
		ft_putnbr_fd(total_time() / KILO, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philo->nbr, 1);
		ft_putstr_fd(" is thinking\n", 2);
		pthread_mutex_unlock(philo->talk_mutex);
	}
	else
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	if (better_usleep(philo, philo->time_to_sleep))
		return (1);
	return (0);
}

int	grab_spoon(t_philo *philo)
{
	pthread_mutex_lock(philo->spoon1);
	pthread_mutex_lock(philo->spoon2);
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead || last_time_ate(philo) >= philo->time_to_die)
	{
		if (*philo->dead == 0)
			*philo->dead = philo->nbr;
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_unlock(philo->spoon1);
		pthread_mutex_unlock(philo->spoon2);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	pthread_mutex_lock(philo->talk_mutex);
	ft_putnbr_fd(total_time() / KILO, 1);
	write(1, " ", 1);
	ft_putnbr_fd(philo->nbr, 1);
	ft_putstr_fd(" has taken a fork 1\n", 1);
	ft_putnbr_fd(total_time() / KILO, 1);
	write(1, " ", 1);
	ft_putnbr_fd(philo->nbr, 1);
	ft_putstr_fd(" has taken a fork 1\n", 1);
	pthread_mutex_unlock(philo->talk_mutex);
	return (0);
}

void drop_spoon(t_philo *philo)
{
	pthread_mutex_unlock(philo->spoon2);
	pthread_mutex_unlock(philo->spoon1);
	// printf("%ld %d has droped a fork\n", total_time() / KILO, philo->nbr);
}

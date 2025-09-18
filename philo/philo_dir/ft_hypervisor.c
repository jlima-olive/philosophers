/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42lisba.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/18 14:33:02 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/* static void allow_to_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_eat_mutex);
	pthread_mutex_lock(&philo->eat_mutex);
	while (all_alive(philo) && (*philo->left_spoon || philo->spoon))
	{
		pthread_mutex_unlock(&philo->left_eat_mutex);
		pthread_mutex_unlock(&philo->eat_mutex);
		usleep(1);
		pthread_mutex_lock(&philo->left_eat_mutex);
		pthread_mutex_lock(&philo->eat_mutex);
	}
	pthread_mutex_unlock(&philo->left_eat_mutex);
	pthread_mutex_unlock(&philo->eat_mutex);
	pthread_mutex_lock(&philo->eat_mutex);
	philo->waiting_to_eat = 0;
	pthread_mutex_unlock(&philo->eat_mutex);
}

static void allow_one_to_eat(t_philo *philo, int flag, int group)
{
	if (flag)
		pthread_mutex_lock(&philo->left->left_eat_mutex);
	pthread_mutex_lock(&philo->left_eat_mutex);
	pthread_mutex_lock(&philo->eat_mutex);
	group = (*philo->left_spoon || (*philo->left->left_spoon && flag));
	while (all_alive(philo) == 1 && (group || philo->spoon))
	{
		if (flag)
			pthread_mutex_unlock(&philo->left->left_eat_mutex);
		pthread_mutex_unlock(&philo->left_eat_mutex);
		pthread_mutex_unlock(&philo->eat_mutex);
		usleep(1);
		if (flag)
			pthread_mutex_lock(&philo->left->left_eat_mutex);
		pthread_mutex_lock(&philo->left_eat_mutex);
		pthread_mutex_lock(&philo->eat_mutex);
		group = (*philo->left_spoon || (*philo->left->left_spoon && flag));
	}
	if (flag)
		pthread_mutex_unlock(&philo->left->left_eat_mutex);
	pthread_mutex_unlock(&philo->left_eat_mutex);
	philo->waiting_to_eat = 0;
	pthread_mutex_unlock(&philo->eat_mutex);
}

static void eat_perm(t_philo *philo, const t_info *info, int flag)
{
	static int last;

	if (philo->nbr == 1)
	{
		allow_one_to_eat(philo, flag, 0);
		philo->spoon = 1;
		philo = philo->right->right;
		while (all_alive(philo) == 1 && philo->nbr < info->nbr_of_philo && philo->nbr != 1)
		{
			allow_to_eat(philo);
			philo = philo->right->right;
		}
	}
	else
	{
		while (all_alive(philo) == 1 && philo->nbr <= info->nbr_of_philo - 1 - flag)
		{
			allow_to_eat(philo);
			philo = philo->right->right;
		}
		if (last && flag)
			philo = philo->right;
		allow_to_eat(philo);
		last = !last;
	}
} */

void *hypervise(t_philo *philo, t_info *info, pthread_mutex_t *info_mutex)
{
	int	ttd;

	pthread_mutex_lock(philo->info_mutex);
	ttd = info->time_to_die;
	pthread_mutex_unlock(info_mutex);
	pthread_mutex_lock(&philo->left->eat_mutex);
	while (philo->left->not_here)
	{
		pthread_mutex_unlock(&philo->left->eat_mutex);
		usleep(1);	
		pthread_mutex_lock(&philo->left->eat_mutex);
	}
	pthread_mutex_unlock(&philo->left->eat_mutex);
	while (1)
	{
		if (last_time_ate(philo) > ttd)
		{
			wait_to_talk(philo, __LONG_MAX__);
			pthread_mutex_lock(philo->info_mutex);
			ttd = *philo->all_alive = 0;
			printf("%ld %d died\n", total_time() / KILO, philo->nbr);
			pthread_mutex_unlock(info_mutex);
			let_other_talk(philo);
			return (NULL);
		}
		philo = philo->right;
	}
	return (NULL);
}

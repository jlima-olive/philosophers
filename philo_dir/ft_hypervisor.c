/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hypervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:57:02 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/11 18:01:31 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void allow_to_eat(t_philo *philo)
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
}

void *hypervise(t_philo *philo, t_info *info, pthread_mutex_t *info_mutex)
{
	int flag;

	flag = philo->left->nbr;
	if (flag == 1)
		return (NULL);
	flag = flag % 2;
	pthread_mutex_lock(philo->info_mutex);
	while (info->all_alive)
	{
		pthread_mutex_unlock(info_mutex);
		eat_perm(philo, info, flag);
		usleep(1);
		eat_perm(philo->right, info, flag);
		usleep(1);
		pthread_mutex_lock(info_mutex);
	}
	pthread_mutex_unlock(info_mutex);
	return (NULL);
}

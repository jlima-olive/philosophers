/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:40:42 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:41:02 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo	*ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info)
{
	t_philo	*ph;

	ph = malloc(sizeof(t_philo));
	if (ph == NULL)
		return (NULL);
	if (pthread_mutex_init(&ph->holder_spoon1, NULL))
		return (free(ph), NULL);
	if (pthread_mutex_init(&ph->gettime, NULL))
		return (pthread_mutex_destroy(&ph->holder_spoon1), free(ph), NULL);
	ph->spoon1 = &ph->holder_spoon1;
	ph->right = right;
	ph->left = left;
	ph->nbr = nbr;
	ph->nbr_of_philo = info->nbr_of_philo;
	ph->time_to_die = info->time_to_die;
	ph->time_to_eat = info->time_to_eat;
	ph->time_to_sleep = info->time_to_sleep;
	ph->notepme = info->notepme;
	ph->dead_mutex = &info->dead_mutex;
	ph->end_sim = &info->end_sim;
	ph->dead = &info->dead;
	ph->init = &info->init;
	ph->eating = 0;
	return (ph);
}

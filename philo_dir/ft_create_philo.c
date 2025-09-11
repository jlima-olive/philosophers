/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:57:41 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/11 18:04:13 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo *ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info)
{
	t_philo *new;

	new = malloc(sizeof(t_philo));
	if (new == NULL)
		return (NULL);
	if (pthread_mutex_init(&new->eat_mutex, NULL))
		return (free(new), NULL);
	new->nbr = nbr;
	new->right = right;
	new->left = left;
	new->info = info;
	new->alive = 1;
	new->spoon = 0;
	new->times_ate = 0;
	new->waiting_to_eat = 1;
	new->talk_perms = &info->talk_perms;
	new->all_alive = &info->all_alive;
	return (new);
}

void ft_philoclear(t_philo *philo)
{
	if (philo == NULL)
		return;
	if (philo->nbr != philo->info->nbr_of_philo)
		ft_philoclear(philo->right);
	pthread_mutex_destroy(&philo->eat_mutex);
	free(philo);
}

void last_cicle(t_philo *philo)
{
	philo->left_eat_mutex = philo->left->eat_mutex;
	philo->left_spoon = &philo->left->spoon;
	if (philo->nbr != philo->info->nbr_of_philo)
		last_cicle(philo->left);
}

t_philo *init_philo_and_mutex(int nbr, t_info *info, pthread_mutex_t *mutex)
{
	t_philo *philo;
	t_philo *head;
	int ind;

	philo = ft_philonew(NULL, NULL, 1, info);
	philo->info_mutex = mutex;
	if (philo == NULL)
		return (NULL);
	head = philo;
	ind = 1;
	while (++ind < nbr)
	{
		philo->right = ft_philonew(philo, NULL, ind, info);
		if (philo->right == NULL)
			return (ft_philoclear(head), NULL);
		philo->right->info_mutex = mutex;
		philo = philo->right;
	}
	philo->right = ft_philonew(philo, head, nbr, info);
	if (philo->right == NULL)
		return (ft_philoclear(head), NULL);
	philo->right->info_mutex = mutex;
	head->left = philo->right;
	last_cicle(philo);
	return (head);
}

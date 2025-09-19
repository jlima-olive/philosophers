/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namejojo <namejojo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:57:41 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/19 02:20:16 by namejojo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo *ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info)
{
	t_philo *new;

	new = malloc(sizeof(t_philo));
	if (new == NULL)
		return (NULL);
	new->spoon1 = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(new->spoon1, NULL))
		return (free(new), NULL);
	new->nbr = nbr;
	new->right = right;
	new->left = left;
	new->eating = 0;
	new->time_to_sleep = info->time_to_sleep;
	new->nbr_of_philo = info->nbr_of_philo;
	new->time_to_eat = info->time_to_eat;
	new->time_to_die = info->time_to_die;
	new->talk_perms = &info->talk_perms;
	new->talk_mutex = &info->talk_mutex;
	new->all_alive = &info->all_alive;
	new->alive_mutex = &info->alive_mutex;
	new->notepme = info->notepme;
	return (new);
}

void ft_philoclear(t_philo *philo)
{
	if (philo == NULL)
		return;
	if (philo->nbr != philo->nbr_of_philo)
		ft_philoclear(philo->right);
	if (philo->nbr % 2)
		pthread_mutex_destroy(philo->spoon2);
	else
		pthread_mutex_destroy(philo->spoon1);
	free(philo);
}

void	last_cicle(t_philo *philo)
{
	philo->spoon2 = philo->left->spoon1;
	if (philo->nbr != philo->nbr_of_philo)
		last_cicle(philo->left);
}

void	ft_ult_swap(pthread_mutex_t *p1, pthread_mutex_t *p2)
{
	pthread_mutex_t	temp;

	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

void	reverse_forks(t_philo *philo)
{
	if (philo->nbr % 2)
		ft_ult_swap(philo->spoon1, philo->spoon2);
	if (philo->nbr != philo->nbr_of_philo)
		last_cicle(philo->left);
}
t_philo *init_philo_and_mutex(int nbr, t_info *info)
{
	t_philo *philo;
	t_philo *head;
	int ind;

	philo = ft_philonew(NULL, NULL, 1, info);
	if (philo == NULL)
		return (NULL);
	head = philo;
	ind = 1;
	while (++ind < nbr)
	{
		philo->right = ft_philonew(philo, NULL, ind, info);
		if (philo->right == NULL)
			return (ft_philoclear(head), NULL);
		philo = philo->right;
	}
	philo->right = ft_philonew(philo, head, nbr, info);
	if (philo->right == NULL)
		return (ft_philoclear(head), NULL);
	head->left = philo->right;
	last_cicle(philo);
	reverse_forks(philo);
	return (head);
}

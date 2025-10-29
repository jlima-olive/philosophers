/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:57:41 by jlima-so          #+#    #+#             */
/*   Updated: 2025/10/29 18:56:00 by jlima-so         ###   ########.fr       */
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
	if (pthread_mutex_init(&new->eating_mutex, NULL))
		return (pthread_mutex_destroy(new->spoon1), free(new), NULL);
	new->right = right;
	new->left = left;
	new->nbr = nbr;
	new->nbr_of_philo = info->nbr_of_philo;
	new->time_to_die = info->time_to_die;
	new->time_to_eat = info->time_to_eat;
	new->time_to_sleep = info->time_to_sleep;
	new->notepme = info->notepme;
	new->dead = &info->dead;
	new->talk = &info->talk;
	new->dead_mutex = &info->dead_mutex;
	new->talk_mutex = &info->talk_mutex;
	new->eating = 0;
	new->init = &info->init;
	return (new);
}

void ft_philoclear(t_philo *philo)
{
	if (philo == NULL)
		return;
	if (philo->nbr_of_philo == 1)
	{
		pthread_mutex_destroy(philo->spoon1);
		free(philo);
		return ;
	}
	if (philo->nbr != philo->nbr_of_philo)
		ft_philoclear(philo->right);
	if (philo->nbr % 2)
		pthread_mutex_destroy(philo->spoon2);
	else
		pthread_mutex_destroy(philo->spoon1);
	pthread_mutex_destroy(&philo->eating_mutex);
	free(philo);
}

void	last_cicle(t_philo *philo)
{
	philo->times_ate = 0;
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
t_philo *init_philo_and_mutex(t_info *info)
{
	t_philo *philo;
	t_philo *head;
	int ind;

	philo = ft_philonew(NULL, NULL, 1, info);
	if (philo == NULL)
		return (NULL);
	head = philo;
	ind = 1;
	while (++ind < info->nbr_of_philo)
	{
		philo->right = ft_philonew(philo, NULL, philo->nbr + 1, info);
		if (philo->right == NULL)
			return (ft_philoclear(head), NULL);
		philo = philo->right;
	}
	philo->right = ft_philonew(philo, head, info->nbr_of_philo, info);
	if (philo->right == NULL)
		return (ft_philoclear(head), NULL);
	head->left = philo->right;
	if (philo->nbr_of_philo != 1)
		last_cicle(philo);
	if (philo->nbr_of_philo != 1)
		reverse_forks(philo);
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:57:41 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/04 14:44:35 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo *ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info)
{
	t_philo *ph;

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
	ph->dead = &info->dead;
	ph->dead_mutex = &info->dead_mutex;
	ph->eating = 0;
	ph->init = &info->init;
	ph->swi = 0;
	// printf("number %d\n", ph->nbr);
	return (ph);
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
	// printf("clear %d\n", philo->nbr);
	free(philo);
}

void	last_cicle(t_philo *philo)
{
	philo->times_ate = 0;
	philo->spoon2 = philo->left->spoon1;
	// printf("cicle %d\n", philo->nbr);
	if (philo->nbr != philo->nbr_of_philo)
		last_cicle(philo->left);
}

static void	ft_ult_swap(pthread_mutex_t **p1, pthread_mutex_t **p2)
{
	pthread_mutex_t	*temp;

	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

void	reverse_forks(t_philo *philo)
{
	if (philo->nbr % 2)
		ft_ult_swap(&philo->spoon1, &philo->spoon2);
	// printf("swap %d\n", philo->nbr);
	if (philo->nbr != philo->nbr_of_philo)
		reverse_forks(philo->left);
}

t_philo *init_philo_and_mutex(t_info *info)
{
	t_philo	*philo;
	t_philo	*head;
	int		ind;

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

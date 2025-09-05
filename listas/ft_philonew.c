/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philonew.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:57:41 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/05 18:51:12 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo	*ft_philonew(t_philo *left, t_philo *right, int nbr, t_info *info)
{
	t_philo		*new;

	new = malloc(sizeof(t_philo));
	if (new == NULL)
		return (NULL);
	if (pthread_mutex_init(&new->eat_mutex, NULL))
		return (free(new), NULL);
	new->nbr = nbr;
	new->right = right;
	new->left = left;
	new->info = info;
	return (new);
}

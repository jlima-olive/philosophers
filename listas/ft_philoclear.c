/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philoclear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:21:27 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/09 16:11:18 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	ft_philoclear(t_philo *philo)
{
	if (philo == NULL)
		return ;
	if (philo->nbr != philo->info->nbr_of_philo)
		ft_philoclear(philo->right);
	pthread_mutex_destroy(&philo->eat_mutex);
	free (philo);
}

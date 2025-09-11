/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:17:12 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/11 15:53:43 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"



int	main(int ac, char **av)
{
	pthread_mutex_t	info_mutex;
	t_info			info;

	if (ac != 5 && ac != 6)
		return (write(2, "invalid number of arguments\n", 28));
	if (init_info(ac, av, &info))
		return (1);
	if (exit_message(&info, ac))
		return (1);
	if (pthread_mutex_init(&info_mutex, NULL))
		return (1);
	init_infosophers(&info, &info_mutex);
	pthread_mutex_destroy(&info_mutex);
	return (0);
}
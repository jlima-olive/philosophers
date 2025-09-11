/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:18:55 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/10 19:19:07 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	init_info(int ac, char **av, t_info *info)
{
	info->nbr_of_philo = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]) * KILO;
	info->time_to_eat = ft_atoi(av[3]) * KILO;
	info->time_to_sleep = ft_atoi(av[4]) * KILO;
	info->notepme = -1;
	if (ac > 5)
		info->notepme = ft_atoi(av[5]);
	info->all_alive = 1;
	info->talk_perms = 1;
	return (0);
}

int	exit_message(t_info *info, int ac)
{
	write(2, "invalid number of philosophers\n",
		(info->nbr_of_philo <= 0) * 32);
	write(2, "invalid time to die\n", (info->time_to_die <= 0) * 21);
	write(2, "invalid time to eat\n", (info->time_to_eat <= 0) * 21);
	write(2, "invalid time to sleep\n", (info->time_to_sleep <= 0) * 23);
	if (ac > 5)
		write(2, "invalid number of times each philosopher must eat \n",
			(info->notepme <= 0) * 52);
	return ((info->nbr_of_philo <= 0) + (info->time_to_die <= 0) + \
			(info->time_to_eat <= 0) + (info->time_to_sleep <= 0) + \
				(info->notepme <= 0) && ac > 5);
}

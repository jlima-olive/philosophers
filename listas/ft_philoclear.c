/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philoclear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:21:27 by jlima-so          #+#    #+#             */
/*   Updated: 2025/09/05 18:46:20 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	ft_philoclear(t_philo **lst, int nbr)
{
	t_philo	*right_node;

	if (lst == NULL || *lst == NULL)
		return ;
	while ((*lst)->right != NULL && --nbr > 0)
	{
		right_node = (*lst)->right;
		free (*lst);
		*lst = right_node;
	}
	free (*lst);
	*lst = NULL;
}

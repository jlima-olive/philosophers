/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_syscal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:27:05 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:36:48 by jlima-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_get_nbr_len(int nbr)
{
	int	len;

	len = (nbr == 0);
	while (nbr != 0)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

int	elevate(int len)
{
	int	nbr;

	nbr = 1;
	while (len--)
		nbr *= 10;
	return (nbr);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	ind;

	ind = 0;
	while (*src)
	{
		dest[ind] = *src;
		src++;
		ind++;
	}
	return (dest);
}

void	ft_cpynbr(char *str, int nbr, int len)
{
	while (len)
	{
		*str = (nbr / elevate(--len)) % 10 + 48;
		str++;
	}
}

void	use_single_syscal(int time, int nbr, char *msg)
{
	char	str[64];
	int		len1;
	int		len2;

	len1 = ft_get_nbr_len(time);
	ft_cpynbr(str, time, len1);
	*(str + len1) = ' ';
	len2 = ft_get_nbr_len(nbr);
	ft_cpynbr(str + len1 + 1, nbr, len2);
	ft_strcpy(str + len1 + 1 + len2, msg);
	*(str + len1 + 1 + len2 + ft_strlen(msg)) = '\0';
	ft_putstr_fd(str, 1);
}

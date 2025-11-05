/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_action_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlima-so <jlima-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:59:13 by jlima-so          #+#    #+#             */
/*   Updated: 2025/11/05 13:11:09 by jlima-so         ###   ########.fr       */
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

int	ft_strlen(char *str)
{
	int	ind;

	ind = 0;
	while (str[ind])
		ind++;
	return (ind);
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

int	any_dead(t_philo *philo)
{
	int	ret;
	
	pthread_mutex_lock(philo->dead_mutex);
	ret = *philo->dead;
	pthread_mutex_unlock(philo->dead_mutex);
	return (ret);
}

int	go_eat(t_philo *philo)
{
	if (grab_spoon(philo))
		return (1);
	philo->eating = 1;
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		use_single_syscal(total_time() / KILO, philo->nbr, " is eating\n");
		pthread_mutex_unlock(philo->dead_mutex);
	}
	else
		return (drop_spoon(philo), pthread_mutex_unlock(philo->dead_mutex), 1);
	if (better_usleep(philo, philo->time_to_eat))
		return (drop_spoon(philo), 1);
	drop_spoon(philo);
	pthread_mutex_lock(&philo->gettime);
	gettimeofday(&philo->lta, NULL);
	pthread_mutex_unlock(&philo->gettime);
	philo->eating = 0;
	philo->times_ate++;
	if (philo->times_ate == philo->notepme)
		(*philo->end_sim)++;
	return (0);
}

int	go_sleep(t_philo *philo)
{
	// if (philo->nbr % 2)
		// usleep(100);
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		use_single_syscal(total_time() / KILO, philo->nbr, " is sleeping\n");
		pthread_mutex_unlock(philo->dead_mutex);
	}
	else
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	if (better_usleep(philo, philo->time_to_sleep))
		return (1);
	return (0);
}

int go_think(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 0)
	{
		use_single_syscal(total_time() / KILO, philo->nbr, " is thinking\n");
		pthread_mutex_unlock(philo->dead_mutex);
	}
	else
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	// if (better_usleep(philo, KILO))
		// return (1);
	return (0);
}

int	grab_spoon(t_philo *philo)
{
	pthread_mutex_lock(philo->spoon2);
	pthread_mutex_lock(philo->spoon1);
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_unlock(philo->spoon1);
		pthread_mutex_unlock(philo->spoon2);
		return (1);
	}
	use_single_syscal(total_time() / KILO, philo->nbr, " has taken a fork\n");
	use_single_syscal(total_time() / KILO, philo->nbr, " has taken a fork\n");
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

void drop_spoon(t_philo *philo)
{
	pthread_mutex_unlock(philo->spoon1);
	pthread_mutex_unlock(philo->spoon2);
}

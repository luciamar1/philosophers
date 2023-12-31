/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucia-ma <lucia-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 13:44:46 by lucia-ma          #+#    #+#             */
/*   Updated: 2023/11/25 00:41:43 by lucia-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_dictionary	create_dict_int(int id)
{
	t_dictionary	dictionary;

	dictionary.id = id;
	dictionary.fork = 0;
	return (dictionary);
}

int	is_impar(int num)
{
	if (num % 2)
		return (1);
	else
		return (0);
}

int	ft_arr_isdigit(char *arr)
{
	while (*arr)
	{
		if (*arr < '0' || *arr > '9')
			return (0);
		else
			arr ++;
	}
	return (1);
}

int	ft_isalpha(int c)
{
	if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z'))
		return (0);
	return (1);
}

int	ft_atoi_chetao(const char *str, int *error)
{
	int				c;
	unsigned int	x;

	c = 1;
	x = 0;
	while (*str)
	{
		while ((*str >= 9 && *str <= 13) || *str == ' ')
			str++;
		if (ft_isalpha(*str))
			return (*error = 1, 0);
		if (*str == '+' || *str == '-' || ft_isalpha(*str))
			if (*str++ == '-')
				c *= -1;
		while (*str <= '9' && *str >= '0')
			x = x * 10 + (*str++ - '0');
		if (x > 0x7fffffff && c == 1)
			return (*error = 1, 0);
		if (x > 0x80000000 && c == -1)
			return (*error = 1, 0);
		return (c * x);
		str++;
	}
	*error = 1;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucia-ma <lucia-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 19:23:31 by lucia-ma          #+#    #+#             */
/*   Updated: 2023/12/13 14:03:13 by lucia-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_timeval_ms(struct timeval *time)
{
	return (time->tv_sec * 1000 + time->tv_usec / 1000);
}

uint64_t	get_timeval_us(struct timeval *time)
{
	return (time->tv_sec * 1000000 + time->tv_usec);
}

int	ft_usleep(long long useconds, t_2link_circ_list *philo)
{
	struct timeval	start;
	uint64_t		time;
	uint64_t		end_us;

	(void) philo;
	gettimeofday(&start, NULL);
	time = get_timeval_us(&start);
	end_us = time + useconds;
	while (time < end_us)
	{
		gettimeofday(&start, NULL);
		time = get_timeval_us(&start);
		usleep(100);
	}
	return (0);
}

void	printf_fork_2link_circ_list(t_2link_circ_list *list)
{
	t_2link_circ_list	*head;

	head = list;
	printf("\n\n\n");
	printf("id_fork list == %d\n", list->id_fork.fork);
	list = list->next;
	while (list != head)
	{
		if (list->id_fork.fork)
			printf("id_fork list == %d\n", list->id_fork.fork);
		list = list->next;
	}
}

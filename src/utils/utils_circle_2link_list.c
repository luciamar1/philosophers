/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_circle_2link_list.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucia-ma <lucia-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:47:35 by lucia-ma          #+#    #+#             */
/*   Updated: 2023/10/25 16:51:53 by lucia-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_2link_circlist(t_2link_circ_list **head, t_dictionary id_fork, t_philo_routine routine)
{
	t_2link_circ_list	*new;

	new = malloc(sizeof(t_2link_circ_list));
	if (!new)
	{
		if (*head)
			clear_2link_circ_list(head);
		return (1);
	}
	new->id_fork = id_fork;
	new->routine = routine;
	if (mutex_init(new))
	{
		if (*head)
			clear_2link_circ_list(head);
		return(1);
	}
	
	if (!*head)
	{
		new->prev = new;
		new->next = new;
	}
	else
	{
		new->next = *head;
		new->prev = (*head)->prev;
		(*head)->prev = new;
		new->prev->next = new;
	}
	*head = new->next ;
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
		// printf("que?\n");
		if(list->id_fork.fork)
			printf("id_fork list == %d\n", list->id_fork.fork);
		// printf("aaaaaaa\n");
		// printf("aaaaa%d\n", list->id_fork != head->id_fork);
		// printf("next == %p\n", list->next);
		list = list->next;
	}
}

int	len_dlist(t_2link_circ_list *stack)
{
	int		len;
	t_2link_circ_list	*init;

	if (!stack)
		return (0);
	len = 1;
	init = stack;
	stack = stack->next;
	while (stack != init)
	{
		len ++;
		stack = stack->next;
	}
	return (len);
}

void	printf_dlist_ind(t_2link_circ_list *list)
{
	t_2link_circ_list	*start;
	if (!list)
	{
		//se mete
		printf("se mete\n");
		return ;
	}
	start = list;
	printf("ind list == %d\n", (int) list->id_fork.id);
	if (list->next)
		list = list->next;
	while (list != start)
	{
		printf("ind list  ==  %d\n", (int) list->id_fork.id);
		if (list->next)
			list = list->next;
	}
	printf("\n\n");
}

int	clear_2link_circ_list(t_2link_circ_list **list)
{
	t_2link_circ_list	*aux;
	t_2link_circ_list	*next_aux;

	if (!*list)
		return (1);
	aux = *list;
	next_aux = (*list)->next;
	while (next_aux != *list)
	{
		if (aux)
			free(aux);
		aux = next_aux;
		next_aux = aux->next;
	}
	if (aux)
		free(aux);
	*list = NULL;
	return (0);
}

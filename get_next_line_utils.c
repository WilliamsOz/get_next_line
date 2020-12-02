/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 09:32:04 by user42            #+#    #+#             */
/*   Updated: 2020/12/02 02:21:32 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line.h"

int		free_book(int fd, t_node **book, int i)
{
	t_node	*before;
	t_node	*after;
	t_node	*keep;

	keep = (*book);
	while ((*book)->s_fd != fd)
	{
		(*book) = (*book)->next;
		i++;
	}
	if (i == 0)
	{
		after = (*book)->next;
		free((*book));
		(*book) = after;
		return (1);
	}
	after = (*book)->next;
	before = keep;
	while (before->next != (*book))
		before = before->next;
	free((*book));
	before->next = after;
	(*book) = keep;
	return (1);
}

char	*strsjoin(char *str1, char *str2, int i, int len)
{
	char	*dest;

	if (str1 != NULL)
		while (str1[i++] != '\0')
			len++;
	i = 0;
	if (str2 != NULL)
		while (str2[i++] != '\0')
			len++;
	if (!(dest = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	dest[len] = '\0';
	i = 0;
	len = 0;
	if (str1 != NULL)
		while (str1[i] != '\0')
			dest[len++] = str1[i++];
	i = 0;
	if (str2 != NULL)
		while (str2[i] != '\0')
			dest[len++] = str2[i++];
	return (dest);
}

char	*get_line_in_rest(t_node *book, char *buffer, int i, size_t is_eof)
{
	char	*temp;
	int		clear_buffer;

	while (book->s_line[i] != '\0' && book->s_line[i] != '\n')
		i++;
	if (book->s_line[i] == '\n' || is_eof == 0)
		return (book->s_line);
	is_eof = read(book->s_fd, buffer, BUFFER_SIZE);
	temp = strsjoin(book->s_line, buffer, 0, 0);
	if (book->s_line != NULL)
		free(book->s_line);
	book->s_line = temp;
	clear_buffer = -1;
	while (++clear_buffer < BUFFER_SIZE + 1)
		buffer[clear_buffer] = '\0';
	book->s_line = get_line_in_rest(book, buffer, 0, is_eof);
	return (book->s_line);
}

char	*get_rest(t_node *book, char *buffer, int i)
{
	size_t	is_eof;
	char	*keep;

	is_eof = read(book->s_fd, buffer, BUFFER_SIZE);
	keep = strsjoin(book->s_line, buffer, 0, 0);
	if (book->s_line != NULL)
		free(book->s_line);
	book->s_line = keep;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = '\0';
	book->s_line = get_line_in_rest(book, buffer, 0, is_eof);
	return (book->s_line);
}

char	*get_next_rest(t_node *book, char *buffer, int i, size_t is_eof)
{
	char	*keep;
	char	*temp;
	char	*get_rest;
	size_t	cpy;

	if (book->s_line[i + 1] != '\0')
	{
		keep = strsjoin(book->s_line + i + 1, NULL, 0, 0);
	}
	else
		keep = NULL;
	if (!(temp = (char*)malloc(sizeof(char) * (is_eof + 1))))
		return (NULL);
	temp[is_eof] = '\0';
	cpy = -1;
	while (++cpy < is_eof)
		temp[cpy] = buffer[cpy];
	get_rest = strsjoin(keep, temp, 0, 0);
	if (keep != NULL)
		free(keep);
	free(temp);
	if (book->s_line != NULL)
		free(book->s_line);
	book->s_line = get_rest;
	return (book->s_line);
}

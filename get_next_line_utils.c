/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 09:32:04 by user42            #+#    #+#             */
/*   Updated: 2020/12/01 14:16:41 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line.h"

int		free_book(const int fd, t_node *book)
{
	t_node	*before;
	t_node	*after;
	t_node	*keep;

	if (book->next == NULL && book->s_fd == fd)
	{
		if (book->s_line != NULL)
			free(book->s_line);
		free(book);
	}
	else
	{
		keep = book;
		before = book;
		while (book->s_fd != fd)
			book = book->next;
		while (before->next != book)
			before = before->next;
		after = book->next;
		if (book->s_line != NULL)
			free(book->s_line);
		free(book);
		before->next = after;
	}
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
	else
	{
		is_eof = read(book->s_fd, buffer, BUFFER_SIZE);
		temp = strsjoin(book->s_line, buffer, 0, 0);
		free(book->s_line);
		book->s_line = temp;
		clear_buffer = -1;
		while (++clear_buffer < BUFFER_SIZE + 1)
			buffer[clear_buffer] = '\0';
		book->s_line = get_line_in_rest(book, buffer, 0, is_eof);
	}
	return (book->s_line);
}

char	*get_rest(t_node *book, char *buffer, int i)
{
	size_t	is_eof;
	char	*keep;

	is_eof = read(book->s_fd, buffer, BUFFER_SIZE);
	if (is_eof > 0)
	{
		keep = book->s_line;
		book->s_line = strsjoin(keep, buffer, 0, 0);
		if (keep != NULL)
			free(keep);
		while (++i < BUFFER_SIZE + 1)
			buffer[i] = '\0';
	}
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
		keep = strsjoin(book->s_line + i + 1, NULL, 0, 0);
	else
		keep = NULL;
	cpy = -1;
	if (!(temp = (char*)malloc(sizeof(char) * (is_eof + 1))))
		return (NULL);
	temp[is_eof] = '\0';
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 09:32:04 by user42            #+#    #+#             */
/*   Updated: 2020/11/26 18:45:34 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_book(const int fd, t_node **book)
{
	t_node	*before;
	t_node	*after;
	t_node	*keep;

	if ((*book)->next == NULL && (*book)->s_fd == fd)
	{
		free(*book);
		(*book) = NULL;
	}
	else
	{
		keep = (*book);
		before = (*book);
		while ((*book)->s_fd != fd)
			(*book) = (*book)->next;
		while (before->next != (*book))
			before = before->next;
		after = (*book)->next;
		if ((*book)->s_line != NULL)
			free((*book)->s_line);
		free((*book));
		before->next = after;
		(*book) = keep;		
	}
}

char	*strsjoin(char *str1, char *str2, char *dest, int len)
{
	int		i;

	i = 0;
	if (str1 != NULL)
		while (str1[i++] != '\0')
			len++;
	i = 0;
	if (str2 != NULL)
		while (str2[i++])
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

char	*cpy_rest_from_buffer(t_node *book, char *buffer, size_t is_eof, int i)
{
	char	*keep;
	char	*temp;

	keep = NULL;
	if (book->s_line != NULL)
		keep = book->s_line;
	if (!(temp = (char*)malloc(sizeof(char) * (is_eof + 1))))
		return (NULL);
	temp[is_eof] = '\0';
	while (++i < (int)is_eof)
		temp[i] = buffer[i];
	book->s_line = strsjoin(keep, temp, book->s_line, 0);
	if (keep != NULL)
		free(keep);
	free(temp);
	return (book->s_line);
}

char	*still_readable(t_node *book, char *line, int i)
{
	char *temp;

	if (!(line = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	line[i] = '\0';
	i = -1;
	while (book->s_line[++i] != '\0' && book->s_line[i] != '\n')
		line[i] = book->s_line[i];
	temp = book->s_line;
	book->s_line = strsjoin(temp + i + 1, NULL, book->s_line, 0);
	free(temp);
	return (line);
}

char	*get_line(t_node *book, char *line, int i, size_t is_eof)
{
	char buffer[BUFFER_SIZE];
	
	is_eof = read(book->s_fd, buffer, BUFFER_SIZE);
	book->s_line = cpy_rest_from_buffer(book, buffer, is_eof, -1);
	i = 0;
	while (book->s_line[i] != '\0' && book->s_line[i] != '\n')
		i++;
	if (book->s_line[i] == '\0' && is_eof == 0)
	{
		line = strsjoin(book->s_line, NULL, line, 0);
		free(book->s_line);
		book->s_line = NULL;
	}
	else
		line = still_readable(book, line, i);
	return (line);
}

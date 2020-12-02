/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 14:43:23 by user42            #+#    #+#             */
/*   Updated: 2020/12/02 12:40:29 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line.h"

char	*get_line(t_node *book, char *line, int len, char *buffer)
{
	size_t	is_eof;

	is_eof = 0;
	while (book->s_line[len] != '\0' && book->s_line[len] != '\n')
		len++;
	if (!(line = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	line[len] = '\0';
	len = 0;
	while (book->s_line[len] != '\0' && book->s_line[len] != '\n')
	{
		line[len] = book->s_line[len];
		len++;
	}
	if (book->s_fd > 0)
		is_eof = read(book->s_fd, buffer, BUFFER_SIZE);
	if (is_eof == 0 && book->s_line[len] == '\0')
	{
		if (book->s_line != NULL)
			free(book->s_line);
		book->s_line = NULL;
	}
	else
		book->s_line = get_next_rest(book, buffer, len, is_eof);
	return (line);
}

char	*first_node(const int fd, char *line, t_node **book)
{
	int		i;
	char	buffer[BUFFER_SIZE + 1];

	if (!((*book) = (t_node*)malloc(sizeof(t_node) * 1)))
		return (NULL);
	(*book)->next = NULL;
	(*book)->s_fd = fd;
	(*book)->s_line = NULL;
	i = -1;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = '\0';
	(*book)->s_line = get_rest((*book), buffer, -1);
	i = -1;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = '\0';
	line = get_line((*book), line, 0, buffer);
	return (line);
}

char	*new_node(const int fd, char *line, t_node **book)
{
	char	buffer[BUFFER_SIZE + 1];
	t_node	*new_book;
	int		i;

	if (!(new_book = (t_node*)malloc(sizeof(t_node) * 1)))
		return (NULL);
	new_book->s_fd = fd;
	new_book->next = (*book);
	new_book->s_line = NULL;
	(*book) = new_book;
	i = -1;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = '\0';
	(*book)->s_line = get_rest((*book), buffer, -1);
	i = -1;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = '\0';
	line = get_line((*book), line, 0, buffer);
	return (line);
}

char	*current_node(char *line, t_node *book)
{
	char	buffer[BUFFER_SIZE + 1];
	int		i;

	i = -1;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = '\0';
	book->s_line = get_rest(book, buffer, -1);
	i = -1;
	while (++i < BUFFER_SIZE + 1)
		buffer[i] = '\0';
	line = get_line(book, line, 0, buffer);
	return (line);
}

int		get_next_line(int fd, char **line)
{
	static t_node	*book;
	t_node			*temp;

	if (!line || fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0))
		return (-1);
	if (book == NULL)
		*line = first_node(fd, *line, &book);
	else
	{
		temp = book;
		while (temp != NULL && fd != temp->s_fd)
			temp = temp->next;
		if (temp == NULL)
			*line = new_node(fd, *line, &book);
		else
		{
			*line = current_node(*line, temp);
			if (temp->s_line == NULL && (free_book(fd, &book, 0)) == 1)
				return (0);
		}
	}
	if (book->s_line == NULL && (free_book(fd, &book, 0)) == 1)
		return (0);
	return (1);
}

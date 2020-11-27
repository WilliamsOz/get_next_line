/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 14:43:23 by user42            #+#    #+#             */
/*   Updated: 2020/11/27 11:52:19 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*manage_rest(char *buffer, t_node *book, char *line, size_t is_eof)
{
	int		clear_buffer;
	int		i;
	
	if (is_eof > 0)
		book->s_line = cpy_rest_from_buffer(book, buffer, is_eof, -1);
	i = 0;
	while (book->s_line[i] != '\0' && book->s_line[i] != '\n')
		i++;
	if (is_eof == 0 || book->s_line[i] == '\n')
	{
		line = get_line(book, line, 0, is_eof);
		return (line);
	}
	else if (book->s_line[i] == '\0' && is_eof > 0)
	{
		clear_buffer = -1;
		while (++clear_buffer < BUFFER_SIZE)
			buffer[clear_buffer] = '\0';
		is_eof = read(book->s_fd, buffer, BUFFER_SIZE);
		line = manage_rest(buffer, book, line, is_eof);
	}
	return (line);
}

char	*first_node(const int fd, char *line, t_node **book, char *buffer)
{
	size_t	is_eof;
	int		i;

	if (!((*book) = (t_node*)malloc(sizeof(t_node) * 1)))
		return (NULL);
	(*book)->next = NULL;
	(*book)->s_fd = fd;
	(*book)->s_line = NULL;
	i = -1;
	while (++i < BUFFER_SIZE)
		buffer[i] = '\0';
	is_eof = read(fd, buffer, BUFFER_SIZE);
	line = manage_rest(buffer, (*book), line, is_eof);
	return (line);
}

char	*new_node(const int fd, char *line, t_node **book, char *buffer)
{
	t_node	*new_book;
	size_t	is_eof;
	int		i;

	if (!(new_book = (t_node*)malloc(sizeof(t_node) * 1)))
		return (NULL);
	new_book->s_fd = fd;
	new_book->next = (*book);
	new_book->s_line = NULL;
	(*book) = new_book;
	i = -1;
	while (++i < BUFFER_SIZE)
		buffer[i] = '\0';
	is_eof = read(fd, buffer, BUFFER_SIZE);
	line = manage_rest(buffer, (*book), line, is_eof);
	return (line);
}

char	*current_node(char *buffer, char *line, t_node **book)
{
	size_t	is_eof;
	int		i;

	i = -1;
	while (++i < BUFFER_SIZE)
		buffer[i] = '\0';
	is_eof = read((*book)->s_fd, buffer, BUFFER_SIZE);
	line = manage_rest(buffer, (*book), line, is_eof);
	return (line);
}

int		get_next_line(int fd, char **line)
{
	static t_node	*book;
	t_node			*temp;
	char			buffer[BUFFER_SIZE];

	if (!line || fd < 0 || BUFFER_SIZE <= 0)
		return (-1);
	if (book == NULL)
		*line = first_node(fd, *line, &book, buffer);
	else if (book != NULL && book->s_line != NULL)
	{
		temp = book;
		while (temp != NULL && fd != temp->s_fd)
			temp = temp->next;
		if (temp == NULL)
			*line = new_node(fd, *line, &book, buffer);
		else if (book->s_line != NULL)
			*line = current_node(buffer, *line, &temp);
	}
	if (book->s_line == NULL)
	{
		free_book(fd, &book);
		return (0);
	}
	return (1);
}

// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>

// int		main(void)
// {
// 	char	*line;
// 	int		file;
// 	int		res;

// 	file = open("./bible2.txt", O_RDONLY);
// 	res = 1;
// 	while (res > 0)
// 	{
// 		res = get_next_line(file, &line);
// 		if (res != -1)
// 		{
// 			printf(res ? "|%s|\n" : "|%s|", line);
// 			free(line);
// 		}
// 		else
// 		{
// 			printf("error -1");
// 			return (0);
// 		}
// 	}
// 	close(file);
// 	return (0);
// }

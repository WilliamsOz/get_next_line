/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiozsert <wiozsert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 14:56:05 by user42            #+#    #+#             */
/*   Updated: 2020/11/27 11:52:20 by wiozsert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

// #ifndef BUFFER_SIZE
// #define BUFFER_SIZE 30
// #endif

# include <stdlib.h>
# include <unistd.h>

typedef struct			s_node
{
	int					s_fd;
	char				*s_line;
	struct s_node		*next;
}						t_node;

void					free_book(const int fd, t_node **book);
char					*strsjoin(char *str1, char *str2, char *dest,
	int len);
char					*cpy_rest_from_buffer(t_node *book, char *buffer,
	size_t is_eof, int i);
char					*get_rest(t_node *book, int i, char *buffer,
	int clear_buffer);
char					*get_line(t_node *book, char *line, int i,
	size_t is_eof);
char					*manage_rest(char *buffer, t_node *book, char *line,
	size_t is_eof);
char					*first_node(const int fd, char *line, t_node **book,
	char *buffer);
char					*new_node(const int fd, char *line, t_node **book,
	char *buffer);
char					*current_node(char *buffer, char *line,
	t_node **book);
int						get_next_line(const int fd, char **line);

//DEL

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

# define DEBUG printf("ICI\n");
# define PRINTD(x) printf("%d\n", x);
# define PRINTC(x) printf("%c\n", x);
# define PRINTS(x) printf("%s\n", x);
# define PRINTZ(x) printf("%zu\n", x);

//DEL

#endif

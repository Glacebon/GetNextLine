/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoskela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 07:44:46 by hkoskela          #+#    #+#             */
/*   Updated: 2019/12/07 13:12:41 by hkoskela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Function: actually_actually_get_the_line
** Increases len until encountering newline or null. If encounters newline,
** uses len as the length of the sentence in strsub for *line, then creates tmp
** duplicate with the rest of the string, frees the old (*fds) and sets the tmp
** to it. If this new *fds starts with null, frees it. If did not encounter
** newline assigns a duplicate of the whole string to *line. Returns 1.
*/

int		actually_actually_get_the_line(char **fds, char **line)
{
	int		len;
	char	*tmp;

	len = 0;
	while ((*fds)[len] != '\n' && (*fds)[len] != '\0')
		len++;
	if ((*fds)[len] == '\n')
	{
		if (!(*line = ft_strsub(*fds, 0, len))
			|| !(tmp = ft_strdup(&((*fds)[len + 1]))))
			return (-1);
		ft_strdel(fds);
		*fds = tmp;
		if ((*fds)[0] == '\0')
			ft_strdel(fds);
	}
	else
	{
		if (!(tmp = ft_strdup(*fds)))
			return (-1);
		*line = tmp;
		ft_strdel(fds);
	}
	return (1);
}

/*
** Function: actually_get_the_line
** Does checks that didn't fit anywhere else. If ret is less than 0 something
** fishy is going on, so returns -1 for error. If ret is 0 and there is no
** fds[fd] returns 0 to tell the reading has ended. Otherwise will go to
** actually, ACTUALLY get the line and return 1.
*/

int		actually_get_the_line(char **fds, char **line, int ret)
{
	if (ret == 0 && !(*fds))
	{
		*line = NULL;
		return (0);
	}
	return (actually_actually_get_the_line(fds, line));
}

/*
** Function: get_next_line
** Validate parameters and start reading file. Static char fds will store
** strings based on the file descriptor number so the function knows if
** that file has been opened before. If it hasn't, it will create the first
** use by duplicating from buf. Later will use strjoin that returns a fresh
** pointer stored in tmp variable so the old one has to be freed and then
** set as the pointer for the joined thingie. Then checks to find a newline,
** if so, breaks out and actually gets the line and returns its result.
*/

int		get_next_line(const int fd, char **line)
{
	int			ret;
	char		*tmp;
	char		buf[BUFF_SIZE + 1];
	static char	*fds[MAX_FD];

	if (!line || fd < 0 || read(fd, buf, 0) < 0)
		return (-1);
	while (!(ft_strchr(fds[fd], '\n')) && (ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if (!fds[fd])
		{
			if (!(fds[fd] = ft_strdup(buf)))
				return (-1);
		}
		else
		{
			if (!(tmp = ft_strjoin(fds[fd], buf)))
				return (-1);
			ft_strdel(&fds[fd]);
			fds[fd] = tmp;
		}
	}
	return (actually_get_the_line(&fds[fd], line, ret));
}

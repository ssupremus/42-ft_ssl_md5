/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itiievsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 11:15:29 by itiievsk          #+#    #+#             */
/*   Updated: 2018/08/24 11:15:32 by itiievsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include "./libft/libft.h"
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>

# define FUNCTIONS 2
# define FUNC_NAME 7
# define FLAGS "pqrs"

typedef struct		s_ssl_func
{
	char			func[FUNC_NAME];
	char			type[3];
	void			(*handler)(int, char**, int);
}					t_ssl_func;

typedef struct		s_file
{
	int				fd;
	char			*target;
	char			*string;
	int				p;
	int				q;
	int				r;
	int				s;
}					t_file;

typedef struct		s_flags
{
	int				nomore;
	int				p;
	int				q;
	int				r;
	int				s;
}					t_flags;

void				md5(int argc, char **argv, int i);
void				sha256(int argc, char **argv, int i);
void				wrong_argument(t_ssl_func handler[], char **argv, int i);
void				usage(void);
void				flags_init(t_flags *flags);
void				md5_s_error(t_flags *flags);
void				md5_err_flag(char ch, t_flags *flags);
void				md5_encrypt(char *str, t_flags *flags);
#endif

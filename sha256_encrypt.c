/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_encrypt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itiievsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 09:25:12 by itiievsk          #+#    #+#             */
/*   Updated: 2018/09/07 09:25:14 by itiievsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "sha256.h"

void		sha256_print(char *str, t_flags *flags, t_sha256_ctx *ctx)
{
	int			i;

	i = -1;
	if (!flags->q && !flags->r && !flags->p)
		ft_printf("SHA256 (%c%s%c) = ", (flags->s ? '\"' : 0),
		(flags->s ? str : ctx->file), (flags->s ? '\"' : 0));
	while (++i < 8)
		ft_printf("%8x", ctx->state[i]);
	((!flags->q && !flags->r) || flags->q || flags->p) ? ft_printf("\n") : 0;
	if (!flags->q && flags->r && !flags->p)
		ft_printf(" %c%s%c\n", (flags->s ? '\"' : 0),
		(flags->s ? str : ctx->file), (flags->s ? '\"' : 0));
}

void		sha256_abcd_assign(t_sha256_ctx *ctx, char order)
{
	if (order == '<')
	{
		ctx->a = ctx->state[0];
		ctx->b = ctx->state[1];
		ctx->c = ctx->state[2];
		ctx->d = ctx->state[3];
		ctx->e = ctx->state[4];
		ctx->f = ctx->state[5];
		ctx->g = ctx->state[6];
		ctx->h = ctx->state[7];
	}
	else if (order == '>')
	{
		ctx->state[0] += ctx->a;
		ctx->state[1] += ctx->b;
		ctx->state[2] += ctx->c;
		ctx->state[3] += ctx->d;
		ctx->state[4] += ctx->e;
		ctx->state[5] += ctx->f;
		ctx->state[6] += ctx->g;
		ctx->state[7] += ctx->h;
	}
}

void		sha256_transform(uint32_t *input, t_sha256_ctx *ctx)
{
	uint32_t i;
	uint32_t t1;
	uint32_t t2;
	uint32_t w[64];

	for(i=0; i<16; i++)
		w[i] = input[i];
	for(i=16; i<64; i++)
		w[i] = w[i-16] + S0(w[i-15]) + w[i-7] + S1(w[i-2]);
	sha256_abcd_assign(ctx, '<');
	for(i=0; i<64; i++)
	{
		t1 = ctx->h + E1(ctx->e) + CH(ctx->e, ctx->f, ctx->g) + g_words[i] + w[i];
		t2 = E0(ctx->a) + MAJ(ctx->a, ctx->b, ctx->c);
		ctx->h = ctx->g;
		ctx->g = ctx->f;
		ctx->f = ctx->e;
		ctx->e = ctx->d + t1;
		ctx->d = ctx->c;
		ctx->c = ctx->b;
		ctx->b = ctx->a;
		ctx->a = t1 + t2;
	}
	sha256_abcd_assign(ctx, '>');
}

void		sha256_process(uint32_t **input, t_sha256_ctx *ctx)
{
	int		j;

	j = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
	while (j < ctx->blocks)
	{
		sha256_transform(input[j], ctx);
		j++;
	}
}

void		sha256_encrypt(char *str, t_flags *flags, t_sha256_ctx *ctx)
{
	int i, j, m, n;
	uint64_t len;
	uint32_t **input;

	for(len = ctx->len * 8 + 1; len % 512!=448; len++);
	len = len + 64;
	ctx->blocks = len / 512;
	input = (uint32_t**)calloc(ctx->blocks, sizeof(uint32_t*));

	for(i=0; i < ctx->blocks; i++)
		input[i] = (uint32_t*)calloc(16, sizeof(uint32_t));

	i = j = n = 0;
	m = 3;

	while(str[n] != '\0')
	{
		input[i][j] |= str[n] << m * 8;
		n++;
		if(m > 0)
			m--;
		else
		{
			m = 3;

			if(j < 16)
				j++;
			else
			{
				j = 0;
				i++;
			}
		}
	}
	input[i][j] |= 0x80 << m * 8;
	input[ctx->blocks-1][14] = (uint32_t)((ctx->len * 8) >> 32);
	input[ctx->blocks-1][15] = (uint32_t)((ctx->len * 8) & 0xffffffff);
	sha256_process(input, ctx);
	sha256_print(str, flags, ctx);
}
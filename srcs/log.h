/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 16:45:05 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 16:47:24 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# include <stdbool.h>
# include <stddef.h>
# include <errno.h>

# define INFO __FILE__, __func__, __LINE__

# define LOG_DEBUG(...) private_log(INFO, "DEBUG", __VA_ARGS__)

# define LOG_ERROR(...) private_log(INFO, "ERROR", __VA_ARGS__)
# define LOG_WARNING(...) private_log(INFO, "WARNING", __VA_ARGS__)

# define FATAL(...) do { LOG_ERROR(__VA_ARGS__); return (false); } while (0)

# define CHECK(expr) do { if (! (expr)) FATAL (#expr " failed"); } while (0)

# define LOG_MSG_SIZE_MAX		511

bool	write_data (const int fd, const char *msg, const size_t size);

void	private_log(const char *file,
					const char *func,
					const int line,
					const char *logtype,
					const char *fmt, ...) __attribute__((format(printf,5,6)));

#endif

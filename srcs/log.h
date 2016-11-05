#ifndef LOG_H
# define LOG_H

# include <stdbool.h>
# include <errno.h>
# include <stddef.h>

# define DEBUG	1

# define INFO __FILE__, __func__, __LINE__

# define PRIVATE_LOG(logtype, ...) do { if (DEBUG) { private_log(INFO, logtype, __VA_ARGS__); } } while (0)
# define LOG_DEBUG(...) PRIVATE_LOG("DEBUG", __VA_ARGS__)
# define LOG_WARNING(...) PRIVATE_LOG("WARNING", __VA_ARGS__)
# define LOG_ERROR(...) PRIVATE_LOG("ERROR", __VA_ARGS__)
# define LOG_ERROR_SYS(msg, error_number) private_sys_log(INFO, msg, error_number)

# define FATAL(...) do { LOG_ERROR(__VA_ARGS__); return (false); } while (0)

# define CHECK(expr) do { if (! (expr)) FATAL(#expr " failed"); } while (0)
# define CHECK_SYS(msg, error_number) do { if (error_number) { LOG_ERROR_SYS(msg, error_number); return (false); } } while (0)

# define LOG_MSG_SIZE_MAX		511

bool	write_data (const int fd, const char *msg, const size_t size);

void	log_info(const char *fmt, ...);

void	private_log(const char *file,
									const char *func,
									const int line,
									const char *logtype,
									const char *fmt, ...) __attribute__((format(printf, 5, 6)));

void	private_sys_log(const char *file,
											const char *func,
											const int line,
											const char *msg,
											const int error_number);

void	private_check(const char *file,
										const char *func,
										const int line,
										const char *expr,
										const bool eval,
										const char *fmt, ...) __attribute__((format(printf,6,7)));

#endif

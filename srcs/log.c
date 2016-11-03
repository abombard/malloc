#include "log.h"
#include <unistd.h>			/* write() */
#include <stdarg.h>			/* va_start(), va_end() */
#include <stdio.h>			/* snprintf(), vsnprintf() */
#include <string.h>     /* strlen(), memcpy() */

bool	write_data (const int fd, const char *msg, const size_t size)
{
	size_t	written;
	ssize_t	nwritten;

	written = 0;
	while (written < size)
	{
		nwritten = write (fd, msg + written, size - written);
		if (nwritten < 0)
		{
      LOG_ERROR_SYS("write", errno);
			return ( false );
		}
		written += (size_t)nwritten;
	}
	return ( true );
}

void  log_info(const char *fmt, ...)
{
  va_list  ap;
  int      size;
  char     msg[LOG_MSG_SIZE_MAX];

  va_start(ap, fmt);
  size = vsnprintf(msg, LOG_MSG_SIZE_MAX, fmt, ap);
  va_end(ap);
  if (size < 0)
  {
    LOG_ERROR_SYS("vsnprintf", errno);
    return ;
  }
  else if ((size_t)size >= sizeof(msg))
  {
		fprintf (stderr, "snprintf size %d sizeof(msg) %d\n", size, (int)sizeof(msg));
		return ;
  }
  msg[size] = '\n';
  size += 1;
  write_data(1, msg, (size_t)size);
}

static bool build_info(const char *logtype,
                       const char *file,
                       const int line,
                       const char *func,
                       const size_t msg_size_max,
                       char *msg,
                       size_t *msg_size)
{
  int size;

  *msg_size = 0;
	size = snprintf (msg, msg_size_max, "%s:%s:%d:%s():", logtype, file, line, func);
	if (size < 0 || (size_t)size >= msg_size_max)
	{
		LOG_ERROR("snprintf size %d msg_size_max %zu\n", size, msg_size_max);
		return (false);
	}
  *msg_size = (size_t)size;
  return (true);
}

void	private_log(const char *file,
									const char *func,
									const int line,
									const char *logtype,
									const char *fmt, ...)
{
	va_list	ap;
	int			size;
	size_t	msg_size;
	char		msg[LOG_MSG_SIZE_MAX];

  if (!build_info(logtype, file, line, func, sizeof(msg), msg, &msg_size))
  {
    fprintf(stderr, "build_info failed file %s line %d func %s sizeof(msg) %zu", file, line, func, (size_t)sizeof(msg));
    return ;
  }
	va_start (ap, fmt);
	size = vsnprintf (msg + msg_size, LOG_MSG_SIZE_MAX - msg_size, fmt, ap);
	va_end (ap);
	if (size < 0 || (size_t)size >= sizeof(msg))
	{
		fprintf (stderr, "vsnprintf failed size %d\n", size);
		return ;
	}
  msg_size += (size_t)size;
  msg[msg_size] = '\n';
  msg_size += 1;
	write_data (2, msg, msg_size);
}

void	private_sys_log(const char *file,
                      const char *func,
                      const int line,
                      const char *in_msg,
                      const int error_number)
{
	char		msg[LOG_MSG_SIZE_MAX];
	size_t	msg_size;
  char    error_msg[LOG_MSG_SIZE_MAX];
  size_t  error_msg_size;
  int     ret;
  size_t  in_msg_size;

  if (!build_info("SYS_ERR", file, line, func, sizeof(msg), msg, &msg_size))
  {
    fprintf(stderr, "build_info failed file %s line %d func %s sizeof(msg) %zu", file, line, func, (size_t)sizeof(msg));
    return ;
  }
  in_msg_size = strlen(in_msg);
  if (msg_size + in_msg_size > sizeof(msg))
  {
    fprintf(stderr, "Not enough place LOG MSG SIZE %d", LOG_MSG_SIZE_MAX);
    return ;
  }
  memcpy(msg + msg_size, in_msg, in_msg_size);
  ret = strerror_r(error_number, error_msg, sizeof (error_msg));
  if (ret)
  {
    fprintf (stderr, "strerror_r failed ret %d error_number %d sizeof (error_msg) %zu\n", ret, error_number, (size_t)sizeof (error_number));
    return ;
  }
  error_msg_size = strlen (error_msg);
  if (msg_size + error_msg_size >= sizeof(msg))
  {
    fprintf (stderr, "msg_size %zu error_msg_size %zu sizeof(msg) %zu", msg_size, error_msg_size, sizeof(msg));
    return ;
  }
  memcpy (msg + msg_size, error_msg, error_msg_size);
  msg_size += error_msg_size;
  msg[msg_size] = '\n';
  msg_size += 1;
	write_data (2, msg, msg_size);
}

void  private_check(const char *file,
                    const char *func,
                    const int line,
                    const char *expr,
                    const bool eval,
                    const char *fmt, ...)
{
	char		msg[LOG_MSG_SIZE_MAX];
	size_t	msg_size;
  int     size;
  va_list ap;

  if (!build_info("CHECK", file, line, func, sizeof(msg), msg, &msg_size))
  {
    fprintf(stderr, "build_info failed file %s line %d func %s sizeof(msg) %zu", file, line, func, (size_t)sizeof(msg));
    return ;
  }
  size = snprintf(msg + msg_size, sizeof(msg) - msg_size, "\n\tEXPRESSION: %s\nEVAL: %s\n LOG: ", expr, eval ? "true":"false");
  if (size < 0 || (size_t)size >= sizeof(msg) - msg_size)
  {
    fprintf(stderr, "snprintf failed size %d\n", size);
    return ;
  }
  msg_size += (size_t)size;
  va_start(ap, fmt);
  size = vsnprintf(msg + msg_size, sizeof(msg) - msg_size, fmt, ap);
  va_end(ap);
  if (size < 0 || (size_t)size >= sizeof(msg) - msg_size)
  {
    fprintf(stderr, "vsnprintf failed size %d\n", size);
    return ;
  }
  msg_size += (size_t)size;
  msg[msg_size] = '\n';
  msg_size += 1;
  write_data(2, msg, msg_size);
}

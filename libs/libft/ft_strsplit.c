#include "libft.h"

static int		get_next_entry(char *s, char c, size_t *begin, size_t *size)
{
	*begin = 0;
	*size = 0;
	while (*s && *s == c)
	{
		s++;
		(*begin)++;
	}
	while (*s && *s != c)
	{
		s++;
		(*size)++;
	}
	return (*size ? 1 : 0);
}

static size_t	compute_entry_count(char *s, char c)
{
	size_t	entry_count;
	size_t	s_offset;
	size_t	entry_offset;
	size_t	entry_size;

	entry_count = 0;
	s_offset = 0;
	while (get_next_entry(s + s_offset, c, &entry_offset, &entry_size))
	{
		s_offset += entry_offset + entry_size;
		entry_count++;
	}
	return (entry_count);
}

static int		fill_array(char *s, char c, char **array)
{
	size_t	s_offset;
	size_t	array_index;
	char	*entry;
	size_t	entry_offset;
	size_t	entry_size;

	array_index = 0;
	s_offset = 0;
	while (get_next_entry(s + s_offset, c, &entry_offset, &entry_size))
	{
		entry = (char *)malloc(entry_size + 1);
		if (!entry)
			return (0);
		s_offset += entry_offset;
		ft_memcpy(entry, s + s_offset, entry_size);
		entry[entry_size] = '\0';
		s_offset += entry_size;
		array[array_index] = entry;
		array_index++;
	}
	return (1);
}

char			**ft_strsplit(char const *s, char c)
{
	size_t	entry_count;
	char	**array;

	if (s == NULL)
		return (NULL);
	entry_count = compute_entry_count((char *)s, c);
	array = malloc(sizeof(char *) * (entry_count + 1));
	if (!array)
		return (NULL);
	array[entry_count] = 0;
	if (!fill_array((char *)s, c, array))
		return (NULL);
	return (array);
}

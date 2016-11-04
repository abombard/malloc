#ifndef INTERN_MALLOC_H
# define INTERN_MALLOC_H

extern void       *call_mmap(size_t size);
extern bool       call_munmap(void *addr, size_t size);

extern t_region   *new_region(t_handle *handle);
extern bool       del_region(t_region *region);

extern t_quantum  *quantum_retrieve(t_memory_type type, size_t quantum_size);
extern t_quantum  *quantum_setup(t_quantum *quantum, t_memory_type type, size_t offset, size_t size);

extern void       free_list_add(t_quantum *quantum, t_list *free_list_add);
extern void       free_list_del(t_quantum *quantum);

extern bool       region_show_list(t_list *head);
extern bool       region_show(t_region *region);
extern bool       quantum_show_list(t_list *head);
extern bool       quantum_show(t_quantum *quantum);

extern size_t get_multiple(size_t size, size_t mult);

#endif


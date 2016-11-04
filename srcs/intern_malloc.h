#ifndef INTERN_MALLOC_H
# define INTERN_MALLOC_H

extern t_region   *region_add(t_list *head, t_quantum_type type, size_t size);
extern bool       region_del(t_region *region);

extern t_quantum  *quantum_find(t_list *head, size_t size);
extern void       quantum_retrieve(t_quantum *quantum, size_t quantum_size, t_list *quantum_free_head);
extern t_quantum  *quantum_setup(t_quantum *quantum, t_quantum_type type, size_t offset, size_t size);

extern void       free_list_add(t_quantum *quantum, t_list *free_list_add);
extern void       free_list_del(t_quantum *quantum);

extern void show_mem(void);
extern bool region_show_list(t_list *head);
extern bool region_show(t_region *region);
extern bool quantum_show_list(t_list *head);
extern bool quantum_show(t_quantum *quantum);

extern size_t get_multiple(size_t size, size_t mult);

#endif


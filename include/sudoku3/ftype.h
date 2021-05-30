#ifndef FTYPE_H
#define FTYPE_H 1

/* field type */
typedef unsigned int f_t;
/* unsigned type */
typedef unsigned int ushort_t;

/* chk if the number is set within f_t */
ushort_t ft_isset(f_t*, ushort_t num);
/* 
	sets a num within f_t. 
 */
void ft_set(f_t*,ushort_t num);
/* unsets a num within f_t. */
void ft_unset(f_t*, ushort_t num);
ushort_t ft_allset(f_t*);
ushort_t ft_noneset(f_t*);
ushort_t ft_list_unset(f_t*,ushort_t**, ushort_t);
ushort_t ft_list_set(f_t*,ushort_t**, ushort_t);
ushort_t ft_list_set_ct(f_t*, ushort_t lim);
ushort_t ft_is_listset (f_t* s,  ushort_t n, ushort_t pos[]);
ushort_t ft_is_listsetn(f_t* s,  ushort_t n, ...);
ushort_t ft_is_listunset (f_t* s,  ushort_t n, ushort_t pos[]);
ushort_t ft_is_listunsetn(f_t* s,  ushort_t n, ...);
#endif

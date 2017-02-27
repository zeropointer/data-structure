#ifndef  __ATOM_H_
#define  __ATOM_H_

#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define INIT_BUCKET_SIZE 1024


struct _atom_node {
    struct _atom* link;
    int len;
    char str[0];
};

typedef struct _atom_node atom_t;

extern void        atom_init (int hint);
extern int         atom_length (const char* str);
extern const char* atom_new (const char* str, int len);
extern const char* atom_string (const char* str);
extern const char* atom_int (long n);
extern void        atom_reset (void);
extern void        atom_free (char* str);


#endif  //__ATOM_H_


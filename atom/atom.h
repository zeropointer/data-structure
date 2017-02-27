#ifndef  __ATOM_H_
#define  __ATOM_H_

#include <string.h>

extern int Atom_length (const char* str);
extern const char* Atom_new (const char* str, int len);
extern const char* Atom_string (const char* str);
extern const char* Atom_int (long n);



#endif  //__ATOM_H_


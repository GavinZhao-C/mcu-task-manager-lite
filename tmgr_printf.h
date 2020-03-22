 
#include <stdio.h>  
#include <stdarg.h>  
  
#if defined NC308  
    #define FAR _far  
#elif defined NC30  
    #define FAR _far  
#else  
    #define FAR  
#endif  
  
int sprintf(char FAR *s, const char FAR *format, ...);
int printf(const char FAR *format, ...);


#ifndef STRINGUTIL_H
#define STRINGUTIL_H

extern bool stringContainsCTLByte(const char *str);
extern void toLowercase(char *str);
extern void toUppercase(char *str);
extern void cut_str_by_delim(const char *str, char delim, char **a, 
                             char **b);
extern bool has_prefix(const char *str, const char *prefix);

#endif /* STRINGUTIL_H */

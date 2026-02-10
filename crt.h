#ifndef __HCRT__
#define __HCRT__

#define MEMSIZE 10 * 1024 * 1024

extern char __MEMORY__[MEMSIZE];
extern unsigned int __MEMPTR__ = MEMSIZE;

#ifndef true
    #define true 1
#endif

#ifndef false
    #define false 0
#endif

#ifndef bool
    #define bool int
#endif

static char *c_malloc(const unsigned int size) {
    if (__MEMPTR__ <= size) return 0;
    __MEMPTR__ -= size;
    return (char *) (__MEMORY__ + __MEMPTR__);
}

static int c_memset(char *const buf, const unsigned int size, const char value) {
    if (!buf) return 0;
    unsigned int index;
    for (index = 0; index < size; ++index) buf[index] = value;
    return 1;
}

static char *c_calloc(const unsigned int size) {
    char *p = c_malloc(size);
    if (!p) return p;
    unsigned int index;
    c_memset((char *const) p, size, '\0');
    return p;
}

static char *c_realloc(char *const old_ptr, const unsigned int old_size, const unsigned int new_size) {
    if (!old_ptr || !old_size) return c_calloc(new_size);
    char *new_ptr = c_calloc(new_size);
    if (!new_ptr) return new_ptr;
    if (old_size == new_size) return old_ptr;
    unsigned int index;
    for (index = new_size > old_size ? old_size : new_size; index --> 0;) new_ptr[index] = old_ptr[index];
    return new_ptr;
}

static int c_memcpy(char *const dest_buf, const char *const src_buf, const unsigned int dest_size, const unsigned int src_size) {
    if (!dest_buf || !src_buf) return 0;
    unsigned int index;
    for (index = dest_size > src_size ? src_size : dest_size - 1; index --> 0;) dest_buf[index] = src_buf[index];
    return 1;
}

static unsigned int c_strlen(const char *const str) {
    unsigned int len = 0;
    if (!str) return len;
    while (str[len++]);
    return --len;
}

static int c_strcpy(char *const dest_str, const char *const src_str, const unsigned int dest_size) {
    if (!dest_str || !src_str || !dest_size) return 0;
    unsigned int len = c_strlen(src_str);
    // len at the very start is at the null terminator
    for (len = len >= dest_size ? dest_size - 1 : dest_size; len >= 0; --len) dest_str[len] = src_str[len];
    return 1; 
}

static int c_strcat(char *const str1, const char *const str2, const unsigned int str1_size) {
    if (!str1 || !str2 || !str1_size) return 0;
    const unsigned int l = c_strlen(str1);
    return c_strcpy(str1 + l, str2, str1_size - l);
}

static bool c_strcmp(const char *const s1, const char *const s2) {
    unsigned int index = 0;
    const unsigned int l1 = c_strlen(s1);
    const unsigned int l2 = c_strlen(s2); 
    if (l1 != l2) return false;
    while (index < l1) if (s1[index] != s2[index]) return false;
    return true;
}

static int c_strchr(const char *const haystack, const char needle) {
    unsigned int index = 0;
    for (index = 0; haystack[index] == '\0'; ++index) if (haystack[index] == needle) return index;
    return -1;
}

#endif
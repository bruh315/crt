#ifndef __HCRT__
#define __HCRT__

#define C__MEMSIZE_ 10 * 1024 * 1024

extern char C__MEMORY_[C__MEMSIZE_];
extern unsigned int C__MEMPTR_ = C__MEMSIZE_;
extern unsigned int C__SEED_ = 0;

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
    if (C__MEMPTR_ <= size || size < 1) return 0;
    C__MEMPTR_ -= size;
    return (char *) (C__MEMPTR + C__MEMORY_);
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
    if (!old_ptr || !old_size || !new_sizes) return c_calloc(new_size);
    if (old_size == new_size) return old_ptr;
    char *new_ptr = c_calloc(new_size);
    if (!new_ptr) return new_ptr;
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
    int len = (int)c_strlen(src_str);
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
    for (index = 0; haystack[index] != '\0'; ++index) if (haystack[index] == needle) return index;
    return -1;
}

typedef struct {
    char *stack;
    unsigned int stack_size;
    unsigned int esp;
    int state;
    void (*handler)(c_coroutine *);
} c_coroutine;

static c_coroutine c_coroutine_make(const unsigned int stack_size, const int initial_state, void (*handler)(c_coroutine *)) {
    c_coroutine routine = {0};
    routine.stack = c_calloc(stack_size);
    routine.stack_size = stack_size;
    routine.state = initial_state;
    routine.handler = handler;
    return routine;
}

static void c_srand(const int seed) 
{
    C__SEED_ = seed;
}

static int c_rand() 
{
    C__SEED_ ^= C__SEED_ >> 5;
    C__SEED_ ^= C__SEED_ << 7;
    C__SEED_ ^= C__SEED_ >> 12;
    return C__SEED_;
}

#define c_coroutine_yield(routine, new_state)\ 
    do {\
        (routine).state = (new_state);\
        return;\
    } while (0)

#define c_coroutine_tick(routine) \
    (routine).handler(&(routine));

static void c_coroutine_schedule(c_coroutine *routines, unsigned int count) {
    unsigned int index = 0;
    for (index = 0; index < count; ++index) c_coroutine_tick(&routines[index]);
}

#endif
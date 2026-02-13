#ifndef __HCRT__
#define __HCRT__

#define C__MEMSIZE_ 10 * 1024 * 1024

#ifndef CRT__VARS
#define CRT__VARS
    char C__MEMORY_[C__MEMSIZE_];
    unsigned int C__MEMPTR_ = C__MEMSIZE_;
    int C__SEED_ = 0; 
#endif

#define c_silent_assert(cond) while (!(cond));

#ifndef true
    #define true 1
#endif

#ifndef false
    #define false 0
#endif

#ifndef bool
    #define bool int
#endif

extern char *c_malloc(const unsigned int size) {
    if (C__MEMPTR_ <= size || size < 1) return 0;
    C__MEMPTR_ -= size;
    return (char *) (C__MEMPTR_ + C__MEMORY_);
}

static int c_memset(char *const buf, const unsigned int size, const char value) {
    if (!buf) return 0;
    unsigned int index;
    for (index = 0; index < size; ++index) buf[index] = value;
    return 1;
}

extern char *c_calloc(const unsigned int size) {
    char *p = c_malloc(size);
    if (!p) return p;
    unsigned int index;
    c_memset((char *const) p, size, '\0');
    return p;
}

extern char *c_realloc(char *const old_ptr, const unsigned int old_size, const unsigned int new_size) {
    if (!old_ptr || !old_size) return c_calloc(new_size);
    if (old_size == new_size) return old_ptr;
    char *new_ptr = c_calloc(new_size);
    if (!new_ptr) return new_ptr;
    unsigned int index;
    for (index = new_size > old_size ? old_size : new_size; index --> 0;) new_ptr[index] = old_ptr[index];
    return new_ptr;
}

extern void c_free(const char *const p) {
    // TODO: implement c_free(const char *const p) {
        //TODO: implement c_free(const char *const p) {
            //TODO: implement c_free(const char *const p) {
                //TODO: implement c_free(const char *const p) {
                    //TODO: implement c_free(const char *const p) {
                        //...
                    //}
                //}
            //}
        //}
    //}
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

struct c_coroutine;

typedef struct c_coroutine {
    int *stack;
    unsigned int stack_size;
    unsigned int esp;
    int state;
    void (*handler)(struct c_coroutine *);
} c_coroutine;

static c_coroutine c_coroutine_make(const unsigned int stack_size, const int initial_state, void (*handler)(c_coroutine *)) {
    c_coroutine routine = {0};
    routine.stack = (int *) c_calloc(stack_size * sizeof(int));
    c_silent_assert(routine.stack);
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

#define c_coroutine_yield(routine, new_state) \
    do {\
        (routine).state = (new_state);\
        return;\
    } while (0)

#define c_coroutine_tick(routine) \
    (routine).handler(&(routine));

static void c_coroutine_schedule(c_coroutine *routines, unsigned int count) {
    unsigned int index = 0;
    for (index = 0; index < count; ++index) (routines[index]).handler(&(routines[index]));;
}

typedef struct {
    int *items;
    unsigned int count; // Items pushed
    unsigned int capacity; // Memory allocated
} c_da;

static c_da c_da_make() {
    c_da da = {0};
    return da;
}

static int c_da_append(c_da *const da, const int element) {
    if (!da) return 0;
    if (da->count >= da->capacity) {
        if (da->capacity == 0) da->capacity = 20;
        else da->capacity *= 2;
        int *const p = (int *)c_realloc((char *)da->items, da->count * sizeof(int), da->capacity * sizeof(int));
        if (!p) return 0;
        da->items = p;
    }
    da->items[da->count++] = element;
    return 1;
}

static int c_da_pop(c_da *const da, int *const out) {
    if (!da || da->count < 1) return 0;
    *out = da->items[--da->count];
    return 1;
}

static int c_da_find(c_da *const haystack, const int element) {
    if (!haystack) return -2;
    unsigned int index = 0;
    for (index = 0; index < haystack->count; ++index) if (haystack->items[index] == element) return index;
    return -1;
}

static int c_da_reserve(c_da *const da, const unsigned int new_size) {
    if (!da || !new_size) return 0;
    int *const p = (int *const)c_realloc((char *)da->items, da->count * sizeof(int), new_size * sizeof(int));
    if (!p) return 0;
    da->items = p;
    da->capacity = new_size;
    return 1;
}

typedef struct {
    void *ref;
    int is_valid;
} c_handle;

static c_handle c_handle_load(void *const p) {
    c_handle ref;
    ref.is_valid = 1;
    ref.ref = p;
    return ref;
}

#define c_handle_set(handle, type, item)\
    do {\
        if (((handle).ref) && (handle).is_valid) (*((type *)(handle).ref)) = item;\
    } while (0)

#define c_handle_get(handle, type, out)\
    do {\
        if ((handle).ref && (handle).is_valid) out = *(((type) *)(handle).ref);\
        else (out) = (type) 0;\
    } while (0)

#define c_handle_release(handle)\
    do {\
        (handle).ref = (void *)0;\
        (handle).is_valid = 0;\
    } while (0)

#endif

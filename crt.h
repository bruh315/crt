#ifndef __HCRT__
#define __HCRT__

//  Persik C runtime.
//  Copyright 2026 - Persival' Ghrosni'i Volosatik (Персивал Грозный Волосатик)
//
//  This software is written by a dog, it is a simple subruntime (If you can even call it that) designed to be easy to use and safe.
//
//  You can use this software (almost) anywhere as long as you follow these restrictions:
//   - You must not claim you wrote the original software.
//   - You can modify it however you want as long as you do not claim that your modified version is the original one.
//   
//     Note: 
//           Crediting the creator of this software is not required but would be highly appreciated


#define C__MEMSIZE_ (((10 * 1024 * 1024)))

//#define C_SUB void

#ifndef CRT__VARS
#define CRT__VARS
    char C__MEMORY_[C__MEMSIZE_];
    unsigned int C__MEMPTR_ = C__MEMSIZE_;
    int C__SEED_ = 0; 
#endif

#define c_silent_assert(cond) while (!(cond))

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

    #ifdef ASSERTALL
        #ifndef UNASSERT_C_MALLOC_FRIENDS
            #ifndef UNASSERT_C_MALLOC
                c_silent_assert(!(C__MEMPTR_ <= size || size < 1));
            #endif    
        #endif
    #endif

    if (C__MEMPTR_ <= size || size < 1) return 0;
    C__MEMPTR_ -= size;
    return (char *) (C__MEMORY_ + C__MEMPTR_);
}

static int c_memset(char *const buf, const unsigned int size, const char value) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_MEMSET
            c_silent_assert(buf);
        #endif
    #endif
    
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

}

static unsigned int c_strlen(const char *const str) {
    
    unsigned int len = 0;

    #ifdef ASSERTALL
        #ifndef UNASSERT_C_STRLEN
            c_silent_assert(str);
        #endif
    #endif

    if (!str) return len;
    while (str[len]) ++len;
    return len;

}

static int c_strcpy(char *const dest_str, const char *const src_str, const unsigned int dest_size) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_STRCPY
            c_silent_assert(!(!dest_str || !src_str || !dest_size));
        #endif
    #endif
    
    if (!dest_str || !src_str || !dest_size) return 0;
    unsigned int len = (int)c_strlen(src_str);
    unsigned int index;
    const unsigned int n = dest_size < len ? dest_size - 1 : len;
    for (index = 0; index < n; ++index) dest_str[index] = src_str[index];
    dest_str[index] = '\0';
    return n; 
}

static int c_strcat(char *const str1, const char *const str2, const unsigned int str1_size) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_STRCAT
            c_silent_assert(!(!str1 || !str2 || !str1_size));
        #endif
    #endif

    if (!str1 || !str2 || !str1_size) return 0;
    const unsigned int l = c_strlen(str1);
    return c_strcpy(str1 + l, str2, str1_size - l);
}

static bool c_strcmp(const char *const s1, const char *const s2) {

    if (!s1 || !s2) return false;

    unsigned int index = 0;
    const unsigned int l1 = c_strlen(s1);
    const unsigned int l2 = c_strlen(s2); 
    if (l1 != l2) return false;
    while (index < l1) if (s1[index] != s2[index++]) return false;
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
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_COROUNTINE_MAKE
            c_silent_assert(routine.stack);
        #endif
    #endif
    routine.stack_size = stack_size;
    routine.state = initial_state;
    routine.handler = handler;
    return routine;
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

static void c_srand(const int seed) 
{
    C__SEED_ = seed ? seed : 1;
}

static int c_rand() 
{
    C__SEED_ = C__SEED_ ? C__SEED_ : 1;
    C__SEED_ ^= C__SEED_ >> 5;
    C__SEED_ ^= C__SEED_ << 7;
    C__SEED_ ^= C__SEED_ >> 12;
    return C__SEED_;
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
    int wn = 0;
    if (da->count >= da->capacity) {
        if (da->capacity == 0) {
            da->capacity = 20;
            wn = 1;
        }
        else da->capacity *= 2;
        int *const p = (int *)c_realloc((char *)da->items, da->count * sizeof(int), da->capacity * sizeof(int));
        if (!p) {
            if (wn) da->capacity = 0;
            else da->capacity /= 2;
            return 0;
        }
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
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_DA_FIND
            c_silent_assert(haystack);
        #endif
    #endif
    
    if (!haystack) return -2;
    unsigned int index = 0;
    for (index = 0; index < haystack->count; ++index) if (haystack->items[index] == element) return index;
    return -1;
}

static int c_da_reserve(c_da *const da, const unsigned int new_size) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_DA_RESERVE
        c_silent_assert(da && new_size);
        #endif
    #endif
    
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
    ref.is_valid = p ? 1 : 0;

    #ifdef ASSERTALL
        #ifndef UNASSERT_C_HANDLE_LOAD
            c_silent_assert(p);
        #endif
    #endif

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
    } while (0)

#define c_handle_release(handle)\
    do {\
        (handle).ref = (void *)0;\
        (handle).is_valid = 0;\
    } while (0)

typedef struct {
    char *memory;
    unsigned int count; // How much shit has been pushed
    unsigned int capacity; // Memory allocated
} c_block;

static c_block c_block_allocate(const unsigned int size) {
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_BLOCK_ALLOCATE
            c_silent_assert(size);
        #endif
    #endif
    c_block block;
    block.memory = c_calloc(size);
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_BLOCK_ALLOCATE
            c_silent_assert(block.memory);
        #endif
    #endif
    block.count = 0;
    block.capacity = size;
    return block;
}

static int c_block_write(c_block *const block, const char byte)
{
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_BLOCK_WRITE
            c_silent_assert(!(!block || !block->memory));
        #endif
    #endif

    if (!block || !block->memory) return 0;
    if (block->count >= block->capacity) {
        block->capacity *= 2;
        char *p = c_realloc(block->memory, block->count, block->capacity);
        if (!p) {
            block->capacity /= 2;
            return 0;
        };
        block->memory = p;
    }
    
    block->memory[block->count++] = byte;
    return 1;
}

static int c_block_read(c_block *const block, const unsigned int index, char *const out) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_BLOCK_READ
            c_silent_assert(block && block->memory && index < block->count);
        #endif
    #endif

    if (!block || !block->memory || index >= block->count) return 0;
    *out = block->memory[index];
    return 1;
}

static int c_sprintf(char *const out, const char *const format, void **args, const unsigned int out_size) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_SPRINTF
            c_silent_assert(out && format && args && out_size);
        #endif
    #endif

    if (!out || !format || !args || !out_size) return 0;
    unsigned int fmt_cur;
    unsigned int out_cur;
    unsigned int arg_cur;
    for (fmt_cur = 0, out_cur = 0, arg_cur = 0; format[fmt_cur] != '\0' && out_cur < out_size - 1;) {
        if (format[fmt_cur] == '%') {
            ++fmt_cur;
            if (format[fmt_cur] == 's') {
                unsigned int index = 0;
                while (((char *)args[arg_cur])[index] != '\0' && out_cur < out_size - 1) out[out_cur++] = ((char *)args[arg_cur])[index++];
                ++arg_cur;
            } else if (format[fmt_cur] == 'c') {
                if (out_cur < out_size - 2) out[out_cur++] = (char) args[arg_cur++];
            }
        } else out[out_cur++] = format[fmt_cur++];
    }
    out[out_cur] = '\0';
    return out_cur;
}

static int c_abort() {
    while(1);
}

//#define C_IMAGE
#ifdef C_IMAGE

static int c__atoi(const char *const integer, const char hook) {
    if (!integer) return 0;

    const char *i = integer;
    if (*i == '-') ++i;

    int out = 0;

    while (*i != hook) out = out * 10 + (*i++ - '0');

    if (*integer == '-') out = -out;

    return out; 
}

static int c_parse_p6(const char *const image, char *const out, const unsigned int out_size, unsigned int *const out_width, unsigned int *const out_height) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_PARSE_P6
            c_silent_assert(!(!image || !out || !out_size));
        #endif
    #endif
    
    if (!image || !out || !out_size) return 0;
    
    if (*image != 'P' || image[1] != '6' || image[2] != '\n') return -1;

    unsigned int img_cur = 3;

    while (image[img_cur] == '#') {
        while (image[img_cur] != '\n') ++img_cur;
        ++img_cur;
    };

    const int width = c__atoi(image + img_cur, ' ');
    while (image[img_cur++] != ' ');

    const int height = c__atoi(image + img_cur, '\n');
    while (image[img_cur++] != '\n');

    const unsigned int image_size = width * height;
    const unsigned int bytes = image_size * 3;
    
    if (out_width) *out_width = width;
    if (out_height) *out_height = height;

    // Ignore the maximum color value because who cares
    while (image[img_cur] != '\n') ++img_cur;
    ++img_cur;

    unsigned int out_cur = 0;

    while (out_cur < out_size && out_cur < bytes) out[out_cur++] = image[img_cur++];

    return image_size;
}

// width and height are passed as strings. Implement itoa() yourself or suffer.
static int c_write_p6(const char *const image_data, char *const out, const unsigned int out_size, const char *width, const char *height) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_WRITE_P6
            c_silent_assert(width && height);
        #endif
    #endif
    
    if (!width || !height) return 0;
    
    const unsigned int lw = c_strlen(width);
    const unsigned int lh = c_strlen(height);

    const unsigned int pixels = c__atoi(width) * c__atoi(height);
    const unsigned int bytes = pixels * 3;
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_WRITE_P6
            c_silent_assert(!(!image_data || !out || out_size < 9 + lw + lh));
        #endif
    #endif

    if (!image_data || !out || out_size < 9 + lw + lh) return 0;
    unsigned int out_cur = 0;

    out[out_cur++] = 'P';
    out[out_cur++] = '6';
    out[out_cur++] = '\n';

    while (out_cur < out_size && *width) out[out_cur++] = *width++;
    out[out_cur++] = ' ';
    
    while (out_cur < out_size && *height) out[out_cur++] = *height++;
    out[out_cur++] = '\n';

    out[out_cur++] = '2';
    out[out_cur++] = '5';
    out[out_cur++] = '5';
    out[out_cur++] = '\n';

    unsigned int img_cur = 0;
    while (img_cur < bytes && out_cur < out_size) out[out_cur++] = image_data[img_cur++];

    return out_cur;
}

static int c_read_persik(const char *const image, char *const out, const unsigned int out_size, unsigned short *const out_width, unsigned short *const out_height) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_READ_PERSIK
            c_silent_assert(out && image && out_size);
        #endif
    #endif

    if (!image || !out || !out_size) return 0;

    if (*image != 'p' || image[1] != 'e' || image[2] != 'r' || image[3] != 's' || image[4] != 'i' || image[5] != 'k' || image[6] != 'i' || image[7] != 's' || image[8] != 'c' || image[9] != 'o' || image[10] != 'o' || image[11] != 'l') return 0;

    unsigned int img_cur = 12;

    unsigned short width  = (((unsigned char) image[img_cur++]) << 8) | ((unsigned char) image[img_cur++]);
    unsigned short height = (((unsigned char) image[img_cur++]) << 8) | ((unsigned char) image[img_cur++]);

    if (!width || !height) return 0;

    if (out_width) *out_width = width;
    if (out_height) *out_height = height;

    const unsigned int pixels = width * height;
    const unsigned int bytes = pixels * 3;

    unsigned int out_cur = 0;
    
    while (out_cur < bytes && out_cur < out_size) out[out_cur++] = image[img_cur++];

    return pixels;
}

static int c_write_persik(const char *const image_data, char *const out, const unsigned int out_size, const unsigned short width, const unsigned short height) {
    
    #ifdef ASSERTALL
        #ifndef UNASSERT_C_WRITE_PERSIK
            c_silent_assert(image_data && out && out_size > 14 && width && height);
        #endif
    #endif

    if (!image_data || !out || out_size <= 14 || !width || !height) return 0;

    c_strcpy(out, "persikiscool", out_size);
    
    unsigned int out_cur = 12;

    out[out_cur++] = (((char)(width >> 8)));
    out[out_cur++] = (((char)(width)) & 0xFF);

    out[out_cur++] = (((char)(height >> 8)));
    out[out_cur++] = (((char)(height)) & 0xFF);

    const unsigned int img_size = (unsigned int)(width * height);
    const unsigned int img_bytes = img_size * 3;

    unsigned int img_cur = 0;

    while (out_cur < out_size && img_cur < img_bytes) out[out_cur++] = image_data[img_cur++];
    
    return out_cur;
}

#endif

#endif

#ifndef _OS_SUPPORT_CUSTOM_H_
#define _OS_SUPPORT_CUSTOM_H_

#define OVERRIDE_SPEEX_ALLOC
static void *speex_alloc (int size)
{
    void *ptr;

    ptr = (void *) malloc (size);

    if (ptr == NULL) {
        return NULL;
    } else {
        memset(ptr, 0, size);
        return ptr;
    }
}

#define OVERRIDE_SPEEX_ALLOC_SCRATCH
static void *speex_alloc_scratch (int size)
{
    void *ptr;

    ptr = (void *) malloc (size);

    if (ptr == NULL) {
        return NULL;
    } else {
        memset(ptr, 0, size);
        return ptr;
    }
}

#define OVERRIDE_SPEEX_REALLOC
static void *speex_realloc (void *ptr, int size)
{
    void *p;
    p = realloc(ptr, size);

    if (p == NULL) {
        return NULL;
    } else {
        return p;
    }
}

#define OVERRIDE_SPEEX_FREE
static void speex_free (void *ptr)
{
    free(ptr);
}
#define OVERRIDE_SPEEX_FREE_SCRATCH
static void speex_free_scratch (void *ptr)
{
    free(ptr);
}

#endif // #ifndef _OS_SUPPORT_CUSTOM_H_
/*********************************************************
    > File Name: preload.c
    > Author: ims
    > Created Time: Wed 21 Jul 2021 11:23:17 PM CST
 *********************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
/* Pointer to the real functions.  These are determined used `dlsym'
   when really needed.  */
static void *(*mallocp)(size_t);
static void *(*def_mallocp)(size_t);
static void (*freep) (void *);
static void (*def_freep) (void *);
static void *(*reallocp) (void *, size_t);
static void *(*callocp) (size_t, size_t);
static int malloc_cnt = 0;


//can't use those func which use malloc avoid recursion;
void* malloc(size_t size)
{
    malloc_cnt++;
    mallocp = (void *(*)(size_t))dlsym (RTLD_NEXT, "malloc");
    return (*mallocp)(size);
}
void free(void *ptr)
{
    mallocp = (void *(*)(size_t))dlsym (RTLD_NEXT, "malloc");
    def_mallocp = (void *(*)(size_t))dlsym (RTLD_DEFAULT, "malloc");
    freep = (void (*)(void *))dlsym (RTLD_NEXT, "free");
    def_freep = (void (*)(void *))dlsym (RTLD_DEFAULT, "free");
    printf("%s:malloc_cnt:%d\n", __func__,malloc_cnt);
    printf("%s:def_mallocp:%p\n", __func__,def_mallocp);
    printf("%s:mallocp:%p\n", __func__,mallocp);
    printf("%s:def_freep:%p\n", __func__,def_freep);
    printf("%s:freep:%p\n", __func__,freep);
    return (*freep)(ptr);
}

void *calloc(size_t nmemb, size_t size)
{
    callocp = (void *(*)(size_t, size_t))dlsym (RTLD_NEXT, "calloc");
    printf("%s:callocp:%p\n", __func__,callocp);
    return (*callocp)(nmemb, size);
}
void *realloc(void *ptr, size_t size)
{
    reallocp = (void *(*)(void *, size_t))dlsym (RTLD_NEXT, "realloc");
    printf("%s:reallocp:%p\n", __func__,reallocp);
    return (*reallocp)(ptr, size);
}

#include "ldac.h"

/***************************************************************************************************
    Align Memory
***************************************************************************************************/
#define LDAC_ALLOC_LINE 8

DECLFUNC size_t align_ldac(
    size_t size)
{
    if (LDAC_ALLOC_LINE != 0)
    {
        size = (((size - 1) / LDAC_ALLOC_LINE) + 1) * LDAC_ALLOC_LINE;
    }

    return size;
}

/***************************************************************************************************
    Clear Allocate Memory
***************************************************************************************************/
DECLFUNC void *calloc_ldac(
    SFINFO *p_sfinfo,
    size_t nmemb,
    size_t size)
{
    char *p_mempos; // r3
    void *v5;       // [sp+4h] [bp-Ch] BYREF

    p_mempos = p_sfinfo->p_mempos;
    if (p_mempos)
    {
        p_sfinfo->p_mempos = &p_mempos[nmemb * align_ldac(size)];
        return p_mempos;
    }
    else
    {
        v5 = malloc(nmemb * size);
        // syspool_get_buff(&v5, nmemb * size);
        return v5;
    }
}
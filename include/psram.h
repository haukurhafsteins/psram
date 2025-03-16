#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
void *psram_malloc(int size);
void *psram_calloc(int num, int size);
void psram_free(void *ptr);
void *psram_realloc(void *ptr, int size);
int psram_size(void *ptr);
int psram_roundup(int size);
int psram_init(void *notUsed);
void psram_shutdown(void *notUsed);
int psram_get_free_size();

#ifdef __cplusplus
}
#endif

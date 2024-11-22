#include <esp_heap_caps.h>
#include <esp_log.h>
#include "psram.h"

#define USE_SPIRAM 1

static const char* TAG = "PSRAM";

void* psram_malloc(int size)
{
#if USE_SPIRAM
    void* ptr = heap_caps_malloc(size + sizeof(size_t), MALLOC_CAP_SPIRAM);
    if (ptr == NULL)
    {
        ESP_LOGE(TAG, "malloc failed for size %d", size);
        return NULL;
    }
    if (size > 1024*1024)
    {
        ESP_LOGW(TAG, "psram_malloc: Large allocation of %d bytes", size);
    }
    // Store the size at the beginning
    *((size_t*)ptr) = size;
    return (char*)ptr + sizeof(size_t);
#else
    return malloc(size);
#endif
}

void psram_free(void* ptr)
{
#if USE_SPIRAM
    if (ptr == NULL) return;
    // Adjust the pointer back to the original allocation
    void* original_ptr = (char*)ptr - sizeof(size_t);
    heap_caps_free(original_ptr);
#else
    free(ptr);
#endif
}

void* psram_calloc(int num, int size)
{
#if USE_SPIRAM
    void* ptr = heap_caps_calloc(1, num * size + sizeof(size_t), MALLOC_CAP_SPIRAM);
    if (ptr == NULL)
    {
        ESP_LOGE(TAG, "calloc failed for size %d", num * size);
        return NULL;
    }
    if (num * size > 1024*1024)
    {
        ESP_LOGW(TAG, "psram_calloc: Large allocation of %d bytes", num * size);
    }
    // Store the size at the beginning
    *((size_t*)ptr) = num * size;
    return (char*)ptr + sizeof(size_t);
#else
    return calloc(num, size);
#endif
}

void* psram_realloc(void* ptr, int size)
{
#if USE_SPIRAM
    if (ptr == NULL)
    {
        return psram_malloc(size);
    }
    // Adjust the pointer back to the original allocation
    void* original_ptr = (char*)ptr - sizeof(size_t);
    void* newPtr = heap_caps_realloc(original_ptr, size + sizeof(size_t), MALLOC_CAP_SPIRAM);
    if (newPtr == NULL)
    {
        ESP_LOGE(TAG, "realloc failed for size %d", size);
        return NULL;
    }
    // Store the new size at the beginning
    *((size_t*)newPtr) = size;
    return (char*)newPtr + sizeof(size_t);
#else
    return realloc(ptr, size);
#endif
}

int psram_size(void* ptr)
{
#if USE_SPIRAM
    if (ptr == NULL) return 0;
    // Adjust the pointer back to the original allocation
    void* original_ptr = (char*)ptr - sizeof(size_t);
    size_t* size = (size_t*)original_ptr;
    return *size;
#else
    return 0;
#endif
}

int psram_get_free_size()
{
    return heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
}

int psram_roundup(int size)
{
#if USE_SPIRAM
    return (size + sizeof(long long) - 1) & ~(sizeof(long long) - 1);
#else
    return size;
#endif
}

int psram_init(void* notUsed)
{
    return 0;
}

void psram_shutdown(void* notUsed)
{
}

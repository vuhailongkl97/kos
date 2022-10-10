#ifndef STRING
#define STRING

#include <kernel/type.h>

using namespace kernel;

namespace std
{
 void* memmove(void* des, void* src, size_t size);
 void* memset(void* des, uint8 c, size_t size);
 int memcmp(const void *ptr1, const void* ptr2, size_t num);
}

#endif
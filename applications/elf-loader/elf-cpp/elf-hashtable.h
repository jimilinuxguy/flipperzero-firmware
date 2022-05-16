#pragma once
#include <stdint.h>
#include "../elf-lib/elf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get function address by function name
 * @param name function name
 * @param address output for function address
 * @return true if the table contains a function
 */
bool elf_resolve_from_hashtable(const char* name, Elf32_Addr* address);

#ifdef __cplusplus
}
#endif
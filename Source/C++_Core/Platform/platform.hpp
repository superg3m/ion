#pragma once

#include "../Common/common.hpp"
#include "../Memory/memory.hpp"
// Date: August 04, 2025
// NOTE(Jovanni): Im cheating here a bit I should have like win32 platform and linux platform but im just gonna do glfw and win32

namespace ION::Platform {
    typedef void* DLL;

    void sleep(u32 ms);
    bool file_path_exists(const char* path);
    Memory::Allocator get_allocator();
    /**
     * @brief returns true if copy succeeded
     * 
     * @param source_path 
     * @param dest_path 
     * @param block_until_success
     */
    bool copy_file(const char* source_path, const char* dest_path, bool block_until_success = true);
    u8* read_entire_file(Memory::Allocator& allocator, const char* file_path, byte_t& out_file_size, ION_Error& error);
    DLL load_dll(const char* dll_path, ION_Error& error);
    DLL free_dll(DLL dll, ION_Error& error);
    void* get_proc_address(DLL dll, const char* proc_name, ION_Error& error);
}
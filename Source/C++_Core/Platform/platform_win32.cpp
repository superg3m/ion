#if defined(PLATFORM_WINDOWS)
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <windowsx.h>
    #include <timeapi.h>

    #include "platform.hpp"

    #include "../Common/logger.hpp"
    #include "../Common/assert.hpp"
    #include "../Common/error.hpp"

    namespace ION::Platform {
        global double g_frequency = {0};
        global double g_start_time = {0};
        global TIMECAPS g_device_time_caps;

        bool file_path_exists(const char* path) {
            return (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES);
        }

        bool copy_file(const char* source_path, const char* dest_path, bool block_until_success) {
            if (block_until_success) {
                while (!CopyFileA(source_path, dest_path, FALSE)) {
                    Sleep(10);
                }

                return true;
            } 

            return CopyFileA(source_path, dest_path, FALSE) == 0;
        }

        void sleep(u32 ms) {
            Sleep((DWORD)ms);
        }
        
        internal void* win32_malloc(const Memory::Allocator* allocator, byte_t allocation_size) {
            (void)allocator;
            return VirtualAlloc(NULL, allocation_size, MEM_COMMIT, PAGE_READWRITE);
        }

        internal void win32_free(const Memory::Allocator* allocator, void* data) {
            (void)allocator;
            VirtualFree(data, 0, MEM_RELEASE);
        }

        Memory::Allocator get_allocator() {
            return Memory::Allocator(win32_malloc, win32_free);
        }

        u8* read_entire_file(Memory::Allocator& allocator, const char* file_path, byte_t& out_file_size, ION_Error& error) {
            HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (file_handle == INVALID_HANDLE_VALUE) {
                LOG_ERROR("CreateFileA() returned an INVALID_HANDLE_VALUE, the file_path/path is likely wrong: read_entire_file(%s)\n", file_path);
                error = ION_ERROR_RESOURCE_NOT_FOUND;
                
                return nullptr;
            }

            LARGE_INTEGER large_int;
            ION::Memory::zero(&large_int, sizeof(LARGE_INTEGER));
            BOOL success = GetFileSizeEx(file_handle, &large_int);
            if (!success) {
                LOG_ERROR("GetFileSizeEx() Failed to get size from file_handle: read_entire_file(%s)\n", file_path);
                error = ION_ERROR_RESOURCE_NOT_FOUND;
                CloseHandle(file_handle);
                return nullptr;
            }

            byte_t file_size = (byte_t)large_int.QuadPart;
            if (file_size > SIZE_MAX) {
                LOG_ERROR("File size is bigger than max size: read_entire_file(%s)\n", file_path);
                error = ION_ERROR_RESOURCE_TOO_BIG;
                CloseHandle(file_handle);

                return nullptr;
            }

            u8* file_data = (u8*)allocator.malloc(file_size + 1); // +1 for null-terminator

            DWORD bytes_read = 0;
            success = ReadFile(file_handle, file_data, (DWORD)file_size, &bytes_read, nullptr);
            CloseHandle(file_handle);
            if (!success && bytes_read == file_size) {
                LOG_ERROR("ReadFile() Failed to get the file data or bytes read doesn't match file_size: read_entire_file(%s)\n", file_path);
                error = ION_ERROR_RESOURCE_NOT_FOUND;
                allocator.free(file_data);
                return nullptr;
            }

            out_file_size = (byte_t)file_size;

            return file_data;
        }

        DLL load_dll(const char* dll_path, ION_Error& error)  {
            HMODULE library = LoadLibraryA(dll_path);
            if (!library) {
                LOG_ERROR("LoadLibraryA() failed: load_dll(%s)\n", dll_path);
                error = ION_ERROR_RESOURCE_NOT_FOUND;

                return nullptr;
            }

            return library;
        }

        DLL free_dll(DLL dll, ION_Error& error)  {
            RUNTIME_ASSERT(dll);
            FreeLibrary((HMODULE)dll);

            return nullptr;
        }

        void* get_proc_address(DLL dll, const char* proc_name, ION_Error& error) {
            RUNTIME_ASSERT(dll);

            void* proc = (void*)GetProcAddress((HMODULE)dll, proc_name);
            if (!proc) {
                LOG_ERROR("GetProcAddress() failed: get_proc_address(%s)\n", proc_name);
                error = ION_ERROR_RESOURCE_NOT_FOUND;
                return nullptr;
            }

            return proc;
        }
    }
#endif
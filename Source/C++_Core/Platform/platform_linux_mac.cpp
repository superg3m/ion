#if defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)
    #include "platform.hpp"

    #include "../Common/logger.hpp"
    #include "../Common/assert.hpp"
    #include "../Common/error.hpp"

    namespace ION::Platform {
        bool file_path_exists(const char* path) {
            FILE *fptr = fopen(path, "r");

            if (fptr == NULLPTR) {
                return false;
            }

            fclose(fptr);

            return true;
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

        Memory::Allocator get_allocator() {
            return Memory::Allocator::libc();
        }

        u8* read_entire_file(const char* file_name, size_t* returned_file_size, CKG_Error* err) {
            FILE* file_handle = fopen(file_name, "rb");
            if (file_handle == NULLPTR) {
                LOG_ERROR("Invalid file_handle, the file_name/path is likely wrong: read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);

                return NULLPTR;
            }

            if (fseek(file_handle, 0L, SEEK_END) != 0) {
                LOG_ERROR("fseek failed: read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                fclose(file_handle);
                return NULL;
            }

            long file_size = ftell(file_handle);
            if (file_size == -1L) {
                LOG_ERROR("ftell failed: read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                fclose(file_handle);
                return NULL;
            }

            rewind(file_handle);
            if (ferror(file_handle)) {
                LOG_ERROR("rewind() failed: read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                fclose(file_handle);
                return NULL;
            }

            u8* file_data = ckg_alloc((size_t)file_size + 1); // +1 for null terminator
            if (fread(file_data, file_size, 1, file_handle) != 1) {
                LOG_ERROR(false, "fread() failed: read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                ckg_free(file_data);
                fclose(file_handle);

                return NULLPTR;
            }

            fclose(file_handle);

            if (returned_file_size) {
                *returned_file_size = file_size + 1;
            }

            return file_data;
        }

        DLL load_dll(const char* dll_path, ION_Error& error)  {
            Hvoid* library = dlopen(dll_name, RTLD_LAZY);
            if (!library) {
                LOG_ERROR("dlopen() failed: load_dll(%s)\n", dll_name);
                error = ION_ERROR_RESOURCE_NOT_FOUND;

                return nullptr;
            }

            return library;
        }

        DLL free_dll(DLL dll, ION_Error& error)  {
            RUNTIME_ASSERT(dll);
            dlclose(dll);

            return nullptr;
        }

        void* get_proc_address(DLL dll, const char* proc_name, ION_Error& error) {
            RUNTIME_ASSERT(dll);

            void* proc = dlsym(dll, proc_name);
            if (!proc) {
                LOG_ERROR("dlsym() failed: ckg_os_get_proc_address(%s)\n", proc_name);
                error = ION_ERROR_RESOURCE_NOT_FOUND;

                return nullptr;
            }

            return proc;
        }
    }
#endif
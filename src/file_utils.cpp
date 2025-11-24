#include "file_utils.hpp"
#include <bit>

#ifdef OS_WINDOWS
#include "win32_helper.hpp"
#include <ntstatus.h>
#include <winternl.h>

namespace {
using pNtCreateFile = NTSTATUS(__stdcall *)(
    PHANDLE FileHandle, ULONG DesiredAccess, PVOID ObjectAttributes,
    PVOID IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes,
    ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions,
    PVOID EaBuffer, ULONG EaLength);

using pNtReadFile = ULONG(__stdcall *)(
    IN HANDLE FileHandle, IN HANDLE Event OPTIONAL,
    IN PVOID ApcRoutine OPTIONAL, IN PVOID ApcContext OPTIONAL,
    OUT PVOID IoStatusBlock, OUT PVOID Buffer, IN ULONG Length,
    IN PLARGE_INTEGER ByteOffset OPTIONAL, IN PULONG Key OPTIONAL);

std::expected<HANDLE, std::error_code>
win32_open_file(std::string_view filename) {

  auto create_file = (pNtCreateFile)GetProcAddress(
      GetModuleHandle(L"ntdll.dll"), "NtCreateFile");
  if (create_file == nullptr) {
    return std::unexpected{make_win32_error_code()};
  }

  if ((filename == ".") || (filename == "..")) {
    return std::unexpected{
        make_error_code(win32_error_code{ERROR_INVALID_NAME})};
  }

  auto name = to_wtf8(filename.data(), filename.size());
  HANDLE ret = nullptr;

  UNICODE_STRING file_name = {.Length = static_cast<USHORT>(name.length()<<1),
                              .MaximumLength =
                                  static_cast<USHORT>(sizeof(PWSTR)+1),
                              .Buffer = name.data()};

  OBJECT_ATTRIBUTES oa = {.Length = sizeof(OBJECT_ATTRIBUTES),
                          .RootDirectory = nullptr,
                          .ObjectName = &file_name,
                          .Attributes = OBJ_CASE_INSENSITIVE,
                          .SecurityDescriptor = nullptr,
                          .SecurityQualityOfService = nullptr};
  IO_STATUS_BLOCK io = {};

  auto res =
      create_file(&ret, GENERIC_ALL, &oa, &io, nullptr,
                  FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE,
                  FILE_CREATE, FILE_NON_DIRECTORY_FILE, nullptr, 0);

  if (res != STATUS_SUCCESS) {
    return std::unexpected{make_HRESULT_error_code(res)};
  }

  return ret;
}
} // namespace
#endif

std::expected<HANDLE, std::error_code> test_win32_file() {
  return win32_open_file("\\??\\C:\\Users\\subbu\\Documents\\test.txt");
}

usize file_handle_helper::file_size(file_handle &handle) {
  auto read_size = fseek(&*handle, 0, SEEK_END);
  if (read_size == 0) {
    auto size = ftell(&*handle);
    if (fseek(&*handle, 0, SEEK_SET) == 0) {
      return static_cast<usize>(size);
    }
  }
  return static_cast<usize>(read_size);
}

usize file_handle_helper::file_position(file_handle &handle) {
  return static_cast<usize>(ftell(&*handle));
}

bool file_handle_helper::is_eof(file_handle &handle) {
  return feof(&*handle) == 0U;
}

std::vector<u8> file_handle_helper::read_vec_u8(file_handle &handle, u32 size) {
  if (size != 0) {
    std::vector<u8> ret(size);
    if (fread(ret.data(), 1, size, &*handle) == size) {
      return ret;
    }
  }
  return {};
}

std::vector<u8> file_handle_helper::read_vec_u8(file_handle &handle) {
  auto size = read_u64(handle);
  if (size != 0) {
    std::vector<u8> ret(size);
    if (fread(ret.data(), 1, size, &*handle) == size) {
      return ret;
    }
  }
  return {};
}

usize file_handle_helper::write_vec_u8(file_handle &handle,
                                       std::vector<u8> &vec) {
  if (write_u64(handle, vec.size()) == vec.size()) {
    if (auto write_size = fwrite(vec.data(), 1, vec.size(), &*handle);
        write_size == vec.size()) {
      return write_size;
    }
  }
  return 0;
}

u8 file_handle_helper::read_u8(file_handle &handle) {
  std::array<u8, sizeof(u8)> buffer{};
  u8 value{};
  if (fread(buffer.data(), 1, sizeof(u8), &*handle) == sizeof(u8)) {
    memcpy(&value, buffer.data(), sizeof(u8));
    return value;
  }
  return 0;
}

u16 file_handle_helper::read_u16(file_handle &handle, bool big_endian) {
  std::array<u8, sizeof(u16)> buffer{};
  u16 value{};
  if (fread(buffer.data(), 1, sizeof(u16), &*handle) == sizeof(u16)) {
    memcpy(&value, buffer.data(), sizeof(u16));
    if (big_endian) {
      return std::byteswap(value);
    }
    return value;
  }
  return 0;
}

u32 file_handle_helper::read_u32(file_handle &handle, bool big_endian) {
  std::array<u8, sizeof(u32)> buffer{};
  u32 value{};
  if (fread(buffer.data(), 1, sizeof(u32), &*handle) == sizeof(u32)) {
    memcpy(&value, buffer.data(), sizeof(u32));
    if (big_endian) {
      return std::byteswap(value);
    }
    return value;
  }
  return 0;
}

u64 file_handle_helper::read_u64(file_handle &handle, bool big_endian) {
  std::array<u8, sizeof(u64)> buffer{};
  u64 value{};
  if (fread(buffer.data(), 1, sizeof(u64), &*handle) == sizeof(u64)) {
    memcpy(&value, buffer.data(), sizeof(u64));
    if (big_endian) {
      return std::byteswap(value);
    }
    return value;
  }
  return 0;
}

std::string file_handle_helper::read_z_string(file_handle &handle) {
  std::vector<char> buffer{};
  int file_char{};
  while ((file_char = fgetc(&*handle)) != EOF) {
    buffer.emplace_back(static_cast<char>(file_char));
    if (file_char == '\0') {
      break;
    }
  }
  return buffer.data();
}

std::string file_handle_helper::read_u8_string(file_handle &handle) {
  auto size = read_u8(handle);
  std::vector<u8> buffer(size);

  if (fread(buffer.data(), 1, size, &*handle) == size) {
    return {buffer.begin(), buffer.end()};
  }
  return {};
}

std::string file_handle_helper::read_u16_string(file_handle &handle,
                                                bool big_endian) {
  auto size = read_u16(handle, big_endian);
  std::vector<u8> buffer(size);
  if (fread(buffer.data(), 1, size, &*handle) == size) {
    return {buffer.begin(), buffer.end()};
  }
  return {};
}

std::string file_handle_helper::read_u32_string(file_handle &handle,
                                                bool big_endian) {
  auto size = read_u32(handle, big_endian);
  std::vector<u8> buffer(size);
  if (fread(buffer.data(), 1, size, &*handle) == size) {
    return {buffer.begin(), buffer.end()};
  }
  return {};
}

std::string file_handle_helper::read_string(file_handle &handle, u64 size) {
  std::vector<u8> buffer(size);
  (void)fread(buffer.data(), 1, size, &*handle);
  return {buffer.begin(), buffer.end()};
}

usize file_handle_helper::write_u8(file_handle &handle, u8 value) {
  if (auto size = fwrite(&value, 1, sizeof(u8), &*handle); size == sizeof(u8)) {
    return size;
  }
  return 0;
}

usize file_handle_helper::write_u16(file_handle &handle, u16 value,
                                    bool big_endian) {
  if (big_endian) {
    value = std::byteswap(value);
  }
  if (auto size = fwrite(&value, 1, sizeof(u16), &*handle);
      size == sizeof(u16)) {
    return size;
  }
  return 0;
}

usize file_handle_helper::write_u32(file_handle &handle, u32 value,
                                    bool big_endian) {
  if (big_endian) {
    value = std::byteswap(value);
  }
  if (auto size = fwrite(&value, 1, sizeof(u32), &*handle);
      size == sizeof(u32)) {
    return size;
  }
  return 0;
}

usize file_handle_helper::write_u64(file_handle &handle, u64 value,
                                    bool big_endian) {
  if (big_endian) {
    value = std::byteswap(value);
  }
  if (auto size = fwrite(&value, 1, sizeof(u64), &*handle);
      size == sizeof(u64)) {
    return size;
  }
  return 0;
}

bool file_handle_helper::write_z_string(file_handle &handle,
                                        const std::string &str) {
  auto ret = fputs(str.c_str(), &*handle) != EOF;
  return ret && (fwrite("", 1, 1, &*handle) != 0U);
}

bool file_handle_helper::write_u8_string(file_handle &handle,
                                         const std::string &str) {
  auto sz_res = write_u8(handle, static_cast<u8>(str.size()));
  return (sz_res != 0U) && fputs(str.c_str(), &*handle) != EOF;
}

bool file_handle_helper::write_u16_string(file_handle &handle,
                                          const std::string &str,
                                          bool big_endian) {
  auto sz_res = write_u16(handle, static_cast<u16>(str.size()), big_endian);
  return (sz_res != 0U) && fputs(str.c_str(), &*handle) != EOF;
}

bool file_handle_helper::write_u32_string(file_handle &handle,
                                          const std::string &str,
                                          bool big_endian) {
  auto sz_res = write_u32(handle, static_cast<u32>(str.size()), big_endian);
  return (sz_res != 0U) && fputs(str.c_str(), &*handle) != EOF;
}

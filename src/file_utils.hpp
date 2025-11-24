#pragma once
#include "defines.hpp"
#include <expected>
#include <memory>
#include <system_error>

using file_deleter = void (*)(FILE *handle);
using file_handle = std::unique_ptr<FILE, file_deleter>;

class FileHandle {
  u64 internal_handle;

public:
  explicit FileHandle(u64 handle);
  ~FileHandle();
  FileHandle(const FileHandle &) = delete;
  FileHandle(FileHandle &&) = default;
  FileHandle &operator=(const FileHandle &) = delete;
  FileHandle &operator=(FileHandle &&) = default;
};

std::expected<HANDLE, std::error_code> test_win32_file();

struct file_handle_helper {
  [[nodiscard]] static usize file_size(file_handle &handle);
  [[nodiscard]] static usize file_position(file_handle &handle);
  [[nodiscard]] static bool is_eof(file_handle &handle);

  //***************************Buffer Read**********************************//
  [[nodiscard]] static std::vector<u8> read_vec_u8(file_handle &handle,
                                                   u32 size);
  [[nodiscard]] static std::vector<u8> read_vec_u8(file_handle &handle);
  [[nodiscard]] static usize write_vec_u8(file_handle &handle,
                                          std::vector<u8> &vec);

  //***************************Intiger Read**********************************//
  [[nodiscard]] static u8 read_u8(file_handle &handle);
  [[nodiscard]] static u16 read_u16(file_handle &handle,
                                    bool big_endian = false);
  [[nodiscard]] static u32 read_u32(file_handle &handle,
                                    bool big_endian = false);

  [[nodiscard]] static u64 read_u64(file_handle &handle,
                                    bool big_endian = false);

  //***************************Intiger Write**********************************//
  [[nodiscard]] static usize write_u8(file_handle &handle, u8 value);

  [[nodiscard]] static usize write_u16(file_handle &handle, u16 value,
                                       bool big_endian = false);

  [[nodiscard]] static usize write_u32(file_handle &handle, u32 value,
                                       bool big_endian = false);

  [[nodiscard]] static usize write_u64(file_handle &handle, u64 value,
                                       bool big_endian = false);

  //***************************String Read**********************************//
  [[nodiscard]] static std::string read_z_string(file_handle &handle);
  [[nodiscard]] static std::string read_u8_string(file_handle &handle);

  [[nodiscard]] static std::string read_u16_string(file_handle &handle,
                                                   bool big_endian = false);

  [[nodiscard]] static std::string read_u32_string(file_handle &handle,
                                                   bool big_endian = false);

  [[nodiscard]] static std::string read_string(file_handle &handle, u64 size);

  //***************************String Write**********************************//
  [[nodiscard]] static bool write_z_string(file_handle &handle,
                                           const std::string &str);
  [[nodiscard]] static bool write_u8_string(file_handle &handle,
                                            const std::string &str);

  [[nodiscard]] static bool write_u16_string(file_handle &handle,
                                             const std::string &str,
                                             bool big_endian = false);

  [[nodiscard]] static bool write_u32_string(file_handle &handle,
                                             const std::string &str,
                                             bool big_endian = false);
};

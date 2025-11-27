#include "known-folders.hpp"

#ifdef OS_WINDOWS
#include "platform/win32_known_folders.hpp"
#endif

std::expected<std::filesystem::path, std::error_code> get_path(Folders folder) {
#ifdef OS_WINDOWS
  return get_win32_path(folder);
#endif
#ifdef OS_LINUX
  return {};
#endif
}

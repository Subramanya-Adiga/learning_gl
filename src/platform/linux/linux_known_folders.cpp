#include "linux_known_folders.hpp"

#include "../../known-folders.hpp"

#include <fstream>
#include <linux/unistd.h>
#include <pwd.h>
#include <string>
#include <unistd.h>

namespace {
std::unordered_map<u8, std::string> lookup;

std::string get_home() {
  auto uid = getuid();
  auto *home = getenv("HOME");

  if (uid != 0 && (home != nullptr)) {
    return home;
  }

  passwd *pw = nullptr;
  passwd pwd = {};

  auto bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (bufsize < 1) {
    bufsize = 16384;
  }

  auto *buffer = (char *)malloc(bufsize);

  auto err = getpwuid_r(uid, &pwd, buffer, bufsize, &pw);
  if (err == ERANGE) {
    bufsize *= 2;
    buffer = nullptr;
    buffer = (char *)malloc(bufsize);
    err = getpwuid_r(uid, &pwd, buffer, bufsize, &pw);
  }
  std::string res_dir = pw->pw_dir;

  free(buffer);

  return res_dir;
}

void init() {
  lookup.emplace(static_cast<u8>(Folders::home), get_home());
  auto file = std::ifstream(lookup[static_cast<u8>(Folders::home)] +
                            "/.config/user-dirs.dirs");
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      using std::operator""sv;
      for (auto &&dat : line | std::ranges::views::split('=') |
                            std::ranges::views::chunk(2)) {

        auto first = dat.begin();
        auto second = dat.end() ;

        if (std::string_view(first) == "XDG_DESKTOP_DIR") {
        }
        if (std::string_view(dat) == "XDG_DOWNLOAD_DIR") {
        }
        if (std::string_view(dat) == "XDG_TEMPLATES_DIR") {
        }
        if (std::string_view(dat) == "XDG_PUBLICSHARE_DIR") {
        }
        if (std::string_view(dat) == "XDG_DOCUMENTS_DIR") {
        }
        if (std::string_view(dat) == "XDG_MUSIC_DIR") {
        }
        if (std::string_view(dat) == "XDG_PICTURES_DIR") {
        }
        if (std::string_view(dat) == "XDG_VIDEOS_DIR") {
        }
      }
    }
  }
}

} // namespace

std::expected<std::filesystem::path, std::error_code>
linux_get_path(Folders fld) {
  if (lookup.empty()) {
    init();
    return lookup[static_cast<u8>(fld)];
  }
  return lookup[static_cast<u8>(fld)];
}

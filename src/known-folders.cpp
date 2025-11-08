#include "known-folders.hpp"

#ifdef OS_WINDOWS

#include "win32_helper.hpp"
#include <ShlObj.h>
#include <strsafe.h>

namespace {
KNOWNFOLDERID convert(Folders fld) {
  switch (fld) {
  case Folders::home:
    return FOLDERID_Profile;
  case Folders::documents:
    return FOLDERID_Documents;
  case Folders::pictures:
    return FOLDERID_Pictures;
  case Folders::music:
    return FOLDERID_Music;
  case Folders::videos:
    return FOLDERID_Videos;
  case Folders::desktop:
    return FOLDERID_Desktop;
  case Folders::downloads:
    return FOLDERID_Downloads;
  case Folders::pub:
    return FOLDERID_Public;
  case Folders::fonts:
    return FOLDERID_Fonts;
  case Folders::app_menu:
    return FOLDERID_StartMenu;
  case Folders::roaming_configuration:
    return FOLDERID_RoamingAppData;
  case Folders::local_configuration:
    return FOLDERID_LocalAppData;
  case Folders::global_configuration:
    return FOLDERID_ProgramData;
  case Folders::data:
  case Folders::cache:
  case Folders::runtime:
    return FOLDERID_LocalAppData;
  default:
    return {};
  }
  return {};
}

std::expected<std::string, std::error_code> get_win32_path(Folders fld) {
  auto val = convert(fld);

  PWSTR path = nullptr;
  auto res = SHGetKnownFolderPath(val, 0, nullptr, &path);
  if (res != S_OK) {
    CoTaskMemFree(path);
    return std::unexpected{make_HRESULT_error_code(res)};
  }

  usize len = 0;
  auto str_res = StringCchLength(path, STRSAFE_MAX_CCH, &len);
  if (str_res != S_OK) {
    CoTaskMemFree(path);
    return std::unexpected{make_HRESULT_error_code(str_res)};
  }

  auto str = to_utf8(path, static_cast<u32>(len));
  CoTaskMemFree(path);
  if (fld == Folders::cache || fld == Folders::data ||
      fld == Folders::runtime) {
    return str + "\\Temp\\";
  }
  return str;
}

} // namespace

#endif

std::expected<std::filesystem::path, std::error_code> get_path(Folders folder) {
  if (auto path = get_win32_path(folder); path.has_value()) {
    return *path;
  } else {
    return std::unexpected{path.error()};
  }
}

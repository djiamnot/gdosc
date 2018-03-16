#include "utils.h"

namespace utils

{
  std::string gdStringToString(String s) {
    std::wstring ws(s.c_str());
    std::string _ret(ws.begin(), ws.end());
    return _ret;
  }
}

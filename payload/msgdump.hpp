// msgdump.hpp --- Dump the Windows message
// Copyright (C) 2017 Katayama Hirofumi MZ. License: CC0.

#ifndef MSGDUMP_HPP
#define MSGDUMP_HPP

#include <windows.h>
#include <string>

//////////////////////////////////////////////////////////////////////////////

std::string md_dump(UINT uMsg, WPARAM wParam, LPARAM lParam);
std::string md_dump_rect(const RECT *prc);

//////////////////////////////////////////////////////////////////////////////

#endif  // ndef MSGDUMP_HPP

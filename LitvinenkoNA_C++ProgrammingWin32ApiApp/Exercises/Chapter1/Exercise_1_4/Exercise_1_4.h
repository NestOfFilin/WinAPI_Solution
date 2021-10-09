#pragma once

#include <Windows.h>
#include <xstring>

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
    std::allocator<TCHAR> > String;

#include "Resource.h"

#pragma once

#include <Windows.h>
#include <xstring>

#include <vector>
#include <memory>
#include <ctime>

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
    std::allocator<TCHAR> > String;

#include "Resource.h"

#pragma once

#include <IO/Stream.hpp>
#include "BreakInfoModel.hpp"

namespace Utl
{
    void PullRegister(Library::IO::BufferStream & stream, RegisterInfo & info);
}

#pragma once

#include <Debug/Breakpoint.hpp>
#include <IO/Stream.hpp>

namespace Utl
{
    void PushRegister(Library::IO::BufferStream & stream, Library::Debug::RegisterInfo const & info);
}
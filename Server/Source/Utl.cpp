#include <Debug/Breakpoint.hpp>
#include <IO/Stream.hpp>

namespace Utl
{
    void PushRegister(Library::IO::BufferStream & stream, Library::Debug::RegisterInfo const & info)
    {
        int count = 32;
        stream << info.pc << info.dar;
        for(int i = 0; i < count; i++) stream << info.gpr[i];
        for(int i = 0; i < count; i++) stream << info.fpr[i];
        stream << info.cr << info.lr << info.ctr;
    }
}
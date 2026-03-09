#include <IO/Stream.hpp>
#include "BreakInfoModel.hpp"

namespace Utl
{
    void PullRegister(Library::IO::BufferStream & stream, RegisterInfo & info)
    {
        int count = 32;
        stream >> info.pc >> info.dar;
        for(int i = 0; i < count; i++) stream >> info.gpr[i];
        for(int i = 0; i < count; i++) stream >> info.fpr[i];
        stream >> info.cr >> info.lr >> info.ctr;
    }
}

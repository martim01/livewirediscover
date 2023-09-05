#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "livewirenamedtype.h"
#include <map>

namespace pml
{
    namespace livewire
    {
        struct source
        {
            std::string sName;
            unsigned short nStream=0;
        };

        struct message
        {
            IpAddress ip;
            std::map<std::string, source> mSources;
        };
    }
}

#include "livewirehandler.h"
#include "livewireparser.h"
#include "log.h"

using namespace pml::livewire;

void StdHandler::Message(const message& aMessage)
{
    pmlLog() << aMessage.ip;
    for(const auto& [src, details] : aMessage.mSources)
    {
        pmlLog() << src << "\t" << details.sName << "\t" << details.nStream;
    }
}

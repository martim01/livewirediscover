#include "livewirehandler.h"
#include "livewireparser.h"
#include "log.h"

using namespace pml::livewire;

void StdHandler::Message(const message& aMessage)
{
    pmlLog(pml::LOG_INFO, "pml::livewire") << aMessage.ip;
    for(const auto& [src, details] : aMessage.mSources)
    {
        pmlLog(pml::LOG_INFO, "pml::livewire") << src << "\t" << details.sName << "\t" << details.nStream;
    }
}

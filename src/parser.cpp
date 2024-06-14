#include "livewireparser.h"
#include <algorithm>
#include "livewirehandler.h"
#include "log.h"
#include "livewirenamedtype.h"

using namespace pml::livewire;


void Parser::ParseMessage(const std::string& sSenderIp, std::vector<unsigned char> vMessage)
{
    auto theMessage = ParseMessage(vMessage);
    if(theMessage)
    {
        m_pHandler->Message(*theMessage);
    }
}

std::optional<pml::livewire::message> Parser::ParseMessage(std::vector<unsigned char> vMessage)
{
    if(vMessage.size() < 0x81)
    {
        pmlLog(LOG_ERROR, "pml::livewire") << "Parser message to short";
        return {};
    }

    message aMessage; 
    pmlLog(pml::LOG_TRACE, "pml::livewire") << "Message length: " << vMessage.size();
    //0x45-0x48 are Ip adresss
    aMessage.ip = IpAddress(std::to_string(vMessage[0x45])+"."+std::to_string(vMessage[0x46])+"."+std::to_string(vMessage[0x47])+"."+std::to_string(vMessage[0x48]));

    //7E-81 is S001
    size_t nByte = 0x7E;
    
    while(vMessage[nByte] == 'S' && nByte+4 < vMessage.size())
    {
        pmlLog(pml::LOG_TRACE, "pml::livewire") << "@ " << nByte;

        source aSource;
        auto sSrc = std::string(vMessage.begin()+nByte, vMessage.begin()+nByte+4);

        pmlLog(pml::LOG_TRACE, "pml::livewire") << "Src= " << sSrc;

        nByte += 0x14;
        if(nByte+1 >= vMessage.size())
        {
            pmlLog(pml::LOG_TRACE, "pml::livewire") << "End of message";    
            break;
        }
        
        aSource.nStream = (static_cast<int>(vMessage[nByte]) << 8) + static_cast<int>(vMessage[nByte+1]);
        pmlLog(pml::LOG_TRACE, "pml::livewire") "Stream= " << aSource.nStream;

        nByte += 0x47;
        if(nByte >= vMessage.size())
        {
            pmlLog(pml::LOG_TRACE, "pml::livewire") << "End of message";
            break;
        }
            
        auto nLength = vMessage[nByte];
        
        if(nByte+nLength >= vMessage.size())
        {
            pmlLog(pml::LOG_TRACE, "pml::livewire") "End of message";
            break;
        }
        nByte++;        
        aSource.sName = std::string(vMessage.begin()+nByte, vMessage.begin()+nByte+nLength);

        pmlLog(pml::LOG_TRACE, "pml::livewire") << "Name= " << aSource.sName;

        aMessage.mSources.try_emplace(sSrc, aSource);
        
        nByte+=nLength;
        //nByte++;
    }
    return aMessage;
}

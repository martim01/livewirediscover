#pragma once
#include <vector>
#include <memory>
#include <list>
#include "livewiremessage.h"
#include <string>
#include <optional>

namespace pml
{
    namespace livewire
    {
        class Handler;

        class Parser
        {
            public:
                Parser(std::shared_ptr<Handler> pHandler) : m_pHandler(pHandler){}

                void ParseMessage(const std::string& sSenderIp, std::vector<unsigned char> vMessage);

                static std::optional<message> ParseMessage(std::vector<unsigned char> vMessage);

            protected:
                std::shared_ptr<Handler> m_pHandler = nullptr;
        };
    }
}


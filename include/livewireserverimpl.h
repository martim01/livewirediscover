#pragma once
#include "asio.hpp"
#include "livewirenamedtype.h"
#include <map>
#include <list>
#include "livewirereceiver.h"

namespace pml
{
    namespace livewire
    {
        class Handler;

        class ServerImpl
        {
            public:
                ServerImpl(std::shared_ptr<Handler> pHandler, bool bThreaded);
                ~ServerImpl();
                void Run();


                void Stop();
                bool IsStopped();

            private:
                void RunContext();
                asio::io_context m_context;
                unsigned long m_nMessageVersion;

                std::unique_ptr<Receiver> m_pReceiver = nullptr;

                std::shared_ptr<Handler> m_pHandler;
                bool m_bThreaded;
                static const uint16_t LIVEWIRE_PORT = 4001;

                std::unique_ptr<std::thread> m_pThread = nullptr;
            };
    }
};


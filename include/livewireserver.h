#pragma once
#include "livewiredlldefine.h"
#include "livewirenamedtype.h"
#include <memory>
#include <chrono>

namespace pml
{
    namespace livewire
    {
        class Handler;
        class ServerImpl;

        class LIVEWIRE_EXPORT Server
        {
            public:
                Server(std::shared_ptr<Handler> pHandler, bool bThreaded=true);
                ~Server();
                void Run();

                void Stop();
                bool IsStopped();

            private:
                std::unique_ptr<ServerImpl> m_pImpl;
            };
    }
};

#include "livewireserverimpl.h"
#include "livewireparser.h"
#include "livewirehandler.h"
#include "livewirereceiver.h"
#include "log.h"

using namespace pml::livewire;

ServerImpl::ServerImpl(std::shared_ptr<Handler> pHandler, bool bThreaded) : m_pHandler(pHandler),
m_bThreaded(bThreaded)
{

}

ServerImpl::~ServerImpl()
{
    Stop();
}


void ServerImpl::Run()
{
    m_pReceiver = std::make_unique<Receiver>(m_context, std::make_shared<Parser>(m_pHandler));
    if(m_context.stopped())
    {
        pmlLog(LOG_INFO) << "LivewireServer\t" << "Restart context";
        m_context.restart();
    }

    if(m_bThreaded && m_pThread == nullptr)
    {
        m_pThread = std::make_unique<std::thread>([this]() {    RunContext();  });
    }
    else
    {
        m_pThread = nullptr;
        RunContext();
    }
}


void ServerImpl::RunContext()
{
    try
    {
        m_pReceiver->Run(asio::ip::make_address("0.0.0.0"), asio::ip::make_address("239.192.255.3"), LIVEWIRE_PORT);

        auto work = asio::require(m_context.get_executor(), asio::execution::outstanding_work.tracked);
        pmlLog(LOG_INFO) << "LivewireServer\t" << "Run context";
        m_context.run();
        pmlLog(pml::LOG_DEBUG) << "LivewireServer\tContext stopped";
    }
    catch (const std::exception& e)
    {
        pmlLog(LOG_ERROR) << "LivewireServer\t Failed to run context: " << e.what();
    }
}
void ServerImpl::Stop()
{
    pmlLog(LOG_INFO) << "LivewireServer\t" << "Stop context";
    m_context.stop();
    if(m_pThread)
    {
        m_pThread->join();
    }
    m_pThread = nullptr;
}

bool ServerImpl::IsStopped()
{
    return m_context.stopped();
}


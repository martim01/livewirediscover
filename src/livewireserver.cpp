#include "livewireserver.h"
#include "livewireserverimpl.h"


using namespace pml::livewire;
Server::~Server()=default;

Server::Server(std::shared_ptr<Handler> pHandler, bool bThreaded) :
    m_pImpl(std::make_unique<ServerImpl>(pHandler, bThreaded))
{

}



void Server::Run()
{
    m_pImpl->Run();
}


void Server::Stop()
{
    m_pImpl->Stop();
}

bool Server::IsStopped()
{
    return m_pImpl->IsStopped();
}



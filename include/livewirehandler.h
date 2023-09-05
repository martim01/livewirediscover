#pragma once
#include "livewiredlldefine.h"

namespace pml
{
    namespace livewire
    {
           
        struct message;

        class LIVEWIRE_EXPORT Handler
        {
            public:

                virtual void Message(const message& aMessage)=0;
        };

        class LIVEWIRE_EXPORT StdHandler : public Handler
        {
            public:
                void Message(const message& aMessage) override;
        };
    } // namespace livewire
    
}

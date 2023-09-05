#pragma once

#ifdef __WIN32__
    #ifdef LIVEWIRE_DLL
        #define LIVEWIRE_EXPORT __declspec(dllexport)
    #else
        #define LIVEWIRE_EXPORT __declspec(dllimport)
    #endif // DNSSD_EXPORT
#else
#define LIVEWIRE_EXPORT
#endif


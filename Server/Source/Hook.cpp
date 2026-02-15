#include <Plugin.hpp>
#include <Debug.hpp>

#include <notifications/notifications.h>
#include <whb/log_udp.h>

#include <memory>

#include "Quantite.hpp"
#include "IPInfo.hpp"

static std::unique_ptr<Quantite> quantite;
static std::unique_ptr<IPInfo> ipInfo;

ON_APP_START()
{
    NotificationModule_InitLibrary();
    Library::Debug::Initialize();
    ipInfo = std::make_unique<IPInfo>();
    quantite = std::make_unique<Quantite>(ipInfo.get());
}

ON_APP_END()
{
    quantite.reset();
    ipInfo.reset();
    Library::Debug::Shutdown();
    NotificationModule_DeInitLibrary();
}
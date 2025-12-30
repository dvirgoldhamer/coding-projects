#define DLL_IMPLEMENTOR

#include "handleton.hpp"
#include "factory.hpp"
#include "thread_pool.hpp"
#include "scheduler.hpp"
#include "logger.hpp"
#include "framework.hpp"
#include "raid_manager.hpp"
#include "file_manager.hpp"
#include "response_manager.hpp"
#include "messages.hpp"
#include "nbd_proxy.hpp"

using factory = ilrd::factory_details::Factory<int, ilrd::interfaces::ICommand>;
using message_factory = ilrd::factory_details::Factory<int, ilrd::MessageBase>;

INSTANTIATE_HANDLETON(factory)
INSTANTIATE_HANDLETON(ilrd::Scheduler)
INSTANTIATE_HANDLETON(ilrd::Logger)
INSTANTIATE_HANDLETON(ilrd::ThreadPool)
INSTANTIATE_HANDLETON(ilrd::RAIDManager)
INSTANTIATE_HANDLETON(message_factory)
INSTANTIATE_HANDLETON(ilrd::FileManager)
INSTANTIATE_HANDLETON(ilrd::ResponseManager)
INSTANTIATE_HANDLETON(ilrd::NBDHandler)



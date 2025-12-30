/*name: Dvir Goldhamer

File Name: response_manager.hpp

purpose: API for ResponseProxy

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_RESPONSE_MANAGER_HPP
#define IRLD_RD168_169_RESPONSE_MANAGER_HPP

#include <unordered_map>
#include <unordered_set>
#include <mutex>

#include "framework_interfaces.hpp"
#include "udp_client_server.hpp"
#include "key_args.hpp"
#include "ilrd_uid.hpp"
#include "concrete_utils.hpp"

namespace ilrd
{

class ResponseManager 
{
public:

    std::shared_ptr<interfaces::IKeyArgs> GetKeyArgs(int fd, ilrd::Mode mode);

    void AddCommand(unsigned long long nbdID, UID uid);
    void UpdateStatus(UID uid, STATUS status, std::shared_ptr<char[]> buffer = nullptr, uint64_t buffer_size = 0);

    STATUS GetCommandStatus(UID uid);
    void DeleteCommand(UID uid);

    ResponseManager(const ResponseManager& other) = delete;
    ResponseManager& operator=(const ResponseManager& other) = delete;

private:
    explicit ResponseManager() = default;
    virtual ~ResponseManager() = default;

    struct CommandItem
    {
        unsigned long long nbd_id;
        STATUS status;
        std::shared_ptr<char[]> buffer;
    };

    std::unordered_map<UID, CommandItem> m_commands;
    std::unordered_set<unsigned long long> m_singleCommands;

    std::mutex m_mtx;

    friend class Handleton<ResponseManager>;
};

} //namespace ilrd

#endif //IRLD_RD168_169_RESPONSE_MANAGER_HPP
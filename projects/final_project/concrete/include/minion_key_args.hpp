/*name: Dvir Goldhamer

File Name: key_args.hpp

purpose: API for read and write args

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_MINION_KEY_ARGS_HPP
#define IRLD_RD168_169_MINION_KEY_ARGS_HPP

#include <fstream>

#include "framework_interfaces.hpp"
#include "raid_manager.hpp"
#include "ilrd_uid.hpp"

namespace ilrd
{
class MinionReadArgs : public interfaces::IKeyArgs
{
public:
    MinionReadArgs(int how_many , int offset, UID uid) : m_how_many(how_many), m_offset(offset), m_uid(uid)
    {
    }

    void SetOffset(int offset) { m_offset = offset; }
    UID GetUID() const{ return m_uid; }
    int GetHowMany() const { return m_how_many; }
    int GetOffset() const { return m_offset; }
    int GetKey() {return (READ);};

private:
    int m_how_many;
    int m_offset;
    UID m_uid;

};

class MinionWriteArgs : public interfaces::IKeyArgs
{
public:
    MinionWriteArgs(int how_many,int offset, UID uid, std::shared_ptr<char[]> buffer) : m_how_many(how_many), m_offset(offset), m_uid(uid), m_buffer(buffer) {}
    
    int GetHowMany() const{ return m_how_many; }
    UID GetUID() const{ return m_uid; }
    int GetOffset() const { return m_offset; }
    std::shared_ptr<char[]> GetBuffer() { return m_buffer;}
    int GetKey() {return (WRITE);};
    void SetOffset(int offset) { m_offset = offset; }

private:
    int m_how_many;
    int m_offset;
    UID m_uid;

    std::shared_ptr<char[]> m_buffer;
};

} //namespace ilrd

#endif //IRLD_RD168_169_MINION_KEY_ARGS_HPP


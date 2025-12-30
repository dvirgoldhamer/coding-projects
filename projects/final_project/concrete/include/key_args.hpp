/*name: Dvir Goldhamer

File Name: key_args.hpp

purpose: API for read and write args

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_KEY_ARGS_HPP
#define IRLD_RD168_169_KEY_ARGS_HPP

#include "framework_interfaces.hpp"
#include "raid_manager.hpp"

namespace ilrd
{
class ReadArgs : public interfaces::IKeyArgs
{
public:
    ReadArgs(int how_many , int offset, unsigned long long id) : m_how_many(how_many), m_offset(offset), m_nbdID(id)
    {
    }

    inline void SetOffset(int offset) { m_offset = offset; }
    inline int GetHowMany() const { return m_how_many; }
    inline int GetOffset() const { return m_offset; }
    inline int GetKey() {return (READ);};
    inline unsigned long long GetID() {return m_nbdID;};

private:
    
    int m_how_many;
    int m_offset;
    unsigned long long m_nbdID;
};

class WriteArgs : public interfaces::IKeyArgs
{
public:
    WriteArgs(int how_many,int offset, std::shared_ptr<char[]> buffer,   unsigned long long id) : m_how_many(how_many), m_offset(offset), m_buffer(buffer), m_nbdID(id) {}
    
    inline int GetHowMany() const{ return m_how_many; }
    inline int GetOffset() const { return m_offset; }
    inline std::shared_ptr<char[]> GetBuffer() { return m_buffer;}
    inline int GetKey() {return (WRITE);};
    inline void SetOffset(int offset) { m_offset = offset; }
    inline unsigned long long GetID() {return m_nbdID;};

private:
    int m_how_many;
    int m_offset;
    std::shared_ptr<char[]> m_buffer;
    unsigned long long m_nbdID;

};

} //namespace ilrd

#endif //IRLD_RD168_169_KEY_ARGS_HPP

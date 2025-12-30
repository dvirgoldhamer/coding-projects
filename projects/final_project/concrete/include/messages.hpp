/*name: Dvir Goldhamer

File Name: messages.hpp

purpose: API for network messages

reviewed by 

Date: */

#ifndef IRLD_RD168_169_MESSAGES_HPP
#define IRLD_RD168_169_MESSAGES_HPP

#include <cstdint>
#include <iostream>
#include <memory>

#include "concrete_utils.hpp"
#include "ilrd_uid.hpp"

namespace ilrd
{

enum KEY
{
    READ_MESSAGE,
    WRITE_MESSAGE,
    READ_RESPONSE_MESSAGE,
    WRITE_RESPONSE_MESSAGE,
};

class MessageBase
{
public:

    MessageBase(uint64_t bufferLength, const KEY& objectKey);
    MessageBase() = default;

    virtual char* ToBuffer(char* buffer);
    virtual char* FromBuffer(char* buffer);
    virtual uint64_t GetBufferLength();
    virtual ~MessageBase() = 0;

    inline KEY GetKey() const
    {
        return m_objectKey;
    }

    inline void SetUID(UID uid)
    {
        m_uid = uid;
    }

    inline UID GetUID() const
    {
        return m_uid;
    }

    inline uint64_t GetMessageLength() const
    {
        return m_bufferLength;
    }

    
private:
    uint64_t m_bufferLength;
    KEY m_objectKey;
    
protected:
    UID m_uid;
};

class WriteMessage : public MessageBase
{
public:

    WriteMessage(uint64_t offset, uint64_t how_many, std::shared_ptr<char[]> buffer);
    WriteMessage() = default;

    virtual char* ToBuffer(char* buffer);
    virtual char* FromBuffer(char* buffer);
    uint64_t GetBufferLength();

    static inline std::shared_ptr<MessageBase> CreateWriteMessage();

    inline uint64_t GetOffset() const
    {
        return m_offset;
    }

    inline uint64_t GetHowMany() const
    {
        return m_how_many;
    }

    inline std::shared_ptr<char[]> GetBuffer() const
    {
        return m_buffer;
    }

private:
    uint64_t m_offset;
    uint64_t m_how_many;
    std::shared_ptr<char[]> m_buffer;
};

std::shared_ptr<MessageBase> WriteMessage::CreateWriteMessage()
{
    return std::make_shared<WriteMessage>();
}

class ReadMessage : public MessageBase
{
public:

    ReadMessage(uint64_t offset, uint64_t how_many);
    ReadMessage() = default;

    virtual char* ToBuffer(char* buffer);
    virtual char* FromBuffer(char* buffer);
    uint64_t GetBufferLength();

    static inline std::shared_ptr<MessageBase> CreateReadMessage();

    inline uint64_t GetOffset() const
    {
        return m_offset;
    }

    inline uint64_t GetHowMany() const
    {
        return m_how_many;
    }

private:
    uint64_t m_offset;
    uint64_t m_how_many;
};

std::shared_ptr<MessageBase> ReadMessage::CreateReadMessage()
{
    return std::make_shared<ReadMessage>();
}

class ReadResponseMessage : public MessageBase
{
public:

    ReadResponseMessage(std::shared_ptr<char[]> buffer, uint64_t buffer_size, STATUS status, UID uid);
    ReadResponseMessage() = default;

    virtual char* ToBuffer(char* buffer);
    virtual char* FromBuffer(char* buffer);
    uint64_t GetBufferLength();

    static inline std::shared_ptr<MessageBase> CreateReadResponseMessage();

    inline STATUS GetStatus() const
    {
        return m_status;
    }

    inline uint64_t GetBufferSize() const
    {
        return m_buffer_size;
    }

    inline std::shared_ptr<char[]> GetBuffer() const
    {
        return m_buffer;
    }

private:

    std::shared_ptr<char[]> m_buffer;
    uint64_t m_buffer_size;
    STATUS m_status;
};

std::shared_ptr<MessageBase> ReadResponseMessage::CreateReadResponseMessage()
{
    return std::make_shared<ReadResponseMessage>();
}

class WriteResponseMessage : public MessageBase
{
public:

    WriteResponseMessage(STATUS status, UID uid);
    WriteResponseMessage() = default;

    virtual char* ToBuffer(char* buffer);
    virtual char* FromBuffer(char* buffer);
    uint64_t GetBufferLength();

    static inline std::shared_ptr<MessageBase> CreateWriteResponseMessage();

    inline STATUS GetStatus() const
    {
        return m_status;
    }

private:

    STATUS m_status;
};

std::shared_ptr<MessageBase> WriteResponseMessage::CreateWriteResponseMessage()
{
    return std::make_shared<WriteResponseMessage>();
}

} //namespace ilrd
#endif //IRLD_RD168_169__MESSAGES_HPP
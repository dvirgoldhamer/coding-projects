/*name: Dvir Goldhamer

File Name: messages.cpp

purpose: Implementation for a messages classes

reviewed by :

Date: */

#include <cstring>

#include "messages.hpp"

namespace ilrd
{

/*******************************BaseMessage*********************************/

MessageBase::MessageBase(uint64_t bufferLength, const KEY& objectKey) : m_bufferLength(bufferLength), m_objectKey(objectKey), m_uid()
{
}

MessageBase::~MessageBase()
{
}

uint64_t MessageBase::GetBufferLength()
{
    return (sizeof(m_bufferLength) + sizeof(m_objectKey) + m_uid.GetBufferLength());
}

char* MessageBase::ToBuffer(char* buffer)
{    
    *(uint64_t*)buffer = m_bufferLength;
    buffer += sizeof(uint64_t);
    
    memcpy(buffer, &m_objectKey, sizeof(KEY));
    buffer += sizeof(KEY);
    
    buffer = m_uid.ToBuffer(buffer);
    
    return buffer; 
}

char* MessageBase::FromBuffer(char* buffer)
{
    m_bufferLength = *(uint64_t*)buffer;
    buffer += sizeof(m_bufferLength);

    memcpy(&m_objectKey, buffer, sizeof(KEY));
    buffer += sizeof(KEY);

    buffer = m_uid.FromBuffer(buffer);

    return buffer; 
}

/*******************************WriteMessage*********************************/

WriteMessage::WriteMessage(uint64_t offset, uint64_t how_many, std::shared_ptr<char[]> buffer) : MessageBase(MessageBase::GetBufferLength() + sizeof(uint64_t) + sizeof(uint64_t) + how_many, WRITE_MESSAGE), m_offset(offset), m_how_many(how_many), m_buffer(buffer)
{
}   

uint64_t WriteMessage::GetBufferLength()
{
    return (MessageBase::GetBufferLength() + 
           sizeof(uint64_t) + 
           sizeof(uint64_t) + m_how_many);
}

char* WriteMessage::ToBuffer(char* buffer)
{
    buffer = MessageBase::ToBuffer(buffer);
    
    *(uint64_t*)buffer = m_offset;
    buffer += sizeof(uint64_t);
    *(uint64_t*)buffer = m_how_many;
    buffer += sizeof(uint64_t);

    memcpy(buffer, m_buffer.get(), m_how_many);
    buffer+= m_how_many;

    return buffer;
}

char* WriteMessage::FromBuffer(char* buffer)
{
    buffer = MessageBase::FromBuffer(buffer);
    
    m_offset = *(uint64_t*)buffer;
    buffer += sizeof(uint64_t);
    m_how_many = *(uint64_t*)buffer;
    buffer += sizeof(uint64_t);
    
    m_buffer = std::make_shared<char[]>(m_how_many);
    memcpy(m_buffer.get(), buffer, m_how_many);
    buffer+= m_how_many;
    
    return buffer;
}

/*******************************ReadMessage*********************************/

ReadMessage::ReadMessage(uint64_t offset, uint64_t how_many) :  MessageBase(this->GetBufferLength(),READ_MESSAGE), m_offset(offset), m_how_many(how_many)
{
}

uint64_t ReadMessage::GetBufferLength()
{
    return (MessageBase::GetBufferLength() + 
           sizeof(uint64_t) + 
           sizeof(uint64_t));
}

char* ReadMessage::ToBuffer(char* buffer)
{
    buffer = MessageBase::ToBuffer(buffer);
    
    *(uint64_t*)buffer = m_offset;
    buffer += sizeof(uint64_t);
    *(uint64_t*)buffer = m_how_many;
    buffer += sizeof(uint64_t);
    
    return buffer;
}

char* ReadMessage::FromBuffer(char* buffer)
{
    buffer = MessageBase::FromBuffer(buffer);
    
    m_offset = *(uint64_t*)buffer;
    buffer += sizeof(uint64_t);
    m_how_many = *(uint64_t*)buffer;
    buffer += sizeof(uint64_t);    
   
    return buffer;
}

/****************************ReadResponseMessage*******************************/

ReadResponseMessage::ReadResponseMessage(std::shared_ptr<char[]> buffer, uint64_t buffer_size, STATUS status, UID uid) : MessageBase(MessageBase::GetBufferLength() + sizeof(uint64_t) + sizeof(STATUS) + buffer_size, READ_RESPONSE_MESSAGE), m_buffer(buffer), m_buffer_size(buffer_size), m_status(status)
{
    SetUID(uid);
}

uint64_t ReadResponseMessage::GetBufferLength()
{
    return (MessageBase::GetBufferLength() + 
           sizeof(uint64_t) + 
           sizeof(STATUS)) + m_buffer_size;
} 

char* ReadResponseMessage::ToBuffer(char* buffer)
{
    buffer = MessageBase::ToBuffer(buffer);
    
    *(STATUS*)buffer = m_status;
    buffer += sizeof(STATUS);
    *(uint64_t*)buffer = m_buffer_size;
    buffer += sizeof(uint64_t);

    memcpy(buffer, m_buffer.get(), m_buffer_size);
    buffer+= m_buffer_size;

    return buffer;
}

char* ReadResponseMessage::FromBuffer(char* buffer)
{
    buffer = MessageBase::FromBuffer(buffer);
    
    m_status = *(STATUS*)buffer;
    buffer += sizeof(STATUS);
    m_buffer_size = *(uint64_t*)buffer;
    buffer += sizeof(uint64_t);
    
    m_buffer = std::make_shared<char[]>(m_buffer_size);
    memcpy(m_buffer.get(), buffer, m_buffer_size);
    buffer+= m_buffer_size;
    
    return buffer;
}

/***************************WriteResponseMessage*******************************/

WriteResponseMessage::WriteResponseMessage(STATUS status, UID uid) : MessageBase(this->GetBufferLength(), WRITE_RESPONSE_MESSAGE), m_status(status)
{
    SetUID(uid);
}

uint64_t WriteResponseMessage::GetBufferLength()
{
    return (MessageBase::GetBufferLength() + sizeof(STATUS));
} 

char* WriteResponseMessage::ToBuffer(char* buffer)
{
    buffer = MessageBase::ToBuffer(buffer);
    
    *(STATUS*)buffer = m_status;
    buffer += sizeof(STATUS);

    return buffer;
}

char* WriteResponseMessage::FromBuffer(char* buffer)
{
    buffer = MessageBase::FromBuffer(buffer);
    
    m_status = *(STATUS*)buffer;
    buffer += sizeof(STATUS);
    
    return buffer;
}

} //namespace ilrd
/*name: Dvir Goldhamer

File Name: input_mediator.hpp

purpose: API for a input mediator

reviewed by Denis Charenko: 

Date: 17.08.25*/

#ifndef ILRD_RD168_169_INPUT_MEDIATOR_HPP
#define ILRD_RD168_169_INPUT_MEDIATOR_HPP

#include "framework_interfaces.hpp"

namespace ilrd
{

class InputMediator
{
public:
    void Run(int fd, Mode mode, std::shared_ptr<interfaces::IInputProxy> input_proxy);
};

}//namespace ilrd

#endif //ILRD_RD168_169_INPUT_MEDIATOR_HPP
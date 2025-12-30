#include <cassert>
#include <iostream>
#include "factory.hpp"

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

void TestFactoryNoArgs();
void TestFactoryWithArgs();
void TestFactoryInvalidKey();

class Animal
{
public:
    virtual ~Animal() = default;
    virtual std::string Speak() const = 0;
};

class Dog : public Animal
{
public:
    std::string Speak() const override { return "Woof"; }
};

class Cat : public Animal
{
public:
    Cat(std::string name, int age) : m_name(name), m_age(age) {}
    std::string Speak() const override { return m_name + " says Meow at age " + std::to_string(m_age); }

private:
    std::string m_name;
    int m_age;
};

int result = 0;

int main()
{
    TestFactoryNoArgs();
    TestFactoryWithArgs();
    TestFactoryInvalidKey();

    if(0 == result)
    {
        std::cout << GREEN << "All tests passed." << DEFAULT << std::endl;
    }

    return 0;
}

void TestFactoryNoArgs()
{
    ilrd::factory_details::Factory<std::string, Animal> animalFactory;

    animalFactory.Register("dog", []() -> Animal* { return new Dog(); });

    std::shared_ptr<Animal> dog = animalFactory.CreateObject("dog");

    if(dog->Speak() != "Woof")
    {
        std::cout << RED << "TestFactoryNoArgs Failed." << DEFAULT << std::endl;
        result = 1;
    }
}

void TestFactoryWithArgs()
{
    ilrd::factory_details::Factory<std::string, Animal, std::string, int> animalFactory;

    animalFactory.Register("cat", [](std::string name, int age) -> Animal* {
        return new Cat(name, age);
    });

    std::shared_ptr<Animal> cat = animalFactory.CreateObject("cat", "yossi", 3);

    if(cat->Speak() != "yossi says Meow at age 3")
    {
        std::cout << RED << "TestFactoryWithArgs Failed." << DEFAULT << std::endl;
        result = 1;
    }
}

void TestFactoryInvalidKey()
{
    ilrd::factory_details::Factory<std::string, Animal> animalFactory;

    try
    {
        animalFactory.CreateObject("ghost");
        std::cout << RED << "TestFactoryInvalidKey Failed." << DEFAULT << std::endl; // should throw exception before
        result = 1;
    }
    catch (const ilrd::factory_details::InvalidKey& e)
    {
        std::cout <<e.what() << std::endl;
    }
}


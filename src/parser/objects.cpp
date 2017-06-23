#include "objects.hpp"

#include <utility>

namespace nyaa {

Nyaason::Nyaason()
    : _object(std::make_unique<Empty>())
{ }

Nyaason::Nyaason(Object* ptr)
    : _object(ptr)
{ }

Nyaason::Nyaason(const Nyaason& other)
    : _object(std::make_unique<Object>(*other._object))
{ }

Nyaason::Nyaason(Nyaason&& other) noexcept
    : _object(std::move(other._object))
{ }

Nyaason& Nyaason::operator=(const Nyaason& other)
{
    _object = std::make_unique<Object>(*other._object);
    return *this;
}

Nyaason& Nyaason::operator=(Nyaason&& other) noexcept
{
    _object = std::move(other._object);
    return *this;
}

Nyaason& Dictionary::operator[](const std::string& key)
{
    return _dictionary[key];
}

Nyaason& Dictionary::operator[](std::string&& key)
{
    return _dictionary[std::move(key)];
}

Nyaason& Dictionary::at(const std::string& key)
{
    return _dictionary.at(key);
}

const Nyaason& Dictionary::at(const std::string& key) const
{
    return _dictionary.at(key);
}

const std::string& Structure::name() const
{
    return _name;
}

Nyaason& Structure::operator[](const std::string& key)
{
    return _dictionary[key];
}

Nyaason& Structure::operator[](std::string&& key)
{
    return _dictionary[std::move(key)];
}

Nyaason& Structure::at(const std::string& key)
{
    return _dictionary.at(key);
}

const Nyaason& Structure::at(const std::string& key) const
{
    return _dictionary.at(key);
}

String::String(const std::string& string)
    : _string(string)
{ }

String::String(std::string&& string)
    : _string(std::move(string))
{ }

} // namespace nyaa
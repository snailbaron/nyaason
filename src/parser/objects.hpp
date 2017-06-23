#pragma once

#include <string>
#include <map>
#include <memory>

namespace nyaa {

struct Object {};

struct Nyaason {
public:
    Nyaason();
    Nyaason(Object* ptr);

    Nyaason(const Nyaason& other);
    Nyaason(Nyaason&& other) noexcept;

    Nyaason& operator=(const Nyaason& other);
    Nyaason& operator=(Nyaason&& other) noexcept;

private:
    std::unique_ptr<Object> _object;
};

class Empty : public Object {
};

class Dictionary : public Object {
public:
    Nyaason& operator[](const std::string& key);
    Nyaason& operator[](std::string&& key);

    Nyaason& at(const std::string& key);
    const Nyaason& at(const std::string& key) const;

private:
    std::map<std::string, Nyaason> _dictionary;
};

class Structure : public Object {
public:
    const std::string& name() const;

    Nyaason& operator[](const std::string& key);
    Nyaason& operator[](std::string&& key);

    Nyaason& at(const std::string& key);
    const Nyaason& at(const std::string& key) const;

private:
    std::string _name;
    Dictionary _dictionary;    
};

class Number : public Object {
public:
    explicit Number(double value) : _value(value) {}
    operator double() const { return _value; }

private:
    double _value;
};

class String : public Object {
public:
    String(const std::string& string);
    String(std::string&& string);

private:
    std::string _string;
};

} // namespace nyaa
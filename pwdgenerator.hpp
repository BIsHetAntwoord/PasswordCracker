#ifndef PWDGENERATOR_HPP_INCLUDED
#define PWDGENERATOR_HPP_INCLUDED

#include <string>
#include <vector>
#include <cstddef>

#include <boost/asio/io_service.hpp>
#include <boost/atomic.hpp>

class Dictionary;
class PasswordConstraints;
class DictionaryElementPart;
class ThreadSafeString;
class DictionaryElement;

typedef int(*password_callback)(void*, const std::string&);

const int PWD_CALLBACK_FOUND = 0;
const int PWD_CALLBACK_NOT_FOUND = 1;
const int PWD_CALLBACK_ERROR = 2;

class PasswordGenerator
{
    private:
        const Dictionary& dict;
        const PasswordConstraints& constraints;
        password_callback callback;
        void* userdata;

        void check_password(boost::asio::io_service&, std::vector<DictionaryElementPart>,
                            size_t dictionary_offset, ThreadSafeString&, boost::atomic<int>&,
                            boost::asio::io_service::work);
        void check_sequence(const std::string&, const std::vector<DictionaryElementPart>&, size_t,
                            boost::asio::io_service&, ThreadSafeString&, boost::atomic<int>&);
    public:
        PasswordGenerator(const Dictionary&, const PasswordConstraints&, password_callback, void* userdata);
        ~PasswordGenerator();

        int generate(std::string&, size_t);
};

#endif // PWDGENERATOR_HPP_INCLUDED

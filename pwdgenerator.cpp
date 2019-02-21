#include "pwdgenerator.hpp"
#include "constraints.hpp"
#include "dictionary.hpp"
#include "threadsafestring.hpp"
#include "containerutils.hpp"
#include "syncedstream.hpp"

#include <boost/bind.hpp>

//To hide some warnings
#define BOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN
#include <boost/thread/thread.hpp>
#undef BOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN

#include <algorithm>

PasswordGenerator::PasswordGenerator(const Dictionary& dict, const PasswordConstraints& constraints, password_callback callback,
                                     void* userdata) :
    dict(dict), constraints(constraints), callback(callback), userdata(userdata) {}

PasswordGenerator::~PasswordGenerator() {}

int PasswordGenerator::generate(std::string& result, size_t thread_count)
{
    boost::asio::io_service service;
    boost::thread_group threadpool;
    ThreadSafeString result_safe;
    boost::atomic<int> result_status(PWD_CALLBACK_NOT_FOUND);

    {
        boost::asio::io_service::work work(service);

        for(size_t i = 0; i < thread_count; ++i)
        {
            threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &service));
        }


        service.post(boost::bind(PasswordGenerator::check_password, this, boost::ref(service),
                                 std::vector<DictionaryElementPart>(), 0,
                                 boost::ref(result_safe), boost::ref(result_status),
                                 work));
    }
    threadpool.join_all();

    result = result_safe.to_stdstr();

    return result_status;
}

void PasswordGenerator::check_password(boost::asio::io_service& service, std::vector<DictionaryElementPart> parts,
                                       size_t dictionary_offset, ThreadSafeString& result, boost::atomic<int>& status,
                                       boost::asio::io_service::work work)
{
    //Stop once found or error
    if(status != PWD_CALLBACK_NOT_FOUND)
        return;

    //Check all passwords with these elements
    if(this->constraints.is_possible(parts))
    {
        do
        {
            this->check_sequence("", parts, 0, service, result, status);
        } while(status == PWD_CALLBACK_NOT_FOUND && std::next_permutation(parts.begin(), parts.end()));
    }

    //Check if this should continue
    if(this->constraints.should_continue(parts))
    {
        for(size_t i = dictionary_offset; i < this->dict.size(); ++i)
        {
            const DictionaryElement* element = this->dict.get_element(i);

            for(const std::vector<DictionaryElementPart>& element_parts : *element)
            {
                std::vector<DictionaryElementPart> new_parts = parts;
                for(const DictionaryElementPart& new_part : element_parts)
                {
                    new_parts.insert(std::upper_bound(new_parts.begin(), new_parts.end(), new_part), new_part);
                }
                service.post(boost::bind(PasswordGenerator::check_password, this, boost::ref(service),
                                         new_parts, i + 1, boost::ref(result), boost::ref(status), work));
            }
        }
    }
}

void PasswordGenerator::check_sequence(const std::string& base, const std::vector<DictionaryElementPart>& parts, size_t index, boost::asio::io_service& service,
                                       ThreadSafeString& result, boost::atomic<int>& status)
{
    if(index == parts.size())
    {
        if(this->constraints.is_valid(base))
        {
            int callback_status = callback(this->userdata, base);
            if(callback_status == PWD_CALLBACK_FOUND || callback_status == PWD_CALLBACK_ERROR)
            {
                service.stop();
                status = callback_status;
                result = base;
            }
        }
        return;
    }

    for(size_t i = 0; i < parts[index].alias_count(); ++i)
    {
        std::string new_base = base + parts[index].alias(i);
        if(this->constraints.should_continue(new_base, parts, index  + 1))
            check_sequence(new_base, parts, index + 1, service, result, status);
    }
}

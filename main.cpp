#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>

#include <memory>

#include "dictionary.hpp"
#include "constraints.hpp"
#include "pwdgenerator.hpp"
#include "debug.hpp"
#include "luapwddata.hpp"
#include "luadictdata.hpp"
#include "luaconstrdata.hpp"
#include "syncedstream.hpp"
#include "pathutils.hpp"

int pwd_callback(void* userdata, const std::string& name)
{
    try
    {
        LuaPasswordData* lua_data = (LuaPasswordData*)userdata;
        return lua_data->run_callback(name);
    }
    catch(const LuaException& except)
    {
        sync_cout << "Lua error: " << except.what() << std::endl;
        return PWD_CALLBACK_ERROR; //Stop all threads
    }
}

int main(int argc, char* argv[])
{
    try
    {
        unsigned int threads;
        std::string dictionary_script;
        std::string constraints_script;
        std::string password_script;

        boost::program_options::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("threads,t", boost::program_options::value<unsigned int>(&threads)->default_value(1), "set number of threads")
            ("dictionary,d", boost::program_options::value<std::string>(&dictionary_script)->default_value("std:default_dictionary.lua"), "set the used dictionary script")
            ("constraints,c", boost::program_options::value<std::string>(&constraints_script)->default_value("std:default_constraints.lua"), "set the used constraints script")
            ("password,p", boost::program_options::value<std::string>(&password_script)->required(), "set the used password script");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

        if(vm.count("help"))
        {
            sync_cout << desc << std::endl;
            return 0;
        }

        boost::program_options::notify(vm);

        dictionary_script = resolve_path(dictionary_script);
        constraints_script = resolve_path(constraints_script);
        password_script = resolve_path(password_script);

        //Get a dictionary
        std::unique_ptr<LuaDictionaryData> lua_dict_data(new LuaDictionaryData(dictionary_script));
        Dictionary dict = lua_dict_data->get_dictionary();
        lua_dict_data.reset();

        //Print dictionary
        sync_cout << "Using dictionary: " << std::endl;
        print_dictionary(std::cout, dict);

        //Get the constraints
        std::unique_ptr<LuaConstraintsData> lua_constr_data(new LuaConstraintsData(constraints_script));
        PasswordConstraints constraints = lua_constr_data->get_constraints();
        lua_constr_data.reset();

        std::unique_ptr<LuaPasswordData> lua_pwd_data(new LuaPasswordData(password_script));

        PasswordGenerator generator(dict, constraints, pwd_callback, lua_pwd_data.get());
        std::string password;

        sync_cout << "Running password check with " << threads << " threads" << std::endl;
        sync_cout << "Start time: " << boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) << std::endl;
        int result_status = generator.generate(password, threads);
        sync_cout << "End time: " << boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) << std::endl;

        lua_pwd_data.reset();

        switch(result_status)
        {
            case PWD_CALLBACK_FOUND:
                sync_cout << "Found password: " << password << std::endl;
                break;
            case PWD_CALLBACK_NOT_FOUND:
                sync_cout << "Could not find password" << std::endl;
                break;
            case PWD_CALLBACK_ERROR:
                sync_cout << "Error while running callback for attempted password: " << password << std::endl;
                break;
        }
    }
    catch(const LuaException& e)
    {
        sync_cerr << "A lua script failed: " << e.what() << std::endl;
    }
    catch(const boost::program_options::error& e)
    {
        sync_cerr << "Invalid command line: " << e.what() << std::endl;
    }

    return 0;
}

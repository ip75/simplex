#include <iostream>
#include <ctime>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost;

#include "udp_server.h"


int main() {

    int file_size;
    int16_t port;
    vector<string> vec_dir;
    path directory;


// Declare a group of options that will be
// allowed only on command line
    boost::program_options::options_description generic("Generic options");
    generic.add_options()
            ("version,v", "print version string")
            ("help", "produce help message")
            ;

// Declare a group of options that will be
// allowed both on command line and in
// config file
    boost::program_options::options_description config("Configuration");
    config.add_options()
            ("port", boost::program_options::value<int16_t>(&port)->default_value(5000),
             "port which receive UDP data")
            ("file-size,s", boost::program_options::value<int>(&file_size)->default_value(100000), // memory pages
            "size of file to store data")
            ("result-path,D",
             boost::program_options::value< vector<string> >(&vec_dir)->composing(),
             "path where to store received data files")
            ;

// Hidden options, will be allowed both on command line and
// in config file, but will not be shown to the user.
    boost::program_options::options_description hidden("Hidden options");
    hidden.add_options()
            ("input-file", boost::program_options::value< vector<string> >(), "input file")
            ;


    try
    {
        boost::asio::io_service io_service;
        udp_server server(io_service, port, file_size);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
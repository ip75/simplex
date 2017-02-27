#include <iostream>
#include <ctime>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;
namespace po = boost::program_options;

#include "udp_server.h"


int main(int argc, const char** argv) {

    int file_size;
    int16_t port;
    path directory;


// Declare a group of options that will be
// allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
            ("version,v", "print version string")
            ("help", "produce help message")
            ;

// Declare a group of options that will be
// allowed both on command line and in
// config file
    po::options_description config("Configuration");
    config.add_options()
            ("port", po::value<int16_t>(&port)->default_value(5000),
             "port which receive UDP data")
            ("file-size,s", po::value<int>(&file_size)->default_value(1000), // memory pages
            "size of file to store data in memory pages (1 page 64Kb - windows)")
            ("result-path,D",
             po::value<string>()->implicit_value(current_path().generic_string()),
             "path where to store received data files")
            ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);


    // --help option
    if ( vm.count("help") )
    {
        std::cout << "HELP:" << std::endl
                  << cmdline_options << std::endl;
        return 1;
    }

    if (vm.count("result-path"))
    {
        directory = vm["result-path"].as<std::string>();
    }

    try
    {
        boost::asio::io_service io_service;
        udp_server server(io_service, port, file_size, directory);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
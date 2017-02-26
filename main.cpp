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
namespace po = boost::program_options;

#include "udp_server.h"


int main(int argc, const char** argv) {

    int file_size;
    int16_t port;
    vector<string> vec_dir;
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
            "size of file to store data")
            ("result-path,D",
             po::value< vector<string> >(&vec_dir)->composing(),
             "path where to store received data files")
            ;

// Hidden options, will be allowed both on command line and
// in config file, but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
            ("input-file", po::value< vector<string> >(), "input file")
            ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::store(po::parse_command_line(argc, argv, config_file_options), vm);
    po::notify(vm);


    // --help option
    if ( vm.count("help") )
    {
        std::cout << "HELP:" << std::endl
                  << cmdline_options << std::endl;
        return 1;
    }

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
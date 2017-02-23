//
// Created by ip on 21.02.2017.
//

#ifndef UDP_STREAMER_H
#define UDP_STREAMER_H

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <boost/interprocess/mapped_region.hpp>

#include <boost/filesystem.hpp>
using namespace boost::filesystem;
#include <boost/iostreams/device/mapped_file.hpp>

class streamer {

public:
    streamer(  size_t mem_pages_size,
                path dir = boost::filesystem::current_path() )
                    : m_size (mem_pages_size), m_dir(dir), m_written(0)
    {
        m_mempagesize = boost::interprocess::mapped_region::get_page_size();
        m_size = m_size * m_mempagesize; // get size in bytes
        init_mmap_file();

        m_f_compress = false;
    }

    ~streamer()
    {
        m_file.close();
    }

private:
    boost::iostreams::mapped_file_sink      m_file;
    size_t                                  m_size;
    size_t                                  m_written;
    size_t                                  m_mempagesize;
    path                                    m_dir;
    path                                    m_filename;
    bool                                    m_f_compress;


    void init_mmap_file()
    {
        if(m_file.is_open())
            m_file.close();

        m_file.open(m_dir, m_size);
        m_written = 0;

        // Check if file was successfully opened
        if(!m_file.is_open())
            throw this;  // TODO: add error handling
    }

    void next_file_name()
    {
        m_filename = m_dir;
        m_filename += "streamer_";
        m_filename += std::tmpnam(NULL);
        m_filename += ".bin";
    }

public:

/// returns size of saved data to memory mapped file
    size_t mempagesize() { return  m_mempagesize; }
    size_t dump_data(boost::asio::mutable_buffer buff);
    size_t dump_data(boost::array<unsigned char*, 1000> buff);
};


#endif //UDP_STREAMER_H

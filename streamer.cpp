//
// Created by ip on 21.02.2017.
//

#include "streamer.h"

using namespace boost::asio;

size_t streamer::dump_data(mutable_buffer buff)
{
    size_t size = buffer_size(buff);
    if (m_written + size > m_size)
    {
        // create next file
        next_file_name();
        init_mmap_file();
    }

    // copy data to memory mapped file
    size_t dumped_size = buffer_copy( mutable_buffer(m_file.data() + m_written, size), buff);

    m_written += dumped_size;
    if (size != dumped_size)
        return dumped_size;  // TODO: no more space to store. Throw exception
}

size_t streamer::dump_data(boost::array<unsigned char*, 1000> buff)
{
    if (m_written + buff.size() > m_size)
    {
        // create next file
        next_file_name();
        init_mmap_file();
    }

    // copy data to memory mapped file
    memcpy(m_file.data() + m_written, buff.data(), buff.size());

    m_written += buff.size();
}
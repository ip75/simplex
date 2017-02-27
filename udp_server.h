//
// Created by ip on 21.02.2017.
//

#ifndef UDP_UDP_SERVER_H
#define UDP_UDP_SERVER_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>

#include "streamer.h"

using boost::asio::ip::udp;



class udp_server
{
public:
    udp_server(boost::asio::io_service& io_service, uint16_t port, int file_size, path directory)
            : socket_(io_service, udp::endpoint(udp::v4(), port)), dumper_(file_size, directory)
    {
        start_receive();
    }

private:
    void start_receive()
    {

        socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), remote_endpoint_,
                boost::bind(&udp_server::handle_receive, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error,
                        std::size_t /*bytes_transferred*/)
    {
        if (!error || error == boost::asio::error::message_size)
        {
            dumper_.dump_data(recv_buffer_);

            start_receive();
        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code& /*error*/,
                     std::size_t /*bytes_transferred*/)
    {
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::asio::mutable_buffer recv_buffer_;
    streamer dumper_;
};

#endif //UDP_UDP_SERVER_H

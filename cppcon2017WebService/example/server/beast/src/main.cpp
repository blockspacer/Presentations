#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

// Sends a WebSocket message and prints the response
int main()
{
    try
    {
        auto const host = "echo.websocket.org";
        auto const port = "80";
        auto const text = "Hellow Wo";

        // The io_service is required for all I/O
        boost::asio::io_service ios;

        // These objects perform our I/O
        tcp::resolver resolver{ios};
        websocket::stream<tcp::socket> ws{ios};

        // Look up the domain name
        auto const lookup = resolver.resolve({host, port});

        // Make the connection on the IP address we get from a lookup
        boost::asio::connect(ws.next_layer(), lookup);

        // Perform the websocket handshake
        ws.handshake(host, "/");

        // Send the message
        ws.write(boost::asio::buffer(std::string(text)));

        // This buffer will hold the incoming message
        boost::beast::multi_buffer buffer;

        // Read a message into our buffer
        ws.read(buffer);

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The buffers() function helps print a ConstBufferSequence
        std::cout << boost::beast::buffers(buffer.data()) << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
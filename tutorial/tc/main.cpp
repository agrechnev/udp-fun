#include <iostream>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/array.hpp>

// TCP Client

using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

int main() {
    cout << "TCP Client !" << endl;
    io_service io;
    tcp::resolver resolver(io);
    // Endpoint to the query(host, port)
    tcp::resolver::iterator endpointIt = resolver.resolve(tcp::resolver::query("localhost", "12345"));
    tcp::socket sock(io);
    connect(sock, endpointIt);   // Open the connection !
    for (;;) {
        array<char, 128> buf;
        boost::system::error_code e;
        size_t len = sock.read_some(buffer(buf), e);
        if (e) break;
        cout.write(buf.data(), len);
    }

    return 0;
}

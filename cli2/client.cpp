#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char **argv) {
    using boost::asio::ip::udp;
    using namespace std;

    if (argc < 3) {
        cerr << "Usage: cli2 <host> <port>" << endl;
        return -1;
    }

    boost::asio::io_service ioService;  // IO service
    udp::resolver udpResolver(ioService);  // UDP resolver

    udp::resolver::query udpQuery(udp::v4(), argv[1], argv[2]); // Query
    udp::endpoint recvEndpoint = *udpResolver.resolve(udpQuery);  // Receiver endpoint

    udp::socket udpSocket(ioService);
    udpSocket.open(udp::v4());

    char sendBuf[1] = {0};

    char recvBuf[1024];
    udp::endpoint sendEndpoint; // Sender endpoint

    for (;;) {
        // The keep-alive message
        udpSocket.send_to(boost::asio::buffer(sendBuf, 1), recvEndpoint);

        for (int i = 0; i < 5; ++i) {
            size_t len = udpSocket.receive_from(boost::asio::buffer(recvBuf, 1024), sendEndpoint);

            cout.write(recvBuf, len);
        }
    }

    return 0;
}

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

int main(){
    using namespace std;
    using boost::asio::ip::udp;

    boost::asio::io_service ioService;  // IO service
    udp::socket udpSocket(ioService, udp::endpoint(udp::v4(), 19090)); // socket

    cout << "Waiting for connection ..." << endl;

    char recvBuf[1];
    udp::endpoint rmteEndpoint; // Remote endpoint
    udpSocket.receive_from(boost::asio::buffer(recvBuf, 1), rmteEndpoint);

    cout << "Connected." << endl;

    for (int i = 0;; ++i) {
        stringstream out;
        out << "Message # " << i << endl;
        string mes = out.str();
        udpSocket.send_to(boost::asio::buffer(mes), rmteEndpoint);

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

int main() {
    using namespace std;
    using boost::asio::ip::udp;

    boost::asio::io_service ioService;  // IO service
    udp::socket udpSocket(ioService, udp::endpoint(udp::v4(), 19090)); // socket

    cout << "Waiting for connections ..." << endl;

    map<udp::endpoint, int> clients;
    thread t([&clients, &udpSocket]() -> void {
        for (;;) {
            char recvBuf[1];
            udp::endpoint rmteEndpoint; // Remote endpoint
            boost::system::error_code ec;
            udpSocket.receive_from(boost::asio::buffer(recvBuf, 1), rmteEndpoint, 0, ec);

            if (!ec) {
                cout << "Connected to " << rmteEndpoint.address().to_string() <<
                     " port " << rmteEndpoint.port() << endl;

                clients[rmteEndpoint] = 10;
            } else {
                cout << "WinSock disconnect error !" << endl;
            }
        }
    });

    for (int i = 0;; ++i) {
        stringstream out;
        out << "Message # " << i << endl;
        string mes = out.str();
        cout << mes;
        for (auto it = clients.begin(); it != clients.end();) {
            udpSocket.send_to(boost::asio::buffer(mes), it->first); // Send to the endpoint
            --(it->second);

            if (it->second == 0) {
                cout << "Connection timed out " << it->first.address().to_string() <<
                     " port " << it->first.port() << endl;
                it = clients.erase(it);  // Remove from the list
            }
            else
                ++it;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}

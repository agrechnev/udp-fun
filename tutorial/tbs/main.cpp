#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <atomic>

#include <boost/asio.hpp>

// TCP BLOCKING server
using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

string makeDateStr(){
    time_t now = time(0);
    return ctime(&now);
}


int main() {
    cout << "TCP Blocking server !" << endl;
    io_service io;
    tcp::acceptor acc(io, tcp::endpoint(tcp::v4(), 12345));
    acc.non_blocking(true);
    tcp::socket sock(io);

    atomic<bool> stop(false);

    thread t([&stop]{
        cin.get();
        cout << "SHUTDOWN !!!" << endl;
        stop = true;
    });

    for(;;){
        // Wait for a connection
        boost::system::error_code ec;
        do {
            this_thread::sleep_for(chrono::milliseconds(100));
            acc.accept(sock, ec);
        } while (!stop && ec);

        if (stop)
            break;

        string dateStr = makeDateStr(); // Daytime string
        write(sock, buffer(dateStr), ec);
        sock.close();
    }

    cout << "Exiting ..." << endl;
    t.join();

    return 0;
}

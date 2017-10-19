#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>

// TCP ASYNC server
using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

string makeDateStr() {
    time_t now = time(0);
    return ctime(&now);
}

class Serv {
public:
    Serv(io_service &ioS) :
            acc(ioS, tcp::endpoint(tcp::v4(), 12345)),
            sock(ioS) {
        start();
    }

private:
    tcp::acceptor acc;
    tcp::socket sock;

    void start() {
        cout << "start()" << endl;
        boost::system::error_code ec;
        sock.close(ec);
        acc.async_accept(sock, [this](const boost::system::error_code &e) -> void {
            handle(e);
        });
    }

    void handle(const boost::system::error_code &error) {
        cout << "handle()" << endl;
        // Send date+time
        if (!error) {
            cout << "writing ..." << endl;
            io_service &io = acc.get_io_service();
            string str = makeDateStr();
            async_write(sock, buffer(str), [this](const boost::system::error_code &, size_t) {
                start(); // Next accept
            });
        } else {
            start(); // Next accept
        }
    }
};

int main() {
    io_service io;
    Serv serv(io);

    thread t([&io]{
        cin.get();
        cout << "SHUTDOWN !!!" << endl;
        io.stop();
    });

    io.run();
    t.join();

    return 0;
}
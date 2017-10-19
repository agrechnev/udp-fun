#include <iostream>
#include <string>
#include <array>
#include <thread>

#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio;

constexpr size_t MAX_SIZE = 1024;

// Echo TCP Blocking Server
int main() {
    io_service io;
    tcp::acceptor a(io, tcp::endpoint(tcp::v4(), 12345));
    // Accept loop
    for (;;){
        tcp::socket s(io);
        a.accept(s);
        thread([s = move(s)]() mutable->void {
            for(;;){
                array<char, MAX_SIZE> buf;
                boost::system::error_code e;
                size_t len = s.read_some(buffer(buf), e);
                cout.write(buf.data(), len);
                cout << endl;
                if (e == boost::asio::error::eof) {
                    break; // Closed
                }
                else if (e)
                    throw boost::system::system_error(e);

                write(s, buffer(buf, len));
            }
            cout << "Thread Closed !" << endl;
        }).detach();
    }

    return 0;
}

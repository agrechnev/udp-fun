#include <iostream>
#include <string>
#include <array>
#include <thread>
#include <atomic>
#include <chrono>

#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio;

constexpr size_t MAX_SIZE = 1024;

// Echo TCP Blocking Server (but I am trying to make it non-blocking)
int main() {
    // The stop variable and thread
    atomic<bool> stop(false);
    thread t([&stop]{
        cin.get();
        cout << "SHUTDOWN !!!" << endl;
        stop = true;
    });

    io_service io;
    tcp::acceptor a(io, tcp::endpoint(tcp::v4(), 12345));
    // Accept loop
    while (!stop){
        tcp::socket s(io);
        cout << "Waiting !" << endl;
        a.non_blocking(true);
        boost::system::error_code ec;
        do {
            this_thread::sleep_for(chrono::milliseconds(100));
            a.accept(s, ec);
        } while (!stop && ec);

        if (stop)
            break;

        s.non_blocking(true);
        thread([s = move(s), &stop]() mutable->void {
            while (!stop){
                array<char, MAX_SIZE> buf;
                boost::system::error_code e;
                size_t len = s.read_some(buffer(buf), e);
                if (e == boost::asio::error::would_block) {
                    this_thread::sleep_for(chrono::milliseconds(100));
                    cout << '.';
                } else if (e) { // Some error or EOF
                    cout << e.value() << ":" << e.message() << endl;
                    break;
//                    throw boost::system::system_error(e);
                }  else {
                    cout << "Writing ..." << endl;
                    cout.write(buf.data(), len);
                    cout << endl;
                    write(s, buffer(buf, len));
                }
            }
            cout << "Thread Closed !" << endl;
        }).detach();
    }

    t.join();
    return 0;
}

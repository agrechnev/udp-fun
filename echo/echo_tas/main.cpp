#include <iostream>
#include <memory>
#include <array>
#include <thread>

#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

constexpr size_t MAX_SIZE = 1024;

//====================================
class Session : public enable_shared_from_this<Session>{
public:
    Session(tcp::socket sock) : sock(move(sock)) {}
    ~Session() {
        cout << "Disconnected !" << endl;
    }

    void start(){
        doRead();
    }

private:
    tcp::socket sock;
//    char data[MAX_SIZE];
    array<char, MAX_SIZE> buf;

    void doRead(){
        sock.async_read_some(buffer(buf),
                             [this, self = shared_from_this()](boost::system::error_code ec, size_t len)->void{
            if (!ec){
                cout.write(buf.data(), len);
                cout << endl;
                doWrite(len);  // Write what we read !
            }
        });
    }

    void doWrite(size_t len){
        async_write(sock, buffer(buf, len),
                    [this, self = shared_from_this()](boost::system::error_code ec, size_t /* len */)->void{
            if (!ec)
                doRead();
        });
    }
};
//====================================
class Server{
public:
    Server(io_service & io, short port) :
        acc(io, tcp::endpoint(tcp::v4(), port)),
        sock(io)  {
        doAccept();
    }
private:
    tcp::acceptor acc;
    tcp::socket sock;

    void doAccept(){
        acc.async_accept(sock, [this](boost::system::error_code ec)->void{
            if (!ec) {
                cout << "Connected !" << endl;
                make_shared<Session>(move(sock))->start();
            }
            doAccept();  // Accept next session
        });
    }
};
//====================================
int main(){
    cout << "Goblins WON !!!" << endl;
    io_service io;
    thread t([&io]{  // Stop thread
        cin.get();
        cout << "SHUTDOWN !!!" << endl;
        io.stop();
    });
    try {
        Server s(io, 12345);
        io.run();
    } catch (exception & e) {
        cerr << e.what() << endl;
    }
    t.join();
    return 0;
}

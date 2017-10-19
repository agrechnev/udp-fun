#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio;

// Echo TCP Client
int main() {
    constexpr size_t MAX_SIZE = 1024;
    io_service io;
    tcp::resolver r(io);
    tcp::socket s(io);
    connect(s, r.resolve({"localhost", "12345"}));

    cout << "Enter message :";
    string msg;
    getline(cin, msg);
    write(s, buffer(msg));

    cout << "Write finished !" << endl;

    vector<char> rdBuf(MAX_SIZE);
    size_t len = s.read_some(buffer(rdBuf));
    cout << "Reply is : ";
    cout.write(rdBuf.data(), len);
    cout << endl;

    return 0;
}

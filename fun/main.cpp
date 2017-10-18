#include <iostream>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/system_timer.hpp>

using namespace std;
using namespace boost::asio;

void print(const boost::system::error_code &, system_timer &t, int &count) {
    cout << count << endl;
    ++count;
    t.expires_at(t.expires_at() + chrono::seconds(1));
    if (count <= 7)
        t.async_wait([&t, &count](const boost::system::error_code &) -> void {
            boost::system::error_code e;
            print(e, t, count);
        });
}

int main() {

    io_service io;
//    deadline_timer t(io, boost::posix_time::seconds(5));
    system_timer t(io, chrono::seconds(1));

    cout << "Before wait() ... " << endl;
    int count = 0;
    t.async_wait([&t, &count](const boost::system::error_code &) -> void {
        boost::system::error_code e;
        print(e, t, count);
    });

    io.run();

    cout << "Final count = " << count << endl;
    cout << "Brianna the Handmaiden !!!" << endl;

    return 0;
}

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <map>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <cstdlib>
#include <time.h>
#include <vector>
#define CST (+8)

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::map;
using std::pair;
using std::vector;

class Work {
public:
 Work(boost::asio::io_context& io) :
        strand_(boost::asio::make_strand(io)),
        timer1_(io, boost::asio::chrono::seconds(2)),
        timer2_(io, boost::asio::chrono::seconds(5))
    {
        timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&Work::Request, this)));
        timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&Work::Finding, this)));
    }

    void Request() {
      
        time_t t = time(NULL);
        tm* tptr = localtime(&t);

        cout << ++order<< " Ride " << "requested at: " << "\t"<< (tptr->tm_hour) << ":" << (tptr->tm_min) << ":" << (tptr->tm_sec) <<"\n" << endl;
            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(2));
            timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&Work::Request, this)));

            if (order == 10) {
                cout << "\tSorry! We couldn't find a Uber in your area. \n\tTry again later." << endl;
                exit(0);
            }
            
    }

    void Finding() {

        time_t t = time(NULL);
        tm* tptr = localtime(&t);
        cout << "\nFinding a Uber near the user... " << (tptr->tm_hour) << ":" << (tptr->tm_min) << ":" << (tptr->tm_sec) << "\n" << endl;
        timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(5));
        timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&Work::Finding, this)));
    }

    ~Work() {};

private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int order;
};

void Uber() {

    boost::asio::io_context io;
    Work w(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
    io.run();
    t.join();

    return;
}


int main()
{ 
    cout << "\tRequest a Uber now?" << endl;

    vector<pair<int, string >> options{
        {1, "Yes"},
        {2, "No"},
    };
    for (auto i = 0; i < options.size(); ++i)
        cout << options[i].first << '\t' << options[i].second << endl;

    int order;
    cin >> order;

    switch (order) {
    case 1:
        system("cls");
        Uber();
        break;
    case 2: exit(0);
        break;
    default:
        break;
    }

    cin.get();    
}
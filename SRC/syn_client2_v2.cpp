#define _CRT_SECURE_NO_WARNINGS
#include<ctime>
#include<iostream>
#include<string>
#include<boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

string takeJob() {
           string id_job;
           cout << "Type your job id: 1/2/3; Choose one of 3 to read file1/file2/file3" << endl;
           cin >> id_job; //get command from the keyboard 
           cout << "Your job is: " << id_job << endl;
           return id_job;
}
string takeUserCommand() {
           string command_user;
           cout << "Type your command: Start/Stop/Pause/Resume; Choose one of 4 commands at a time" << endl;
           cin >> command_user; //get command from the keyboard 
           cout << "Your command is: " << command_user << endl;
           return command_user;
}

string read_(tcp::socket & socket) {
       boost::asio::streambuf buf;
       boost::system::error_code error;
       boost::asio::read_until( socket, buf, 'n', error );
       if (error){
              // cout << error << endl;
              return ("error");
       } else {
       string data = boost::asio::buffer_cast<const char*>(buf.data());
       return data;
       }
}

void send_(tcp::socket & socket, const string& message) {
       const string msg = message;
       boost::asio::write( socket, boost::asio::buffer(msg) );
}

int main() {
    try{
//define sockets
      boost::asio::io_service io_service; //call io_service
      boost::asio::ip::tcp::socket socket1(io_service); //define socket
      boost::asio::ip::tcp::endpoint ep1(boost::asio::ip::address::from_string("127.0.0.1"), 2019); //define endpoint to connect
      boost::asio::ip::tcp::socket socket2(io_service);
      boost::asio::ip::tcp::endpoint ep2(boost::asio::ip::address::from_string("127.0.0.1"), 2020);
      boost::asio::ip::tcp::socket socket3(io_service);
      boost::asio::ip::tcp::endpoint ep3(boost::asio::ip::address::from_string("127.0.0.1"), 2021);
      socket1.connect(ep1);
      socket2.connect(ep2);
      socket3.connect(ep3);
//take requests from users
int id_server;
string id_client = "2";
while (1) {
cout << "Choose server you want to connect: 1,2,3" << endl;
cin >> id_server; //get command from the keyboard 
cout << "You chose server:  " << id_server << endl;
if (id_server == 1) {
    cout << "Connected to server 1....." << endl;
    string job = takeJob();
    string userCommand = takeUserCommand();
    string message_to_server = id_client + "," + job + "," + userCommand;
    cout << "Sending: " << message_to_server << endl;
    send_(socket1,message_to_server);
    string ack = read_(socket1);
    cout << "Feedback from server: " << ack << endl;
    cout << "........................." << endl;
} else if (id_server == 2) {
    cout << "Connected to server2" << endl;
    string job = takeJob();
    string userCommand = takeUserCommand();
    string message_to_server = job + "," + userCommand;
    cout << "Sending: " << message_to_server << endl;
    send_(socket2,message_to_server);
    string ack = read_(socket2);
    cout << "Feedback from server: " << ack << endl;
    cout << "........................." << endl;
} else if (id_server == 3) {
    cout << "Connected to server3" << endl;
    string job = takeJob();
    string userCommand = takeUserCommand();
    string message_to_server = job + "," + userCommand;
    cout << "Sending: " << message_to_server << endl;
    send_(socket3,message_to_server);
    string ack = read_(socket3);
    cout << "Feedback from server: " << ack << endl;
    cout << "........................." << endl;
} else {
    cout << "No such a server provided!!!s" << endl;
}
}
} catch(boost::system::system_error const& e) {
		std::cout << "Warning: could not connect : " << e.what() << std::endl;
	}
   return 0;
}
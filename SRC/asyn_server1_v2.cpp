#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <unistd.h> 
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio/write.hpp>
#include <unordered_map>
#include <pthread.h>
#include <fstream>
#include "jsoncpp/json/json.h"
#include <json/value.h>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>
#include <map>


using boost::asio::ip::tcp;
using namespace std;
using std::thread;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
namespace ba = boost::asio;
namespace bs = boost::system;
using ba::ip::tcp;

namespace
{
    enum lastStatus {
            STOP,
            START,
            PAUSE,
            RESUME
             };
      

      const size_t MAX_LEN = 512;
      const uint16_t ECHO_PORT = 2020;

      class Session : public std::enable_shared_from_this<Session>
      {
      private:
            tcp::socket socket_;
            char data_[MAX_LEN];
            //typedef std::unordered_map<std::string, std::thread> ThreadMap;
            typedef std::unordered_map<std::string, pthread_t> ThreadMap;
            ThreadMap tm_;
            std::map<std::string, int> lines_temp_ = {
                  {"file1client1", 0},
                  {"file2client1", 0},
                  {"file3client1", 0},
                  {"file1client2", 0},
                  {"file2client2", 0},
                  {"file3client2", 0},
                  {"file1client3", 0},
                  {"file2client3", 0},
                  {"file3client3", 0}
            };
            

            enum lastStatus status_;

            void write(std::size_t len)
            {
                  std::cout << "Received " << len << " bytes: ";
                  std::cout.write(data_, len);
                  std::cout << std::endl;
                  string data = data_;
                  data = data.substr(0,len);
                  std::cout << std::endl;
                  std::cout << data << std::endl;
                  
                  std::shared_ptr<Session> self{ shared_from_this() };
                  boost::asio::async_write(socket_, boost::asio::buffer("Received command"), [self](boost::system::error_code ec, std::size_t) {
                        if (!ec)
                        {
                              self->read();
                        }
                  });
            }

      public:
            Session(tcp::socket socket) : socket_(std::move(socket)) {}

            void read()
            {
                  std::shared_ptr<Session> self{ shared_from_this() };
                  socket_.async_read_some(boost::asio::buffer(data_), [self](boost::system::error_code ec, std::size_t len) {
                        if (!ec) {
                              self->write(len);
                              string data = self->data_;
                              data = data.substr(0,len);

                          //convert message from client into vector -> [id_client, id_job, command], ex: [1, 1, Start]
                                std::stringstream sstr(data);
                                std::vector<std::string> v;
                                while(sstr.good()) {
                                             std::string substr;
                                             getline(sstr, substr, ',');
                                             v.push_back(substr);
                                           }

                    //take id_client and id_job and create string parts for making thread ID 
                    string client_id = v[0];
                    string client_id_string = "client" + client_id;
                    string job_id = v[1];
                    string job_id_string = "file" + job_id;
                    string temp_key = job_id_string + client_id_string;
                    string command = v[2];
                    //take the number of lines in file
                    int lines_number = self->sum_of_line(job_id);
                    //take last command from the metadata
                    std::string lastcmd = self->read_metadata_file_lastcmd(job_id_string, client_id_string);
                    //take current reading from the metadat
                    int currentreading = self->read_metadata_file_currentreading(job_id_string, client_id_string);
                    //temporary variable for line reading
                    
                    //create id for each thread
                    std::string keyword("thread");
                    std::string tname = keyword + client_id_string + job_id_string;
                          if ((lastcmd == "Stop") && (command == "Start")) {
                                  self->start_thread_read(tname, job_id, temp_key, lines_number);  
                                  self->update_metafile_lastcmd(job_id_string, client_id_string, command);
                          }
                          if ((lastcmd == "Start") && (command == "Stop")) {
                                  self->stop_thread(tname);
                                  self->update_metafile_lastcmd(job_id_string, client_id_string, command);
                                  int current_reading = 0;
                                  self->update_metadata_file_currentreading(job_id_string, client_id_string, current_reading);
                               }
                          if ((lastcmd == "Start") && (command == "Pause")) {
                                  self->stop_thread(tname);
                                  self->update_metafile_lastcmd(job_id_string, client_id_string, command);
                                  int current_reading = self->lines_temp_[temp_key];
                                  self->update_metadata_file_currentreading(job_id_string, client_id_string, current_reading);
                           }
                          if ((lastcmd == "Pause") && (command == "Resume")) {
                                  int currentreading = self->read_metadata_file_currentreading(job_id_string, client_id_string);
                                  self->start_thread_read_specific_line(tname, job_id, currentreading, temp_key, lines_number);
                                  self->update_metafile_lastcmd(job_id_string, client_id_string, command);
                          }
                          if ((lastcmd == "Resume") && (command == "Stop")) {
                                  self->stop_thread(tname);
                                  self->update_metafile_lastcmd(job_id_string, client_id_string, command);
                                  int current_reading = 0;
                                  self->update_metadata_file_currentreading(job_id_string, client_id_string, current_reading);
                          }
                          if ((lastcmd == "Resume") && (command == "Pause")) {
                                  self->stop_thread(tname);
                                  self->update_metafile_lastcmd(job_id_string, client_id_string, command);
                                  int current_reading = self->lines_temp_[temp_key];
                                  self->update_metadata_file_currentreading(job_id_string, client_id_string, current_reading);
                          }
                          if ((lastcmd == "Pause") && (command == "Stop")) {
                                  self->update_metafile_lastcmd(job_id_string, client_id_string, command);
                                  int current_reading = 0;
                                  self->update_metadata_file_currentreading(job_id_string, client_id_string, current_reading);
                          }
                        }
                   });
            }

        
            void read_file(const std::string &tname, string job_id, string temp_key, int lines_number) { //read from the first line
                   string filepath = "./files/" + job_id + ".txt";
                   std::ifstream file(filepath);
                   std::string line;
                   for (int line_i = 0; getline(file, line) && line_i < lines_number; line_i++) {
                        cout << line << endl;
                        lines_temp_[temp_key] = line_i;
                        sleep(3);
                   }
                }


            int sum_of_line (string job_id) {
                  string filepath = "./files/" + job_id + ".txt";
                  std::ifstream file(filepath);
                  std::string line;
                  int count = 0;
                  while (getline(file, line)) {
                        count++;
                  }
                  return count;
                  }


            void read_file_from_specific_line(const std::string &tname, string job_id, int line_number, string temp_key, int lines_number) {
                  string filepath = "./files/" + job_id + ".txt";
                  std::ifstream file(filepath);
                  std::string line;
                  for (int line_i = 0; getline(file, line) && line_i < lines_number; line_i++) {
                        if (line_i >= line_number) {
                              cout << line << endl;
                              lines_temp_[temp_key] = line_i;
                              sleep(3);
                        } else continue;
                  }
                  }
        
              
            void start_thread_read(const std::string &tname, string job_id, string temp_key, int lines_number)
              {
                  std::thread thr = std::thread(&Session::read_file, this, tname, job_id, temp_key, lines_number);
                  tm_[tname] = thr.native_handle();
                  thr.detach();
                  std::cout << "Thread " << tname << " created:" << std::endl;
              }

            void start_thread_read_specific_line(const std::string &tname, string job_id, int line_number, string temp_key, int lines_number)
              {
                  std::thread thr = std::thread(&Session::read_file_from_specific_line, this, tname, job_id, line_number, temp_key, lines_number);
                  tm_[tname] = thr.native_handle();
                  thr.detach();
                  std::cout << "Thread " << tname << " created:" << std::endl;
              }

            void stop_thread(const std::string &tname)
              {
                  ThreadMap::const_iterator it = tm_.find(tname);
                  if (it != tm_.end()) {
                        pthread_cancel(it->second);
                      tm_.erase(tname);
                      std::cout << "Thread " << tname << " killed:" << std::endl;
                  }
              }
        
            // void show_thread(const std::string &keyword)
            //   {
            //       std::string cmd("ps -T | grep ");
            //       cmd += keyword;
            //       system(cmd.c_str());
            //   }

            string read_metadata_file_lastcmd(string job_name,string client_name) 
              { 
                  std::string lastcmd;
                  std::ifstream metadata("metadata.json", std::ifstream::binary);
                  Json::Reader reader;
                  Json::Value obj;
                  reader.parse(metadata, obj);
                  lastcmd = obj[job_name][client_name]["lastcommand"].asString();
                  return lastcmd;
               }

            int read_metadata_file_currentreading(string job_name,string client_name) 
               { 
                  int reading;
                  std::ifstream metadata("metadata.json", std::ifstream::binary);
                  Json::Reader reader;
                  Json::Value obj;
                  reader.parse(metadata, obj);
                  reading = obj[job_name][client_name]["currentreading"].asInt();
                  return reading;
                }

            void update_metafile_lastcmd(string job_name, string client_name, string new_cmd) {
                              std::ifstream metadata("metadata.json", std::ifstream::binary);
                              std::ofstream outFile;
                              Json::Reader reader;
                              Json::Value obj;
                              Json::StyledStreamWriter writer;
                              reader.parse(metadata, obj);
                              obj[job_name][client_name]["lastcommand"] = new_cmd;
                              outFile.open("metadata.json");
                              writer.write(outFile, obj);
                              outFile.close();
                              std::cout << obj[job_name][client_name]["lastcommand"].asString() << std::endl;
                  }

            void update_metadata_file_currentreading(string job_name, string client_name, int new_reading) {
                std::ifstream metadata("metadata.json", std::ifstream::binary);
                        std::ofstream outFile;
                        Json::Reader reader;
                        Json::Value obj;
                        Json::StyledStreamWriter writer;
                        reader.parse(metadata, obj);
                        obj[job_name][client_name]["currentreading"] = new_reading;
                        outFile.open("metadata.json");
                        writer.write(outFile, obj);
                        outFile.close();
                        std::cout << obj[job_name][client_name]["currentreading"].asInt() << std::endl;
            }


       };

      class Server
      {
      private:
            tcp::acceptor acceptor_;

            void accept()
            {
                  std::cout << "Server ready" << std::endl;
                  acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
                  {
                        if (!ec)
                        {
                              std::make_shared<Session>(std::move(socket))->read();
                        }
                        accept();
                  });
            }
      public:
            Server(boost::asio::io_context& io) : acceptor_(io, tcp::endpoint(tcp::v4(), ECHO_PORT))
            {
                  accept();
            }
      };
}

int main(int argc, char* argv[])
{
      boost::asio::io_context io;
      try
      {
            Server server(io);
            io.run();
      }
      catch (std::exception& e)
      {
            std::cerr << e.what() << std::endl;
      }
}

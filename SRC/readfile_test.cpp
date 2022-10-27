#include <iostream>
#include "jsoncpp/json/json.h"
#include <json/value.h>
#include <fstream>
#include <string> 
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h> 
#include <time.h>


using namespace std;


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

void read_file_from_specific_line(string job_id, int line_number) {
            string filepath = "./files/" + job_id + ".txt";
            std::ifstream file(filepath);
            std::string line;
            int count = sum_of_line(job_id);
            cout << "the number of lines is: " << count << endl;
            for (int line_i = 0; getline(file, line) && line_i < count; line_i++) {
                  if (line_i >= line_number) {
                        cout << line << endl;
                        sleep(3);
                  } else continue;
            }
        }

int main() {
	string job = "1";
      read_file_from_specific_line(job, 35);
}
#include <iostream>
#include <fstream>
#include "jsoncpp/json/json.h"
#include <json/value.h>

using namespace std;

string readMetadataFile(string job_name, string client_name) {
	std::ifstream metadata("metadata.json", std::ifstream::binary);
	Json::Reader reader;
	Json::Value obj;
	reader.parse(metadata, obj);
    string cmd = obj[job_name][client_name]["lastcommand"].asString();
    return cmd;
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
// string read_metadata_file_lastcmd() 
// 		{     
//               string job = "file1";
// 			  string client = "client1";
// 			  std::string lastcmd;
// 			  std::ifstream metadata("metadata.json", std::ifstream::binary);
// 	          Json::Reader reader;
// 	          Json::Value obj;
// 	          reader.parse(metadata, obj);
//               lastcmd = obj[job][client]["lastcommand"];
//               return lastcmd;
// 		}

int main() {
	string job = "file1";
	string client = "client1";
	string newcmd = "Start";
	update_metafile_lastcmd(job, client, newcmd);
}
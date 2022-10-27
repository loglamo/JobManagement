#include <vector>
#include <string>
#include <sstream>
#include <iostream>
 
using namespace std;
using std::vector;

vector<string> convert(std::string input) {
     // std::stringstream sstr("7,8,9,5");
     std::stringstream sstr(input);
     std::vector<std::string> v;
     while(sstr.good())
      {
        std::string substr;
        getline(sstr, substr, ',');
        v.push_back(substr);
      }
     // for (std::size_t i = 0; i < v.size(); i++)
     //    std::cout << v[i] << std::endl;
     return v;
} 
int main()
{   
    string str = "1,2,3,4";
    vector<string> convert = convert(str);
    std::cout << convert[0] << std::endl;
}    
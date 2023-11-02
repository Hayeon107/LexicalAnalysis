#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <map>
using namespace std;
// Global variable declarations
map<string, int> output;
map<string, int>::iterator iter;
ifstream file("front.txt");

int main() {
    // Open the input data file and process its contents
    string id1 = "operand1";
    string id2 = "operand2";
    string id3 = "target";
    int const1 = 3;
    int const2 = 2;
    int const3 = 3;

    output[id1] = const1;
    output[id2] = output[id1]+const2;
    output[id3] = output[id1] + output[id2]; 

    for (iter = output.begin(); iter != output.end(); iter++) {
        cout << iter->first << ": " << iter->second << "; ";
    }
    return 0;
}

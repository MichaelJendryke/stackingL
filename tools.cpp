#include <string> 
#include <cstdio>
#include <memory.h>

using namespace std;
 
 std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

 char* string2charstar(string s) {
    //std::string s
    char *a = new char[s.size() + 1];
    a[s.size()] = 0;
    memcpy(a, s.c_str(), s.size());
    return a;
}
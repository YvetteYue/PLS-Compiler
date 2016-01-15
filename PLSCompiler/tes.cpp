 // reading a text file
/**    #include <iostream>
    #include <fstream>
#include <cstdlib>
using namespace std;
int main() {
        char buffer[256];
        ifstream in("D:\\Document\\Documents\\学习\\参加活动.txt");
        if (! in.is_open())
        { cout << "Error opening file"; exit (1); }
        while (!in.eof() )
        {
            in.getline (buffer,100);
            cout << buffer << endl;
        }
        return 0;
}
    //结果 在屏幕上输出

**/

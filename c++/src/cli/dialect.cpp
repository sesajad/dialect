#include <fstream.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    // Check the number of parameters
    if (argc < 2) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " file.dlt" << std::endl;
        std::cerr << "       " << argv[0] << " -def file.dld file.dlt" << std::endl;
    }
    std::cout << "Dialect is a programming language/accent creation and translation tool" << std::endl;

}
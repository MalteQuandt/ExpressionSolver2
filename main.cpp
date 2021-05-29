#include <iostream>
#include "Token.hpp"

using namespace std;

int main(int argc, char **argv)
{
    // Some simple one-line non-inline comment!
    if (argc == 1)
        return EXIT_FAILURE;
    cout << tok::eval(argv[1]) << endl;
}
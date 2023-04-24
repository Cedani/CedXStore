#ifdef _WIN32
#include <iostream>
#include "App/App.hpp"

int main(int argc, char **argv)
{
    lau::App *_myapp = new lau::App;
    return (wxEntry(argc, argv));
}
#endif
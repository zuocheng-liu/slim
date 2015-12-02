#include "Slim/Util/CGITCPServerApplication.hpp"
#include <iostream>
using  Slim::Util::CGITCPServerApplication;
using namespace std;
int main(int argc, char** argv) {
    CGITCPServerApplication app;
    return app.run(argc, argv);
}

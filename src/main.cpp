#include <Slim/Util/CGITCPServerApplication.hpp>
#include <iostream>
using  Slim::Util::CGITCPServerApplication;
int main(int argc, char** argv) {
    CGITCPServerApplication app;
    return app.run(argc, argv);
}

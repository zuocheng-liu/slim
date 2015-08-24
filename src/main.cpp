#include "Slim/Util/ServerApplication.hpp"
#include <iostream>
using  Slim::Util::ServerApplication;
int main(int argc, char** argv) {
    ServerApplication app;
    return app.run(argc, argv);
}

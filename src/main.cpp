#include <iostream>
#include "Slim::Util::ServerApplication.hpp"

int main(int argc, char** argv) {
    Slim::Util::ServerApplication app;
    return app.run(argc, argv);
}

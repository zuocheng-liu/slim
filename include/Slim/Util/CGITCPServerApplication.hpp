#ifndef SLIM_UTIL_CGITCPSERVER_APPLICATION_HPP
#define SLIM_UTIL_CGITCPSERVER_APPLICATION_HPP

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServerParams.h> 
#include <Poco/Net/TCPServer.h> 
#include <Poco/Exception.h>
#include <Poco/ThreadPool.h>
#include <Poco/Util/ServerApplication.h>
#include "Slim/Net/CGI/TCPServerConnectionFactory.h"

namespace Slim {
namespace Util {
class CGITCPServerApplication: public Poco::Util::ServerApplication {
    public:
        CGITCPServerApplication() {
        }

        ~CGITCPServerApplication() {
        }

    protected:
        void initialize(Poco::Util::Application& self) {
            // load default configuration files, if present
            Poco::Util::ServerApplication::loadConfiguration(); 
            Poco::Util::ServerApplication::initialize(self);
        }

        void uninitialize() {
            Poco::Util::ServerApplication::uninitialize();
        }

        int main(const std::vector<std::string>& args) {
            // get parameters from configuration file
            unsigned short port = (unsigned short) config().getInt("CGIServer.port", 8081);
            int maxQueued  = config().getInt("CGIServer.maxQueued", 100);
            int maxThreads = config().getInt("CGIServer.maxThreads", 16);
            Poco::ThreadPool::defaultPool().addCapacity(maxThreads);
            Poco::Net::TCPServerParams *pParams = new Poco::Net::TCPServerParams();
            pParams->setMaxQueued(maxQueued);
            pParams->setMaxThreads(maxThreads);

            // set-up a server socket
            Poco::Net::ServerSocket socket(port);
            // Pass the ServerSocket to a TCPServer 
            Poco::Net::TCPServer server(new Slim::Net::CGI::TCPServerConnectionFactory(), socket); 
            server.start();
            // wait for CTRL-C or kill
            waitForTerminationRequest();
            // Stop the HTTPServer
            server.stop();
            return Poco::Util::Application::EXIT_OK;
        }
}; // end class CGITCPServerApplication
} // end namespace Util
} // end namespace Slim
#endif

#ifndef SLIM_UTIL_HTTPSERVER_APPLICATION_HPP
#define SLIM_UTIL_HTTPSERVER_APPLICATION_HPP

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Exception.h>
#include <Poco/ThreadPool.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Slim/Net/HTTPRequestHandlerFactory.hpp>
#include <iostream>

using Slim::Net::HTTPRequestHandlerFactory;

namespace Slim {
    namespace Util {
        class HttpServerApplication: public Poco::Util::ServerApplication {
            public:
                HttpServerApplication() {
                }

                ~HttpServerApplication() {
                }

            protected:
                void initialize(Poco::Util::Application& self) {
                    loadConfiguration(); // load default configuration files, if present
                    Poco::Util::ServerApplication::initialize(self);
                }

                void uninitialize() {
                    Poco::Util::ServerApplication::uninitialize();
                }

                void defineOptions(Poco::Util::OptionSet& options) {
                    Poco::Util::ServerApplication::defineOptions(options);

                    options.addOption(
                            Poco::Util::Option("help", "h", "display help information on command line arguments")
                            .required(false)
                            .repeatable(false));
                }

                void handleOption(const std::string& name, const std::string& value) {
                    Poco::Util::ServerApplication::handleOption(name, value);
                }

                int main(const std::vector<std::string>& args) {
                    // get parameters from configuration file
                    unsigned short port = (unsigned short) config().getInt("HTTPTimeServer.port", 9980);
                    //std::string format(config().getString("HTTPTimeServer.format", Poco::DateTimeFormat::SORTABLE_FORMAT));
                    int maxQueued  = config().getInt("HTTPTimeServer.maxQueued", 100);
                    int maxThreads = config().getInt("HTTPTimeServer.maxThreads", 16);
                    Poco::ThreadPool::defaultPool().addCapacity(maxThreads);
                    Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
                    pParams->setMaxQueued(maxQueued);
                    pParams->setMaxThreads(maxThreads);

                    // set-up a server socket
                    Poco::Net::ServerSocket svs(port);
                    // set-up a HTTPServer instance
                    Poco::Net::HTTPServer srv(new HTTPRequestHandlerFactory(), svs, pParams);
                    // start the HTTPServer
                    srv.start();
                    // wait for CTRL-C or kill
                    waitForTerminationRequest();
                    // Stop the HTTPServer
                    srv.stop();
                    return Poco::Util::Application::EXIT_OK;
                }

            private:
        };
    }
}
#endif

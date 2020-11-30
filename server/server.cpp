#include <model/myBlockChain.h>
#include <openabe/openabe.h>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
 
#include <iostream>
#include <string>
#include <vector>
 
using namespace std;
using namespace Poco::Net;
using namespace Poco::Util;
 
class MyRequestHandler : public HTTPRequestHandler
{
public:
	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
	{
		resp.setStatus(HTTPResponse::HTTP_OK);
		resp.setContentType("text/html");
 
		ostream & out = resp.send();
 
		out << "<h1>Hello World!</h1>";
	}
};
 
class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &)
	{
		return new MyRequestHandler;
	}
};
 
class MyServerApp :public ServerApplication
{
protected:
	int main(const vector<string> &)
	{
		HTTPServer s(new MyRequestHandlerFactory, ServerSocket(8080), new HTTPServerParams);
 
		s.start();
		cout << endl << "Server started" << endl;
 
		waitForTerminationRequest();  // wait for CTRL-C or kill
 
		cout << endl << "Shutting down..." << endl;
 
		s.stop();
 
		return Application::EXIT_OK;
	}
};
 
int main(int argc, char **argv)
{
	MyServerApp app;
 
	return app.run(argc, argv);
}
#include <http_tcpServer_linux.h>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>

namespace
{
  const int		BUFFER_SIZE = 30720;
  
  void                  logMsg(const std::string &message)
  {
    std::cout << message << std::endl;
  }

  void                  exitWithError(const std::string &errorMessage)
  {
    logMsg("ERROR: " + errorMessage);
    exit(1);
  }
}

namespace	http
{
  TcpServer::TcpServer(std::string ip_address, int port) : m_ip_address(ip_address), m_port(port), m_socket(),
							   m_new_socket(), m_incomingMessage(), m_socketAddress(),
							   m_socketAddress_len(sizeof(m_socketAddress)),
							   m_serverMessage(buildResponse())
  {
    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port = htons(m_port);
    m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
    if (startServer() != 0)
      {
	std::ostringstream ss;
	ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
	logMsg(ss.str());
      }
  }

  TcpServer::~TcpServer()
  {
    closeServer();
  }

  int		TcpServer::startServer()
  {
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0)
      {
	exitWithError("Cannot create socket");
	return m_socket;
      }

    if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
      {
	exitWithError("Cannot connect socket to address");
	return 1;
      }
    return 0;
  }

  void		TcpServer::closeServer()
  {
    close(m_socket);
    close(m_new_socket);
    exit(0);
  }

  void	TcpServer::startListen()
  {
    if (listen(m_socket, 20) < 0)
    {
        exitWithError("Socket listen failed");
    }

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " 
        << inet_ntoa(m_socketAddress.sin_addr) 
        << " PORT: " << ntohs(m_socketAddress.sin_port) 
        << " ***\n\n";
    logMsg(ss.str());

    int	bytesReceived;

    while (true)
      {
	logMsg("====== Waiting for a new connection ======\n\n\n");
	acceptConnection(m_new_socket);

	char buffer[BUFFER_SIZE] = {0};
	bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
	if (bytesReceived < 0)
	  {
	    exitWithError("Failed to read bytes from client socket connection");
	  }

	std::ostringstream ss;
	ss << "-------- Received Request from client --------\n\n";
	logMsg(ss.str());

	// TODO: parse client request -> send appropriate response
	
	sendResponse();
	close(m_new_socket);
      }
  }

  void	TcpServer::acceptConnection(int &new_socket)
  {
    new_socket = accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);

    if (new_socket < 0)
      {
	std::ostringstream ss;
	ss << "Server failed to accept incoming connection from ADRESS: "
	   << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: "
	   << ntohs(m_socketAddress.sin_port);
	exitWithError(ss.str());
      }
  }

  std::string		TcpServer::buildResponse()
  {
    std::ostringstream	ss;
    std::ifstream	inFile;
    std::string		contents;
    std::string		ret;

    // Opening the html file, defined in http_tcpServer_linux.cpp
    inFile.open(BLOG_HOME_PAGE);

    if (!inFile)
      {
	std::string_view errMsg = "<!DOCTYPE html><html lang=\"en\"><body><h1> ERROR </h1><p> Could not open file </p></body></html>";
	std::cerr << "Unable to open file: \"" << BLOG_HOME_PAGE << "\"\n";
	ss << "HTTP/1.1 500 ERROR\nContent-Type: text/html\nContent-Length: " << errMsg.size() << "\n\n" << errMsg;
	return ss.str();
      }
    while (getline(inFile, contents))
      {
	ret.append(contents);
      }

    // Closing the html file
    inFile.close();
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << ret.size() << "\n\n" << ret;
    return ss.str();
  }

  void		TcpServer::sendResponse()
  {
    long	bytesSent;

    bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());

    if (bytesSent == m_serverMessage.size())
      {
	logMsg("------ Server Response sent to client ------\n\n");
      }
    else
      {
	logMsg("Error sending response to client\n");
      }
  }
} // namespace http

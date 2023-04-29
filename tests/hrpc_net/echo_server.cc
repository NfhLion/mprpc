#include "TcpServer.h"
#include "EventLoop.h"
#include "TcpConnection.h"

#include <iostream>

using namespace hrpc;
using namespace hrpc::net;

void onConnection(const TcpConnectionPtr &conn)
{
    std::cout << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN") << std::endl;;
    
}

void onMessage(const TcpConnectionPtr &conn)
{
    muduo::string msg(conn->readAll());
    std::cout << conn->name() << " echo " << msg.size() << " bytes, "
                     << "data received at " << msg << std::endl;
    
    conn->send(msg);
}

int main(int argc, const char *argv[])
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8988);
    TcpServer server(&loop, addr, "echo");
    server.setConnectionCallback(&onConnection);
    server.setMessageCallback(&onMessage);
    server.start();
    loop.loop();
    return 0;
}

//
//  server.cpp
//
//  To Compile: g++ -o server.exe server.cpp PracticalSocket.cpp
//  To Run: ./server.exe 'Server's Port Number' (1024)

#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()

void handleClient(TCPSocket *sock); // TCP client handling function

using namespace std;
const unsigned int RCVBUFSIZE = 32;    // Size of receive buffer
int main(int argc, char *argv[])
{
    if (argc != 2) //Test for correct number of arguments
    {
        cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
        exit(1);
    }
    unsigned short servPort = atoi(argv[1]); //First arg: local port
    try
    {
        cout<<"server running"<<endl;
        TCPServerSocket servSock(servPort);  //Server Socket object
        for (;;) {handleClient(servSock.accept()); }  // Wait for a client to connect
        
    }
    catch (SocketException &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
}
void handleClient(TCPSocket *sock)
{
    cout << "Handling client ";
    try{cout << sock->getForeignAddress() << ": ";}
    catch (SocketException e){cerr << "Unable to get foreign address" << endl;}
    
    try{cout << sock->getForeignPort() << endl;}
    catch (SocketException e){cerr << "Unable to get foreign port" << endl;}
    
    // Send received string and receive again until the end of transmission
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;
    //cout<<"test"<<endl;
    while ((recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE)) > 0)
    {
        //Zero means// end of transmission
        cout<<echoBuffer<<endl;
        //Echo message back to client
        sock->send(echoBuffer, recvMsgSize);
    }
    delete sock; //done with the socket, so you can delete it
}

   
    

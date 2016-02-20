//
//  Server.cpp
//  Created by Stephanie Smith on 1/14/16.
//
//  Description: server (running on NUC) will communicate with arduino to control robot
//  To Compile: g++ -o Server.exe Server.cpp PracticalSocket.cpp arduino-serial-lib.cpp
//  To Run: ./Server.exe 'Server's Port Number'

#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include "Server.hpp"
#include "arduino-serial-lib.h" //for writeArduino()
#include <vector>             // also for split(string)
#include <sstream>            // for split(string)
#include <string>

#define BAUDRATE 9600 // default baudrate
#define BUF_MAX 256   // default max buffer size
#define TIMEOUT 5000 // default timeout

const unsigned int RCVBUFSIZE = 50; // Size of receive buffer

void handleClient(TCPSocket *sock, int fd); //prototypes
vector<string> splitString(string str);
void arduinoSend(const char *message, TCPSocket *socket);
char* writeToArduino(const int mode, const int powerLevel, int fd);
int arduinoInit();

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
        int fd=arduinoInit();
        cout<<"server running"<<endl;
        TCPServerSocket servSock(servPort);  //Server Socket object
        for (;;) {handleClient(servSock.accept(), fd); }  // Wait for a client to connect
    
    }
    catch (SocketException &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
    return 0;
}
/**
 * Input: socket opened by server
 **/
void handleClient(TCPSocket *sock, int fd)
{
    cout << "Handling client ";
    try{cout << sock->getForeignAddress() << ": ";}
    catch (SocketException e){cerr << "Unable to get foreign address" << endl;}
    
    try{cout << sock->getForeignPort() << endl;}
    catch (SocketException e){cerr << "Unable to get foreign port" << endl;}
    
    // Send received string and receive again until the end of transmission
    char buffer[RCVBUFSIZE];
    int recvMsgSize;
    memset(buffer, 0, RCVBUFSIZE); //clearing the buffer for next time
    while ((recvMsgSize = sock->recv(buffer, RCVBUFSIZE)) > 0) //Zero means end of transmission
    {
        cout<<"Server received: "<<buffer<<endl;
        string str = string(buffer);
        if(str.substr(0,4).compare("ard-")==0) //see if it was a message intended for the arduino
        {
            str=str.substr(4, recvMsgSize);
            const char *message = str.c_str(); //stripping the 'ard-' off of the string
            vector<string> ard_control=splitString(message); //break up the line
            int mode;
            int powerLevel;
            if(ard_control.size()==1|| ard_control.size() ==2 ) //if the arduino commands were correctly specified
            {
                mode = std::stoi(ard_control[0], nullptr, 10); //get the mode
                powerLevel = ard_control.size()==2 ? std::stoi(ard_control[1], nullptr, 10) : -1; //if no power level was specified send a -1 power
                char *ardMessage=writeToArduino(mode, powerLevel, fd); //write to the arduino
            }
            else{cout<<"FIXX THIS!"<<endl;}
            sock->send(message, strlen(message)); //sending arduino message back to client
        }
        else{sock->send(buffer, recvMsgSize);} //send message as is
    }
    delete sock; //done with the socket, so you can delete it
}
/**
 * Function initializes connection between arduino & NUC
 **/
int arduinoInit()
{
    int fd=-1; //status

    const char* port="/dev/cu.usbmodem1411"; //default for stephanie's laptop
    fd = serialport_init(port, BAUDRATE); //opening port
    (fd == -1) ? cout<< "couldn't open port" << endl : cout<< "opened port " << port << endl;
    serialport_flush(fd);
    return fd;
}
/**
 * Input: message to send to arduino
 * Output: message sent to arduino (pass this back to the client)
 */

char* writeToArduino(const int mode, const int powerLevel, int fd)
{
    cout<<"mode "<<mode << " powerLevel: "<<powerLevel<<endl;
    //int fd=-1; //status
    const char* port="/dev/cu.usbmodem1411"; //default for stephanie's laptop
    //const char* test_message="hello arduino!";
    int rc; //status
    
    char* buf=(char*)malloc(BUF_MAX*sizeof(char));
    char eolchar = '\n'; //when receive messages line by line
    
    printf("setting baud rate to %d\n", BAUDRATE);
    cout<<"setting serial port to: "<<port<<endl;
    
   
    // Accounts for little-endian and big-endian machines
    char highMode = (mode >> (8*3)) & 0xff;
    char lowMode = (mode >> (8*0)) & 0xff;
    char modeToSend = highMode >  lowMode ? highMode : lowMode;
    printf("modetosend: %c\n", modeToSend+48);
    rc = serialport_writebyte(fd, modeToSend); //writing the to the arduino
    if(rc==-1) perror("error writing");
    
    // Send over the power level if it is valid
    if(powerLevel >= 0) {
        // Accounts for little-endian and big-endian machines
        char highPower = (powerLevel >> (8*3)) & 0xff;
        char lowPower = (powerLevel >> (8*0)) & 0xff;
        char powerToSend = highPower > lowPower ? highPower : lowPower;
        rc = serialport_writebyte(fd, powerToSend); //writing the to the arduino
        if(rc==-1) perror("error writing");
    }
    
    if( fd == -1) perror("serial port not opened");
    memset(buf,0,BUF_MAX); //clearing buffer
    serialport_read_until(fd, buf, eolchar, BUF_MAX, TIMEOUT); //'Arduino received <message>'
    printf("in write arduino: %s\n", buf); //printing out what server receives from arduino
   
    return buf;
    
    //WHEN YOU DON'T HAVE AN ARDUINO JUST USE THIS
    //have to do some shit to get char*
    /*string arduinoMess = "Arduino received ";
    char *tempMess = new char[arduinoMess.length() + 1];
    strcpy(tempMess, arduinoMess.c_str());
    return strcat(tempMess, message); //returning message received from arduino (test)
    return NULL;*/
}
/**
 * description: splits the string at the ' '
 * input: a single string
 * output: a vector of strings --> where each element is a word
 **/
vector<string> splitString(string str) //this may be used on the server side too
{
    string buf; // Have a buffer string
    stringstream ss(str); // Insert the string into a stream
    vector<string> tokens; // Create vector to hold our words
    
    while (ss >> buf)
        tokens.push_back(buf);
    
    for(int i=0; i<tokens.size(); i++)
    {
        cout<<tokens[i]<<endl;
    }
    return tokens;
}

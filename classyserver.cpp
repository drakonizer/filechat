#include <iostream>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>


using namespace std;

class Server{
private:
    struct sockaddr_in server_addr;
    int sze, clientCount;

    WSADATA wsa;

    int initWinsock();
    void createSock();
    int bindSock();
    void killSock();
	void sendRecvMsg();

public:
    void InitializeWinSock();
    void CreateSocket();
    void BindSocket();

    SOCKET client, server;
    int port = 1500;
    int BUFSIZE =  1024;
    bool isExit = false;
    char buffer[1024];

};

void Server::InitializeWinSock(){
    initWinsock();
}

void Server::CreateSocket(){
    createSock();
}

void Server::BindSocket(){
    bindSock();
	sendRecvMsg();
}

int Server::initWinsock(){
    cout << "[*] Initialising Winsock." << endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cout << "[!] Failed. Error Code:" << WSAGetLastError();
        return 1;
    }

    cout << "[:)] Initialised!" << endl;
}

void Server::createSock(){
    if((client = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cout << "[!] Could not create socket: " << WSAGetLastError();
    }

    cout << "[*] Socket server has been created.\n" << endl;
}

void Server::killSock(){
        cout << "[!] Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        closesocket(server);
        WSACleanup();
        isExit = false;
        exit(1);
}

int Server::bindSock(){
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY); // any address can attempt to connect
    server_addr.sin_port = htons(port);


    if ((bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
    {
        cout << "[!] Error binding connection, the socket has already been established..." << endl;
        return -1;
    }


    sze = sizeof(server_addr);
    cout << "[*] Looking for clients." << endl;

    listen(client, 1);


    clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&sze);

    // first check if it is valid or not
    if (server < 0)
        cout << "[!] Error accepting client connection." << endl;
}

void Server::sendRecvMsg()
{
	while (server > 0)
    {
        strcpy(buffer, "[:)] Server connected!");
        send(server, buffer, BUFSIZE, 0);
        cout << "[*] Connected with the client. " << clientCount << endl;
        cout << "[*] Enter # (i.e. Shift + 3), to end the connection." << endl;



        cout << "Client: ";
        do {
            recv(server, buffer, BUFSIZE, 0);

            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
                send(server, buffer, BUFSIZE, 0);
                if (*buffer == '#') {
                    send(server, buffer, BUFSIZE, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
                recv(server, buffer, BUFSIZE, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);
		killSock();
    }
}

int main(){
    Server s;
    s.InitializeWinSock();
    s.CreateSocket();
    s.BindSocket();

    return 0;
}

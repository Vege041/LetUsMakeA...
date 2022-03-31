//client.cpp
#include<iostream>  
#include<winsock.h>  
#include<string.h>  
#pragma comment(lib, "ws2_32.lib")   
//加载ws2_32.lib库  
using namespace std;  
int main() {  
  
    WSADATA wsaData; //存放被WSAStartup函数调用后返回的Windows Sockets数据的数据结构  
    WSAStartup(MAKEWORD(2, 2), &wsaData);//声明使用socket2.2版本  
    //创建套接字  
    SOCKET ClientSocket;  
    //地址类型为AD_INET，服务类型为流式(SOCK_STREAM)，协议采用TCP  
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if (ClientSocket == INVALID_SOCKET)  
    {  
        cout << "套接字创建失败";  
        WSACleanup();  
        return 0;  
    }  
  
    SOCKADDR_IN ServerAddr;  
    ServerAddr.sin_family = AF_INET;     //指定IP格式  
    USHORT uPort = 8888;                 //服务器监听端口  
    ServerAddr.sin_port = htons(uPort);   //绑定端口号  
    // ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  
    ServerAddr.sin_addr.S_un.S_addr = inet_addr("192.168.3.14");  
    char client_name[32] = { 0 };  
    cout << "请输入你的名字：";  
    cin.getline( client_name,32);  
    connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));  
    cout << "连接成功" << endl;  
  
    /*等待连接*/  
    char server_name[32] = { 0 };  
    char buffer[1024] = { 0 };  
    int relen = 0;  
    int selen = 0;  
    /*通过建立的连接进行通信*/  
    //发送和接受客户端与服务端的名字  
    selen = send(ClientSocket, client_name, strlen(client_name), 0);  
    relen = recv(ClientSocket, server_name, sizeof(server_name), 0);  
    /*server_name[relen] = '\0';*/  
    while (1)  
    {  
        while (1) {  
            cout << client_name << ": ";  
            memset(buffer, 0, sizeof(buffer));  
            cin.getline(buffer,sizeof(buffer));  
            if (strcmp(buffer, "exit") == 0)  
            {  
                selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
                cout << "程序将在3秒后退出" << endl;  
                Sleep(3000);  
                closesocket(ClientSocket);  
                WSACleanup();  
                return 0;  
            }  
            else if (strcmp(buffer, "over") == 0)  
            {  
                selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
                break;  
            }  
            selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
        }  
        while (1) {  
            memset(buffer, 0, sizeof(buffer));//在每次交互前，把之前的buf数据清空，避免缓冲区冗余  
            relen = recv(ClientSocket, buffer, sizeof(buffer), 0);  
            //buffer[relen] = '\0';//接受消息  
            if (strcmp(buffer, "exit") == 0)//输入消息含有exit便退出  
            {  
                cout << buffer <<"程序将在3秒后退出" <<endl;  
                Sleep(3000);  
                closesocket(ClientSocket);  
                WSACleanup();  
                return 0;  
            }  
            else if (strcmp(buffer, "over") == 0)  
            {  
                break;  
            }  
            cout << server_name << ": ";  
            cout << buffer << endl;  
        }  
    }  
}  
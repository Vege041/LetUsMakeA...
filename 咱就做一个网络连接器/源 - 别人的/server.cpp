//server.cpp
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
   SOCKET ServerSocket;  
   //地址类型为AD_INET，服务类型为流式(SOCK_STREAM)，协议采用TCP  
   ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
   if (ServerSocket == INVALID_SOCKET)  
   {  
       cout << "套接字创建失败";  
       WSACleanup();  
       return 0;  
   }  
 
   SOCKADDR_IN ServerAddr;  
   ServerAddr.sin_family = AF_INET;     //指定IP格式  
   USHORT uPort = 8888;                 //服务器监听端口  
   ServerAddr.sin_port = htons(uPort);   //绑定端口号  
   ServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;  
   if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)  //建立捆绑  
   {  
       cout << "绑定失败";  
       closesocket(ServerSocket);  
       return 0;  
   }  
   char server_name[32] = { 0 };  
   cout << "请输入你的名字：";  
   cin.getline(server_name,32);  
 
   /*开始监听*/  
   listen(ServerSocket, 1);  
 
   /*等待连接*/  
   char client_name[32] = { 0 };  
   SOCKET ClientSocket;  
   SOCKADDR_IN ClientAddr;  
   int ClientAddrlen = sizeof(ClientAddr);  
   ClientSocket = accept(ServerSocket,(SOCKADDR*)&ClientAddr,&ClientAddrlen);  
   cout << "等待连接...\n";  
   if (ClientSocket == INVALID_SOCKET)  
   {  
       cout << "客户端发出请求，服务器接收请求失败：\n" << WSAGetLastError();  
       closesocket(ServerSocket);  
       WSACleanup();  
       return 0;  
   }  
   else  
   {  
       cout << "客服端与服务器建立连接成功：\n" ;  
   }  
   char buffer[1024] = { 0 };  
   int relen = 0;  
   int selen = 0;  
   /*通过建立的连接进行通信*/  
   //发送和接受客户端与服务端的名字  
   selen=send(ClientSocket, server_name, strlen(server_name), 0);  
   relen=recv(ClientSocket, client_name, sizeof(client_name), 0);  
   client_name[relen] = '\0';  
   while (1)  
   {  
       while (1)  
       {  
           memset(buffer, 0, sizeof(buffer));//在每次交互前，把之前的buf数据清空，避免缓冲区冗余  
           relen = recv(ClientSocket, buffer, sizeof(buffer), 0);  
           /*buffer[relen] = '\0';*/  
           //接受消息  
           if (strcmp(buffer, "exit") == 0)//输入消息含有exit便退出  
           {  
               cout << "程序将在3秒后退出" << endl;  
               Sleep(3000);  
               closesocket(ServerSocket);  
               closesocket(ClientSocket);  
               WSACleanup();  
               return 0;  
           }  
           else if (strcmp(buffer, "over") == 0)  
           {  
               break;  
           }  
           cout << client_name << ": ";  
           cout << buffer << endl;  
       }  
       while (1) {  
           memset(buffer, 0, sizeof(buffer));  
           cout << server_name << ": ";  
           cin.getline(buffer,1024);  
           //发出消息  
           if (strcmp(buffer, "over") == 0)  
           {  
               selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
               break;  
           }  
              else if(strcmp(buffer, "exit") == 0)  
              {  
                  selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
                  cout << "程序将在3秒后退出" << endl;  
                  Sleep(3000);  
                  closesocket(ServerSocket);  
                  closesocket(ClientSocket);  
                  WSACleanup();  
                  return 0;  
              }  
              selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
          }  
      }  
  }  

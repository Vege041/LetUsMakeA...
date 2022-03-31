//server.cpp
#include<iostream>  
#include<winsock.h>  
#include<string.h>  
#pragma comment(lib, "ws2_32.lib")   
//����ws2_32.lib��  
using namespace std;  
int main() {  
  
    WSADATA wsaData; //��ű�WSAStartup�������ú󷵻ص�Windows Sockets���ݵ����ݽṹ  
   WSAStartup(MAKEWORD(2, 2), &wsaData);//����ʹ��socket2.2�汾  
   //�����׽���  
   SOCKET ServerSocket;  
   //��ַ����ΪAD_INET����������Ϊ��ʽ(SOCK_STREAM)��Э�����TCP  
   ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
   if (ServerSocket == INVALID_SOCKET)  
   {  
       cout << "�׽��ִ���ʧ��";  
       WSACleanup();  
       return 0;  
   }  
 
   SOCKADDR_IN ServerAddr;  
   ServerAddr.sin_family = AF_INET;     //ָ��IP��ʽ  
   USHORT uPort = 8888;                 //�����������˿�  
   ServerAddr.sin_port = htons(uPort);   //�󶨶˿ں�  
   ServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;  
   if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)  //��������  
   {  
       cout << "��ʧ��";  
       closesocket(ServerSocket);  
       return 0;  
   }  
   char server_name[32] = { 0 };  
   cout << "������������֣�";  
   cin.getline(server_name,32);  
 
   /*��ʼ����*/  
   listen(ServerSocket, 1);  
 
   /*�ȴ�����*/  
   char client_name[32] = { 0 };  
   SOCKET ClientSocket;  
   SOCKADDR_IN ClientAddr;  
   int ClientAddrlen = sizeof(ClientAddr);  
   ClientSocket = accept(ServerSocket,(SOCKADDR*)&ClientAddr,&ClientAddrlen);  
   cout << "�ȴ�����...\n";  
   if (ClientSocket == INVALID_SOCKET)  
   {  
       cout << "�ͻ��˷������󣬷�������������ʧ�ܣ�\n" << WSAGetLastError();  
       closesocket(ServerSocket);  
       WSACleanup();  
       return 0;  
   }  
   else  
   {  
       cout << "�ͷ�����������������ӳɹ���\n" ;  
   }  
   char buffer[1024] = { 0 };  
   int relen = 0;  
   int selen = 0;  
   /*ͨ�����������ӽ���ͨ��*/  
   //���ͺͽ��ܿͻ��������˵�����  
   selen=send(ClientSocket, server_name, strlen(server_name), 0);  
   relen=recv(ClientSocket, client_name, sizeof(client_name), 0);  
   client_name[relen] = '\0';  
   while (1)  
   {  
       while (1)  
       {  
           memset(buffer, 0, sizeof(buffer));//��ÿ�ν���ǰ����֮ǰ��buf������գ����⻺��������  
           relen = recv(ClientSocket, buffer, sizeof(buffer), 0);  
           /*buffer[relen] = '\0';*/  
           //������Ϣ  
           if (strcmp(buffer, "exit") == 0)//������Ϣ����exit���˳�  
           {  
               cout << "������3����˳�" << endl;  
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
           //������Ϣ  
           if (strcmp(buffer, "over") == 0)  
           {  
               selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
               break;  
           }  
              else if(strcmp(buffer, "exit") == 0)  
              {  
                  selen = send(ClientSocket, buffer, sizeof(buffer), 0);  
                  cout << "������3����˳�" << endl;  
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

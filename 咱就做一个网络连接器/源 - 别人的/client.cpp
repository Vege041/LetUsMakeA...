//client.cpp
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
    SOCKET ClientSocket;  
    //��ַ����ΪAD_INET����������Ϊ��ʽ(SOCK_STREAM)��Э�����TCP  
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if (ClientSocket == INVALID_SOCKET)  
    {  
        cout << "�׽��ִ���ʧ��";  
        WSACleanup();  
        return 0;  
    }  
  
    SOCKADDR_IN ServerAddr;  
    ServerAddr.sin_family = AF_INET;     //ָ��IP��ʽ  
    USHORT uPort = 8888;                 //�����������˿�  
    ServerAddr.sin_port = htons(uPort);   //�󶨶˿ں�  
    // ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  
    ServerAddr.sin_addr.S_un.S_addr = inet_addr("192.168.3.14");  
    char client_name[32] = { 0 };  
    cout << "������������֣�";  
    cin.getline( client_name,32);  
    connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));  
    cout << "���ӳɹ�" << endl;  
  
    /*�ȴ�����*/  
    char server_name[32] = { 0 };  
    char buffer[1024] = { 0 };  
    int relen = 0;  
    int selen = 0;  
    /*ͨ�����������ӽ���ͨ��*/  
    //���ͺͽ��ܿͻ��������˵�����  
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
                cout << "������3����˳�" << endl;  
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
            memset(buffer, 0, sizeof(buffer));//��ÿ�ν���ǰ����֮ǰ��buf������գ����⻺��������  
            relen = recv(ClientSocket, buffer, sizeof(buffer), 0);  
            //buffer[relen] = '\0';//������Ϣ  
            if (strcmp(buffer, "exit") == 0)//������Ϣ����exit���˳�  
            {  
                cout << buffer <<"������3����˳�" <<endl;  
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
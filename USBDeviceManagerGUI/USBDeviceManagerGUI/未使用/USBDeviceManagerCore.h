#pragma once

#include "USBDeviceManagerGUI.h"

#include <Windows.h>
#include <fileapi.h>

#define  WM_COPYFILE_NOTIFY WM_USER+118+2  //自定义的windows消息，用来通知窗口

class FileCopy
{

public:
    FileCopy(void);
    FileCopy(LPTSTR lpExistingPathName, LPTSTR lpNewPathName, HWND hNotifyWnd = NULL);
    ~FileCopy(void);

    BOOL Init(LPTSTR lpExistingPathName, LPTSTR lpNewPathName, HWND hNotifyWnd = NULL);
    BOOL Begin();
    void Pause();
    void Resume();
    void Cancel();
    //停止复制
    //void Stop();     //Stop();结束复制过程，但保存已经复制的内容，Cancel();会删除已复制的内容。

    //等待复制结束，用来实现“同步”效果,调用此函数后线程会阻塞，直到复制完成或取消。
    void WaitForEnd();
    //获取复制进度
    float GetProgress();
    //获取文件总大小，函数返回方式模仿 API GetFileSize();   一般情况下超过4GB的文件不多
    //，lpFileSizeHigh直接忽视就行了
    DWORD GetTotalFileSize(DWORD* lpFileSizeHigh = NULL);
    //获取已经复制的字节数;
    DWORD GetBytesTransferred(DWORD* lpTransferredHigh = NULL);

    //设置复制速度
    void SetSpeed(int iSpeed);
    void SetHNotifyWnd(HWND wnd) { hNotifyWnd = wnd; }
    void SetPNotifyWnd(LPVOID wnd) { pNotifyWnd = wnd; }
public:
    HWND mainWindow;
    CWnd* pMainWnd;
private:
    LPTSTR lpExistingPathName;           //源文件
    LPTSTR lpNewPathName;                //目标文件
    int iSpeedControl;                   //速度控制的变量
    BOOL bCancel;                        //取消标志，用来传入CopyFileEx的回调函数
    HANDLE hEvent_Pause;                 //“复制暂停”事件
    float fCopyProgress;                 //复制进度
    HWND hNotifyWnd;                     //接受通知消息的窗口
    LPVOID pNotifyWnd;                    //ditto
    HANDLE hEvent_End;                   //“复制结束”事件
    HANDLE hThread_Copy;                 //线程句柄
    LARGE_INTEGER totalFileSize;         //总的文件长度 
    LARGE_INTEGER totalBytesTransferred; //已经复制的字节数
    int ret_PGR;                         //作为CopyProgressRoutine的返回值，此参数未用
    void Initialize();                   //初始化内部数据：各种句柄和变量;
    //线程函数，在线程中调用CopyFileEx实现文件复制
    static DWORD WINAPI ThreadProc_Copy(LPVOID lpParam);


    //CopyFileEx的回调函数，在此函数中实现文件复制过程的控制。
    static DWORD CALLBACK CopyProgressRoutine(
        LARGE_INTEGER TotalFileSize,
        LARGE_INTEGER TotalBytesTransferred,
        LARGE_INTEGER StreamSize,
        LARGE_INTEGER StreamBytesTransferred,
        DWORD dwStreamNumber,
        DWORD dwCallbackReason,
        HANDLE hSourceFile,
        HANDLE hDestinationFile,
        LPVOID lpData
    );
};


#pragma once

#include "USBDeviceManagerGUI.h"

#include <Windows.h>
#include <fileapi.h>

#define  WM_COPYFILE_NOTIFY WM_USER+118+2  //�Զ����windows��Ϣ������֪ͨ����

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
    //ֹͣ����
    //void Stop();     //Stop();�������ƹ��̣��������Ѿ����Ƶ����ݣ�Cancel();��ɾ���Ѹ��Ƶ����ݡ�

    //�ȴ����ƽ���������ʵ�֡�ͬ����Ч��,���ô˺������̻߳�������ֱ��������ɻ�ȡ����
    void WaitForEnd();
    //��ȡ���ƽ���
    float GetProgress();
    //��ȡ�ļ��ܴ�С���������ط�ʽģ�� API GetFileSize();   һ������³���4GB���ļ�����
    //��lpFileSizeHighֱ�Ӻ��Ӿ�����
    DWORD GetTotalFileSize(DWORD* lpFileSizeHigh = NULL);
    //��ȡ�Ѿ����Ƶ��ֽ���;
    DWORD GetBytesTransferred(DWORD* lpTransferredHigh = NULL);

    //���ø����ٶ�
    void SetSpeed(int iSpeed);
    void SetHNotifyWnd(HWND wnd) { hNotifyWnd = wnd; }
    void SetPNotifyWnd(LPVOID wnd) { pNotifyWnd = wnd; }
public:
    HWND mainWindow;
    CWnd* pMainWnd;
private:
    LPTSTR lpExistingPathName;           //Դ�ļ�
    LPTSTR lpNewPathName;                //Ŀ���ļ�
    int iSpeedControl;                   //�ٶȿ��Ƶı���
    BOOL bCancel;                        //ȡ����־����������CopyFileEx�Ļص�����
    HANDLE hEvent_Pause;                 //��������ͣ���¼�
    float fCopyProgress;                 //���ƽ���
    HWND hNotifyWnd;                     //����֪ͨ��Ϣ�Ĵ���
    LPVOID pNotifyWnd;                    //ditto
    HANDLE hEvent_End;                   //�����ƽ������¼�
    HANDLE hThread_Copy;                 //�߳̾��
    LARGE_INTEGER totalFileSize;         //�ܵ��ļ����� 
    LARGE_INTEGER totalBytesTransferred; //�Ѿ����Ƶ��ֽ���
    int ret_PGR;                         //��ΪCopyProgressRoutine�ķ���ֵ���˲���δ��
    void Initialize();                   //��ʼ���ڲ����ݣ����־���ͱ���;
    //�̺߳��������߳��е���CopyFileExʵ���ļ�����
    static DWORD WINAPI ThreadProc_Copy(LPVOID lpParam);


    //CopyFileEx�Ļص��������ڴ˺�����ʵ���ļ����ƹ��̵Ŀ��ơ�
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


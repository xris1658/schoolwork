#include "pch.h"
#include "USBDeviceManagerCore.h"
#include "USBDeviceManagerGUIDlg.h"
#include "CCopyStatus.h"

#include <shobjidl_core.h>

#pragma region Implements: Class FileCopy
FileCopy::FileCopy(void)
{
	Initialize();
}

FileCopy::FileCopy(LPTSTR lpExistingPathName,LPTSTR lpNewPathName,HWND hNotifyWnd)
{
	Init(lpExistingPathName,lpNewPathName,hNotifyWnd);
}

FileCopy::~FileCopy(void)
{
	CloseHandle(hEvent_End);
	CloseHandle(hEvent_Pause);
	CloseHandle(hThread_Copy);
}

void FileCopy::Initialize()
{
	bCancel=FALSE;
	hNotifyWnd=NULL;
	fCopyProgress=0;
	hEvent_Pause=NULL;
	iSpeedControl=-1;
	totalFileSize.HighPart=0;
	totalFileSize.LowPart=0;
	totalBytesTransferred.HighPart=0;
	totalBytesTransferred.LowPart=0;
	hThread_Copy=NULL;

	ret_PGR=PROGRESS_CONTINUE;

	//��ʼ�� �����ƽ������¼�        �ֶ�����  ���ź�
	if(hEvent_End!=NULL)
		CloseHandle(hEvent_End);
	hEvent_End=CreateEvent(NULL,TRUE,FALSE,NULL);

	//��ʼ�� ��������ͣ���¼���       �ֶ�����  ���ź�״̬
	if(hEvent_Pause!=NULL)
		CloseHandle(hEvent_Pause);
	hEvent_Pause=CreateEvent(NULL,TRUE,TRUE,NULL);
}

BOOL FileCopy::Init(LPTSTR lpExistingPathName, LPTSTR lpNewPathName, HWND hNotifyWnd/* =NULL */)
{
	Initialize();
	this->lpExistingPathName=lpExistingPathName;
	this->lpNewPathName=lpNewPathName;
	this->hNotifyWnd=hNotifyWnd;

	HANDLE hFile=CreateFile(lpExistingPathName,GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE==hFile)
		return FALSE;
	if(!GetFileSizeEx(hFile,&totalFileSize))
		return FALSE;

	return TRUE;
}

BOOL FileCopy::Begin()
{
	//���߳��е���CopyFileEx����,Ϊ�˱�����ķ�װ�ԣ�
	//�̺߳�����д����ľ�̬��Ա�������˴�����thisָ��Ϊ�˷��ʳ�Ա����
	//CopyFileEx�Ļص�����Ҳ������������ʵ�ֵġ�
	hThread_Copy=CreateThread(NULL, 0, ThreadProc_Copy, this, 0, NULL);
	if(NULL==hThread_Copy)
	{
		return FALSE;
	}

	return TRUE;
}


DWORD WINAPI FileCopy::ThreadProc_Copy(LPVOID lpParam)
{
	//��õ�ǰ���ʵ���е��������
	HWND hNotifyWnd=((FileCopy*)lpParam)->hNotifyWnd;
    LPVOID pNotifyWnd=((FileCopy*)lpParam)->pNotifyWnd;
	LPTSTR lpExistingPathName=((FileCopy*)lpParam)->lpExistingPathName;
	LPTSTR lpNewPathName=((FileCopy*)lpParam)->lpNewPathName;

	//���ú���API����CopyFileEx�������ļ�
	BOOL bSucceed=CopyFileEx(lpExistingPathName,lpNewPathName,
		CopyProgressRoutine,
		lpParam,&(((FileCopy*)lpParam)->bCancel),
		COPY_FILE_ALLOW_DECRYPTED_DESTINATION|COPY_FILE_COPY_SYMLINK|COPY_FILE_FAIL_IF_EXISTS);

	//�����������򴰿ڷ���֪ͨ��Ϣ��
	if(hNotifyWnd!=NULL || pNotifyWnd!=NULL)
	{
		if(bSucceed)
		{
			PostMessage(hNotifyWnd, WM_COPYFILE_NOTIFY, 1, (LPARAM)lpExistingPathName);
		}
		else
		{
			PostMessage(hNotifyWnd, WM_COPYFILE_NOTIFY, 0, (LPARAM)lpExistingPathName);
		}
	}

	//���������������¼����ó��ź�״̬
	//SetEvent(((FileCopy*)lpParam)->hEvent_End);
	return 0;
}

DWORD CALLBACK FileCopy::CopyProgressRoutine(
	LARGE_INTEGER TotalFileSize,
	LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,
	LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,
	DWORD dwCallbackReason,
	HANDLE hSourceFile,
	HANDLE hDestinationFile,
	LPVOID lpData
	)
{
//�����ļ����Ⱥ��Ѿ����Ƶ�������
    FileCopy* currentFileCopy = (FileCopy*)lpData;
    BOOL bCancel = currentFileCopy->bCancel;
    HWND hNotifyWnd = currentFileCopy->hNotifyWnd;
    CCopyStatus* pNotifyWnd = (CCopyStatus*)(currentFileCopy->pNotifyWnd);
    HWND mainWindow = currentFileCopy->mainWindow;
    while (!(pNotifyWnd->ready)) {}
	currentFileCopy->totalFileSize=TotalFileSize;
	currentFileCopy->totalBytesTransferred=TotalBytesTransferred;
//���㸴�ƽ���
	currentFileCopy->fCopyProgress=TotalBytesTransferred.QuadPart*1.0/TotalFileSize.QuadPart;

//ͨ���¼�����ʵ����ͣ��
	WaitForSingleObject(currentFileCopy->hEvent_Pause,INFINITE);

//ͨ��Sleep()�����Ƹ����ٶ�
	int iSpeed=currentFileCopy->iSpeedControl;
	if(iSpeed>=0)
		Sleep(iSpeed);
//������
    if (!bCancel)
    {
        CUSBDeviceManagerGUIDlg* pMainWnd = reinterpret_cast<CUSBDeviceManagerGUIDlg*>(currentFileCopy->pMainWnd);
        ITaskbarList3* pMainWndTaskbar = pMainWnd->m_pTl3;
        pNotifyWnd->UpdateValue(currentFileCopy->fCopyProgress);
        pMainWndTaskbar->SetProgressState(mainWindow, TBPF_NORMAL);
        pMainWndTaskbar->SetProgressValue(mainWindow, TotalBytesTransferred.QuadPart, TotalFileSize.QuadPart);
	    return PROGRESS_CONTINUE;
    }
    else
    {
        CUSBDeviceManagerGUIDlg* pMainWnd = reinterpret_cast<CUSBDeviceManagerGUIDlg*>(currentFileCopy->pMainWnd);
        ITaskbarList3* pMainWndTaskbar = pMainWnd->m_pTl3;
        pMainWndTaskbar->SetProgressValue(mainWindow, 0, 1);
        pMainWndTaskbar->SetProgressState(mainWindow, TBPF_NOPROGRESS);
        return PROGRESS_CANCEL;
    }
}

void FileCopy::Pause()
{
	ResetEvent(hEvent_Pause);
}

void FileCopy::Resume()
{
	SetEvent(hEvent_Pause);
}

void FileCopy::Cancel()
{
	bCancel=TRUE;
	Resume();		//�ָ���ͣ״̬�����߳���Ȼ������
}

void FileCopy::WaitForEnd()
{
	WaitForSingleObject(hEvent_End,INFINITE);
}

float FileCopy::GetProgress()
{
	return fCopyProgress;
}

DWORD FileCopy::GetTotalFileSize(DWORD* lpFileSizeHigh)
{
	if(lpFileSizeHigh)
		*lpFileSizeHigh=totalFileSize.HighPart;
	return totalFileSize.LowPart;
}

DWORD FileCopy::GetBytesTransferred(DWORD* lpTransferredHigh)
{
	if(lpTransferredHigh)
		*lpTransferredHigh=totalBytesTransferred.HighPart;
	return totalBytesTransferred.LowPart;
}

void FileCopy::SetSpeed(int iSpeed)
{
	iSpeedControl=iSpeed;
//ÿ���߳�Sleep()��ʱ�䲻����1000ms
	if(iSpeedControl>1000)
		iSpeedControl=1000;
}

#pragma endregion

#pragma region Implements: Process Info
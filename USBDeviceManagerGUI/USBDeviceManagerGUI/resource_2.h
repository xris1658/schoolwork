#pragma once
#include "resource.h"

#include <afxwin.h>

static CRect rectControls[13];
static DWORD id[13]=
{
    IDC_LIST_DEVICE,
    IDC_BUTTON_REFRESH,
    IDC_LABEL_NAME,
    IDC_LABEL_ALL,
    IDC_LABEL_USED,
    IDC_LABEL_AVAILABLE,
    IDC_PROGRESS_DISKSPACEINDICATOR,
    IDC_BUTTON_COPY,
    IDC_BUTTON_DELETE,
    IDC_BUTTON_ABLE,
    IDC_BUTTON_PCB,
    IDC_BUTTON_OPTIONS,
    IDC_EDIT_INFO
};
static CString info[13]=
{
    TEXT("��ʾ���е� USB �洢�豸��"),
    TEXT("ˢ���豸�б�ͨ��������ڼ�⵽�豸�Ķ�ʱ�Զ�ˢ�¡�"),
    TEXT("��ʾ�豸�ľ����������š�"),
    TEXT("��ʾ�þ���ܴ�С��"),
    TEXT("��ʾ�þ��Ѿ�ʹ�õĿռ䡣"),
    TEXT("��ʾ�þ��ʣ��ռ䡣"),
    TEXT("����ʹ�������ֱ��չʾ��"),
    TEXT("���ļ����ļ��и��Ƶ� USB �洢�豸��"),
    TEXT("ɾ�� USB �洢�豸�е��ļ����ļ��С�"),
    TEXT("����ѡ��� USB �洢�豸����������Ҫʱ�ٴ������豸��"),
    TEXT("��ʾ�˳���Ľ�����Ϣ��"),
    TEXT("�����һЩ���á�"),
    TEXT("������ʾ�ؼ��ļ�Ҫ˵����")
};
static CString disableTypeInfo[3] =
{
    TEXT("��ȫɾ��Ӳ����\r\nֹͣ��USB�豸�Ĺ��硣\r\n�����ɹ��󼴿�ֱ�Ӱγ��豸��\r\n�ٴβ����豸���ɼ���ʹ�á�"),
    TEXT("ɾ���������ţ�\r\n����������Ź��ء�\r\n�����ɹ������ͨ������������ŵķ�ʽ����ʹ���豸��"),
    TEXT("�����豸��\r\n�����豸��ʹ��ֹͣ���С�\r\n�����ɹ������ͨ�����õķ�ʽ����ʹ���豸��")
};
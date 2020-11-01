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
    TEXT("显示所有的 USB 存储设备。"),
    TEXT("刷新设备列表。通常程序会在检测到设备改动时自动刷新。"),
    TEXT("显示设备的卷标和驱动器号。"),
    TEXT("显示该卷的总大小。"),
    TEXT("显示该卷已经使用的空间。"),
    TEXT("显示该卷的剩余空间。"),
    TEXT("磁盘使用情况的直观展示。"),
    TEXT("将文件或文件夹复制到 USB 存储设备。"),
    TEXT("删除 USB 存储设备中的文件或文件夹。"),
    TEXT("禁用选择的 USB 存储设备。可以在需要时再次启用设备。"),
    TEXT("显示此程序的进程信息。"),
    TEXT("程序的一些设置。"),
    TEXT("这里显示控件的简要说明。")
};
static CString disableTypeInfo[3] =
{
    TEXT("安全删除硬件：\r\n停止对USB设备的供电。\r\n操作成功后即可直接拔出设备。\r\n再次插入设备即可继续使用。"),
    TEXT("删除驱动器号：\r\n解除驱动器号挂载。\r\n操作成功后可以通过添加驱动器号的方式继续使用设备。"),
    TEXT("禁用设备：\r\n禁用设备，使其停止运行。\r\n操作成功后可以通过启用的方式继续使用设备。")
};
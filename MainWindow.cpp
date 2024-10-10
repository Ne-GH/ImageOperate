#include "pch.h"
#include "framework.h"
#include "MainApp.h"
#include "MainWindow.h"
#include "afxdialogex.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


MainWindow::MainWindow(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_IMAGEOPERATE_DIALOG, pParent), image_operator_(nl::ImageOperator()) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void MainWindow::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MainWindow, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_OPEN_IMAGE, &MainWindow::open_image)
END_MESSAGE_MAP()



BOOL MainWindow::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标


    menu_.LoadMenu(MAIN_MENU);
    SetMenu(&menu_);


    bind(image_operator_.get_show_window());

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void MainWindow::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR MainWindow::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void MainWindow::bind(HWND handle) {
    HWND hParent = ::GetParent(handle);
    ::SetParent(handle, GetDlgItem(IDC_MAIN_PIC)->m_hWnd);
    ::ShowWindow(hParent, SW_HIDE);
}


void MainWindow::open_image() {

}

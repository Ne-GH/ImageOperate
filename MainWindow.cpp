﻿
// ImageOperateDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MainApp.h"
#include "MainWindow.h"
#include "afxdialogex.h"

#include "resource.h"
#include <chrono>


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
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ZOOM_UP, &MainWindow::OnBnClickedZoomUp)
	ON_BN_CLICKED(IDC_ZOOM_IN, &MainWindow::OnBnClickedZoomIn)
	ON_BN_CLICKED(IDC_TO_GRAYSCALE, &MainWindow::OnBnClickedToGrayscale)
	ON_BN_CLICKED(IDC_TO_BINARY, &MainWindow::OnBnClickedToBinary)
END_MESSAGE_MAP()



BOOL MainWindow::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	


    menu_.LoadMenu(MAIN_MENU);
    SetMenu(&menu_);


    bind(image_operator_.get_show_window());



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void MainWindow::OnPaint() {
	if (IsIconic()) {
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

HCURSOR MainWindow::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}


void MainWindow::bind(HWND handle) {
    HWND hParent = ::GetParent(handle);
    ::SetParent(handle, GetDlgItem(IDC_MAIN_PIC)->m_hWnd);
    ::ShowWindow(hParent, SW_HIDE);
    ::ShowWindow(handle, SW_SHOW);
	
}

void MainWindow::show_message_box(const std::string& message) {
	CString msg(message.c_str());
	AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
}


void MainWindow::open_image() {
	TCHAR image_type[] = L"图片(*.png;*.jpg)|*.png;*.jpg|所有文件(*.*)|*.*||";
	CFileDialog file_dialog(TRUE, NULL, NULL, 0, image_type, this);

	if (IDOK != file_dialog.DoModal())
		return;

	image_operator_.open(file_dialog.GetPathName().GetBuffer());

	CRect rect;
	GetDlgItem(IDC_MAIN_PIC)->GetWindowRect(rect);

	image_operator_.resize(rect.Width(), rect.Height());
	image_operator_.rotation(45);

	image_operator_.show_image();

}


void MainWindow::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {



	static int min_width = 580, min_height = 410;

	if (lpMMI->ptMinTrackSize.x < min_width)
		lpMMI->ptMinTrackSize.x = min_width;

	if (lpMMI->ptMinTrackSize.y < min_height)
		lpMMI->ptMinTrackSize.y = min_height;


	CDialogEx::OnGetMinMaxInfo(lpMMI);

}



void MainWindow::OnBnClickedZoomUp() {
	// zoom_ += 0.1;
	image_operator_.zoom(1.1);
	image_operator_.show_image();
}


void MainWindow::OnBnClickedZoomIn() {
	// zoom_ -= 0.1;
	image_operator_.zoom(0.9);
	image_operator_.show_image();
}


void MainWindow::OnBnClickedToGrayscale() {
	image_operator_.to_grayscale();
	image_operator_.show_image();
}


void MainWindow::OnBnClickedToBinary() {
	image_operator_.to_binary();
	image_operator_.show_image();
}

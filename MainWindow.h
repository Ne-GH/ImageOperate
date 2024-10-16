﻿
// ImageOperateDlg.h: 头文件
//

#pragma once
#include "ImageOperator.h"

// MainWindow 对话框
class MainWindow : public CDialogEx
{
    CMenu menu_;
    nl::ImageOperator image_operator_;
    CSliderCtrl zoom_slider_;
public:
    MainWindow(CWnd* pParent = nullptr);	// 标准构造函数
    void bind(HWND);
    void show_message_box(const std::string& message);

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_IMAGEOPERATE_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持



// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void open_image();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnBnClickedZoomUp();
    afx_msg void OnBnClickedZoomIn();
    afx_msg void OnBnClickedToGrayscale();
    afx_msg void OnBnClickedToBinary();
    // afx_msg void OnNMCustomdrawZoomSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


// stationDlg.h : 头文件
//

#pragma once
#include "cmscomm.h"


// CstationDlg 对话框
class CstationDlg : public CDialogEx
{
// 构造
public:
	CstationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_STATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
//	CComboBox m_ctlButtonNames;
	afx_msg void OnBnClickedBtnOpen();
	CButton m_btnOpen;
	CButton m_btnClose;
	CButton m_btnSend;
	CComboBox m_ctrlComx;
	CComboBox m_ctrlBaud;
	CMSComm m_ctrlComm;
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnSend();
	CString m_str_rec;
	CString m_str_send;
	afx_msg void OnBnClickedBtnAgreeSend();
	int m_i_send_data1;
	int m_i_send_data2;
	int m_i_send_data3;
	int m_i_send_data4;
	CString m_str_rec_data1;
	CString m_str_rec_data2;
	CString m_str_rec_data3;
	CString m_str_rec_data4;
	CComboBox m_ctlJoystickName;
	void OnCbnSelchangeButtonNames(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_XAxis;
	CString m_YAxis;
	CString m_ZAxis;
	CString m_RZAxis;
	afx_msg void OnBnClickedOpenCamera();
	afx_msg void OnBnClickedCloseCamera();
	void MyCommAgreeSend(void);
	bool CommState;
	CComboBox m_ctlButtonNames;
	CString m_str_yaw;
	unsigned char UART_CommandRoute(unsigned char * com_rx_buffer, long len);
	CString m_str_pitch;
	CString m_str_press;
	CString m_str_roll;
	CString m_str_tempr;
	CString m_str_lon;
	CString m_str_lat;
	CString m_str_altitud;
//	CEdit m_str_svnum;
	CString m_str_svnum;
	CString m_str_speed;
	CStatic m_str_ew;
	CString m_str_ns;
};

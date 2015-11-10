
// stationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "station.h"
#include "stationDlg.h"
#include "afxdialogex.h"
#include "DIJoystick.h"



#include <highgui.h>
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Joystick变量
CDIJoystick myJoystick;
//Joystick变量
CvCapture* capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
CvVideoWriter* writer = 0;

unsigned char UART_CommandRoute(unsigned char *com_rx_buffer,long len);


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CstationDlg 对话框




CstationDlg::CstationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CstationDlg::IDD, pParent)
	, CommState(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_str_rec = _T("");
	m_str_send = _T("");
	m_i_send_data1 = 0;
	m_i_send_data2 = 0;
	m_i_send_data3 = 0;
	m_i_send_data4 = 0;
	m_str_rec_data1 = _T("");
	m_str_rec_data2 = _T("");
	m_str_rec_data3 = _T("");
	m_str_rec_data4 = _T("");
	m_XAxis = _T("");
	m_YAxis = _T("");
	m_ZAxis = _T("");
	m_RZAxis = _T("");
	m_str_yaw = _T("");
	m_str_pitch = _T("");
	m_str_press = _T("");
	m_str_roll = _T("");
	m_str_tempr = _T("");
	m_str_lon = _T("");
	m_str_lat = _T("");
	m_str_altitud = _T("");
	m_str_svnum = _T("");
	m_str_speed = _T("");
	m_str_ns = _T("");
}

void CstationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_BUTTON_NAMES, m_ctlButtonNames);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_COM_COMX, m_ctrlComx);
	DDX_Control(pDX, IDC_COM_BAUD, m_ctrlBaud);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);


	DDX_Text(pDX, IDC_EDIT_REC, m_str_rec);
	DDX_Text(pDX, IDC_EDIT_SEND, m_str_send);
	DDX_Text(pDX, IDC_SEND_DATA1, m_i_send_data1);
	DDX_Text(pDX, IDC_SEND_DATA2, m_i_send_data2);
	DDX_Text(pDX, IDC_SEND_DATA3, m_i_send_data3);
	DDX_Text(pDX, IDC_SEND_DATA4, m_i_send_data4);
	DDX_Text(pDX, IDC_REC_DATA1, m_str_rec_data1);
	DDX_Text(pDX, IDC_REC_DATA2, m_str_rec_data2);
	DDX_Text(pDX, IDC_REC_DATA3, m_str_rec_data3);
	DDX_Text(pDX, IDC_REC_DATA4, m_str_rec_data4);
	DDX_Control(pDX, IDC_JOYSTICK_NAME, m_ctlJoystickName);
	DDX_Text(pDX, IDC_XAXIS, m_XAxis);
	DDX_Text(pDX, IDC_YAXIS, m_YAxis);
	DDX_Text(pDX, IDC_ZAXIS, m_ZAxis);
	DDX_Text(pDX, IDC_RZAXIS, m_RZAxis);
	DDX_Control(pDX, IDC_BUTTON_NAMES, m_ctlButtonNames);
	DDX_Text(pDX, IDC_YAW, m_str_yaw);
	DDX_Text(pDX, IDC_PITCH, m_str_pitch);
	DDX_Text(pDX, IDC_PRESS, m_str_press);
	DDX_Text(pDX, IDC_ROLL, m_str_roll);
	DDX_Text(pDX, IDC_TEMPR, m_str_tempr);
	DDX_Text(pDX, IDC_LON, m_str_lon);
	DDX_Text(pDX, IDC_LAT, m_str_lat);
	DDX_Text(pDX, IDC_ALTITUD, m_str_altitud);
	//  DDX_Control(pDX, IDC_SVNUM, m_str_svnum);
	DDX_Text(pDX, IDC_SVNUM, m_str_svnum);
	DDX_Text(pDX, IDC_SEEED, m_str_speed);
	DDX_Control(pDX, IDC_STATIC_EW, m_str_ew);
	DDX_Text(pDX, IDC_STATIC_NS, m_str_ns);
}

BEGIN_MESSAGE_MAP(CstationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CstationDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CstationDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_SEND, &CstationDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_AGREE_SEND, &CstationDlg::OnBnClickedBtnAgreeSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_OPEN_CAMERA, &CstationDlg::OnBnClickedOpenCamera)
	ON_BN_CLICKED(ID_CLOSE_CAMERA, &CstationDlg::OnBnClickedCloseCamera)
END_MESSAGE_MAP()

//枚举串口
void EnumerateSerialPorts(CUIntArray& ports)
{
	//Make sure we clear out any elements which may already be in the array
	ports.RemoveAll();

		//Use QueryDosDevice to look for all devices of the form COMx. This is a better
		//solution as it means that no ports have to be opened at all.
		TCHAR szDevices[65535];
		DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
		if (dwChars)
		{
			int i = 0;
			for (;;)
			{
				//Get the current device name
				TCHAR* pszCurrentDevice = &szDevices[i];
				//If it looks like "COMX" then
				//add it to the array which will be returned
				int nLen = _tcslen(pszCurrentDevice);
				if (nLen > 3 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0)
				{
					//Work out the port number
					int nPort = _ttoi(&pszCurrentDevice[3]);
					ports.Add(nPort);
				}
				// Go to next NULL character
				while (szDevices[i] != _T('\0'))
					i++;
				// Bump pointer to the next string
				i++;
				// The list is double-NULL terminated, so if the character is
				// now NULL, we're at the end
				if (szDevices[i] == _T('\0'))
					break;
			}
		}
		else
			TRACE(_T("Failed in call to QueryDosDevice, GetLastError:%d\n"), GetLastError());
}
// CstationDlg 消息处理程序

BOOL CstationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化按键
	m_btnOpen.EnableWindow(TRUE);
	m_btnClose.EnableWindow(FALSE);
	m_btnSend.EnableWindow(FALSE);
	m_ctrlComx.EnableWindow(TRUE);

	
	//初始化combox
	CommState=false;//默认串口关闭
	CUIntArray ports;
	EnumerateSerialPorts(ports);
	for (int i = 0; i<ports.GetSize(); i++)
	{
		CString str_com;
//		CString temp;
		CString str;
		str.Format(_T("%d"), ports.ElementAt(i));
//		str.Format("%d",ports.ElementAt(i));
		str_com = (CString)"COM" + str;
//		str_com = temp+str;
		if (str_com != "COM0")
		{
			m_ctrlComx.AddString(str_com);
//			m_JoyComx.AddString(str_com);
		}
	}
	m_ctrlBaud.AddString(_T("9600"));
	m_ctrlBaud.AddString(_T("38400"));
	m_ctrlBaud.AddString(_T("19200"));
	m_ctrlBaud.AddString(_T("57600"));
	m_ctrlBaud.AddString(_T("115200"));


	m_ctrlComx.SetCurSel(0);
//	m_JoyComx.SetCurSel(0);
	m_ctrlBaud.SetCurSel(0);

//////////////////////////////////////////////////////////////////////
//
// 摄像头窗口函数
//
//////////////////////////////////////////////////////////////////////
	// TODO: Add extra initialization here
	pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);
        pDC =pwnd->GetDC();
	//pDC =GetDC();
        hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	// 设置计时器,每xms触发一次事件
	SetTimer(1, 50, NULL);
	SetTimer(2, 80, NULL);
	SetTimer(3, 10, NULL);//摄像头10ms



//////////////////////////////////////////////////////////////////////
//
// Set the HWND for this Dialog Instance To Enable DInput!
//手柄初始化
//
//////////////////////////////////////////////////////////////////////
	CWnd *jb = this;

	myJoystick.SetHWND(jb->m_hWnd);

	// Reset Combo Control
	m_ctlJoystickName.ResetContent();

	LPCDIDEVICEINSTANCE lpddi = NULL;

	// Ensure you have First Joystick ID to start search for additional Devices!
	lpddi = myJoystick.GetFirstJoystickID();

	if (!lpddi)
	{
		// No joysticks have been found!
		//AfxMessageBox("I have not been able to find a joystick on your system.", "No Joystick Detected", MB_ICONHAND | MB_OK);
		AfxMessageBox(_T("I have not been able to find a joystick on your system."));

		//OnCancel();
	}
	else
	{

		while (lpddi)
		{
			int x = m_ctlJoystickName.AddString(lpddi->tszInstanceName);
			m_ctlJoystickName.SetItemDataPtr(x, (void*)lpddi);
			lpddi = myJoystick.GetNextJoystickID();
		}

		m_ctlJoystickName.SetCurSel(0);
		OnCbnSelchangeButtonNames();

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CstationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CstationDlg::OnPaint()
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
HCURSOR CstationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CstationDlg, CDialogEx)
	ON_EVENT(CstationDlg, IDC_MSCOMM1, 1, CstationDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


////////////////////////////以下串口协议//////////////////////////////
//uart reicer flag
#define b_uart_head  0x80  //收到A5 头 标志位
#define b_rx_over    0x40  //收到完整的帧标志
// USART Receiver buffer

//volatile unsigned char rx_wr_index; //缓冲写指针
volatile unsigned char RC_Flag;  //接收状态标志字节
#define RX_BUFFER_SIZE 50 //接收缓冲区字节数
unsigned char data_buffer[RX_BUFFER_SIZE];//有效数据缓冲区
CString str_rec_temp1;
CString str_rec_temp2;
CString str_rec_temp3;
CString str_rec_temp4;

 //--校验当前接收到的一帧数据是否 与帧校验字节一致
//unsigned char Sum_check(unsigned char *check_buffer)
unsigned char Sum_check(void)
{ 
	unsigned char i;
	unsigned int checksum=0; 
  for(i=0;i<data_buffer[0]-2;i++)
	  checksum+=data_buffer[i];
	if((checksum%256)==data_buffer[data_buffer[0]-2])
		return(0x01); //Checksum successful
	else
		return(0x00); //Checksum error
}


void CstationDlg::OnCommMscomm1()
{
	// TODO: 在此处添加消息处理程序代码
/////////////////////////////////////////////////////////////
///////////////////接收姿态和GPS数据////////////////////////////////////	  	
	//从串口接收数,协议接收
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //设置BYTE数组
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //值为2表示接收缓冲区内有字符
	{
		variant_inp=m_ctrlComm.get_Input(); //读缓冲区消息
		safearray_inp=variant_inp; ///变量转换
		len=safearray_inp.GetOneDimSize(); //得到有效的数据长度
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);

		if(UART_CommandRoute(rxdata,len))
		{
			UpdateData(FALSE); //更新编辑框内容	
		}
	}

	
	/*/////////////////////////////////////////////////////////////
///////////////////定时器发送int*4数据,接收到下位机+1后的数据处理////////////////////////////////////	  	
	//从串口接收数,协议接收
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //设置BYTE数组
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //值为2表示接收缓冲区内有字符
	{
		variant_inp=m_ctrlComm.get_Input(); //读缓冲区消息
		safearray_inp=variant_inp; ///变量转换
		len=safearray_inp.GetOneDimSize(); //得到有效的数据长度
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);

		if(UART_CommandRoute(rxdata,len))
		{
			m_str_rec_data1=str_rec_temp1; //加入接收编辑框对应字符串
			m_str_rec_data2=str_rec_temp2; //加入接收编辑框对应字符串
			m_str_rec_data3=str_rec_temp3; //加入接收编辑框对应字符串
			m_str_rec_data4=str_rec_temp4; //加入接收编辑框对应字符串
			UpdateData(FALSE); //更新编辑框内容	
		}
	}
*/
/*	
/////////////////////////////////////////////////////////////
///////////////////"协议发送"////////////////////////////////////	  	
	//从串口接收数,协议接收
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //设置BYTE数组
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //值为2表示接收缓冲区内有字符
	{
		variant_inp=m_ctrlComm.get_Input(); //读缓冲区消息
		safearray_inp=variant_inp; ///变量转换
		len=safearray_inp.GetOneDimSize(); //得到有效的数据长度
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);

		if(UART_CommandRoute(rxdata,len))
		{
			m_str_rec_data1=str_rec_temp1; //加入接收编辑框对应字符串
			m_str_rec_data2=str_rec_temp2; //加入接收编辑框对应字符串
			m_str_rec_data3=str_rec_temp3; //加入接收编辑框对应字符串
			m_str_rec_data4=str_rec_temp4; //加入接收编辑框对应字符串
			UpdateData(FALSE); //更新编辑框内容	
		}
	}
*/	
/*		
/////////////////////////////////////////////////////////////
///////////////////将接收的数据实时显示////////////////////////////////////
	//从串口接收数据并显示在编辑框中
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //设置BYTE数组
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //值为2表示接收缓冲区内有字符
	{
		variant_inp=m_ctrlComm.get_Input(); //读缓冲区消息
		safearray_inp=variant_inp; ///变量转换
		len=safearray_inp.GetOneDimSize(); //得到有效的数据长度
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);
		for(k=0;k<len;k++) //将数组转换为CString型变量
		{
			char bt=*(char*)(rxdata+k); //字符型
			strtemp.Format(_T("%c"),bt); //将字符送入临时变量strtemp存放
			m_str_rec+=strtemp; //加入接收编辑框对应字符串
		}
	}
	UpdateData(FALSE); //更新编辑框内容	
////////////////////////////////////////////////////////////
*/


}


void CstationDlg::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str_com ;
	CString Baud_rate;
	int opencom = 0;
	m_ctrlComx.GetWindowText(str_com);
	m_ctrlBaud.GetWindowText(Baud_rate);

	m_btnOpen.EnableWindow(FALSE);
	m_btnClose.EnableWindow(TRUE);
	m_btnSend.EnableWindow(TRUE);
	m_ctrlComx.EnableWindow(TRUE);

	if (str_com == "COM1")
		   opencom = 1;
	else if(str_com == "COM2")
		   opencom = 2;
	else if (str_com == "COM3")
		opencom = 3;
	else if (str_com == "COM4")
		opencom = 4;
	else if (str_com == "COM5")
		opencom = 5;
	else if (str_com == "COM6")
		opencom = 6;
	else if (str_com == "COM7")
		opencom = 7;
	else if (str_com == "COM8")
		opencom = 8;
	else if (str_com == "COM9")
		opencom = 9;
	else if (str_com == "COM10")
		opencom = 10;
	else if (str_com == "COM11")
	opencom = 11;
	else if (str_com == "COM12")
		opencom = 12;
	else if (str_com == "COM13")
		opencom = 13;
	else if (str_com == "COM14")
		opencom = 14;
	else if (str_com == "COM15")
		opencom = 15;
	else
		opencom = 0;

	
	m_ctrlComm.put_CommPort(opencom);//选择com口
	m_ctrlComm.put_InputMode(1);//输入方式为二进制方式
	m_ctrlComm.put_InBufferSize(1024);//输入缓冲区大小为1024byte
	m_ctrlComm.put_OutBufferSize(512);//输出缓冲区大小为512byte

	if (Baud_rate == "9600")
		m_ctrlComm.put_Settings(_T("9600,n,8,1"));//设置串口参数：9600波特率，无奇偶校验，8个数据位，1个停止位
	else if (Baud_rate == "19200")
		m_ctrlComm.put_Settings(_T("19200,n,8,1"));//设置串口参数：9600波特率，无奇偶校验，8个数据位，1个停止位
	else if (Baud_rate == "57600")
		m_ctrlComm.put_Settings(_T("57600,n,8,1"));//设置串口参数：9600波特率，无奇偶校验，8个数据位，1个停止位
	else if (Baud_rate == "38400")
		m_ctrlComm.put_Settings(_T("38400,n,8,1"));//设置串口参数：9600波特率，无奇偶校验，8个数据位，1个停止位
	else if (Baud_rate == "115200")
		m_ctrlComm.put_Settings(_T("115200,n,8,1"));//设置串口参数：9600波特率，无奇偶校验，8个数据位，1个停止位
	else
		m_ctrlComm.put_Settings(_T("9600,n,8,1"));//设置串口参数：9600波特率，无奇偶校验，8个数据位，1个停止位
	
	if (!m_ctrlComm.get_PortOpen())
		m_ctrlComm.put_PortOpen(opencom);//打开串口
	else
		////MessageBox("Open COM fail!");
		AfxMessageBox(_T("Open COM fail!"));
	m_ctrlComm.put_RThreshold(1);//每当串口接收缓冲区有多余或等于1个字符时将引发一个接收数据的oncomm事件
	m_ctrlComm.put_InputLen(0);//设置当前接收区数据长度为0
	m_ctrlComm.get_Input();//预读缓冲区以清空残留数据
//	Receive_Flag = 0;
	CommState=true;//串口状态为打开

}


void CstationDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnOpen.EnableWindow(TRUE);
	m_btnClose.EnableWindow(FALSE);
	m_btnSend.EnableWindow(FALSE);
	m_ctrlComx.EnableWindow(TRUE);
	m_ctrlComm.put_PortOpen(0);//关闭串口
	CommState=false;//串口状态为关闭
}


void CstationDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
//“发送”测试
	UpdateData(true); //读取编辑框内容
	m_ctrlComm.put_Output(COleVariant(m_str_send));//发送数据
//	m_strSendData.Empty(); //发送后清空输入框
	UpdateData(false); //更新编辑框内容
	//对发送的数据进行强制类型转换，由 CString 字符串数据转换为 VARIANT 类型。


}


void CstationDlg::OnBnClickedBtnAgreeSend()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: Add your control notification handler code here
	
	unsigned char ch_send;
	unsigned int i_and=0;//
	UpdateData(true); //读取编辑框内容
	CByteArray tempArray;
	tempArray.RemoveAll();
	tempArray.SetSize(10);//

	
	tempArray.SetAt(0,0xA5);//A5和5A是数据包包头
	tempArray.SetAt(1,0x5A);//A5和5A是数据包包头

	tempArray.SetAt(2,4+2+2);//长度：4个字节的数据+2个字节的开始（长度和数据包ID）+2个字节的结束（校验和结束标志aa）
	i_and+=8;//校验和累加
	
	tempArray.SetAt(3,0xA3);//A3，数据包ID
	i_and+=0xA3;//校验和累加

	ch_send=m_i_send_data1;//获取第一个测试数据
	tempArray.SetAt(4,ch_send);
	i_and+=ch_send;//校验和累加

	ch_send=m_i_send_data2;//获取第一个测试数据
	tempArray.SetAt(5,ch_send);
	i_and+=ch_send;//校验和累加

	ch_send=m_i_send_data3;//获取第一个测试数据
	tempArray.SetAt(6,ch_send);
	i_and+=ch_send;//校验和累加

	ch_send=m_i_send_data4;//获取第一个测试数据
	tempArray.SetAt(7,ch_send);
	i_and+=ch_send;//校验和累加
	
	tempArray.SetAt(8,i_and%256);//校验和
	tempArray.SetAt(9,0xAA);//结束标志aa
	m_ctrlComm.put_Output(COleVariant(tempArray));//发送数据
	m_ctrlComm.put_Output(COleVariant(tempArray));//发送数据
}

////////////////////////////////////////////////////////////////////
//手柄控制程序段                                                     
////////////////////////////////////////////////////////////////////
void CstationDlg::OnCbnSelchangeButtonNames(void)
{
	// TODO:  在此添加控件通知处理程序代码
	CWnd*jb = this;
	myJoystick.SetHWND(jb->m_hWnd);

	LPCDIDEVICEINSTANCE lpddi = NULL;

//	m_ctlButtonNames.ResetContent();

	int x = m_ctlJoystickName.GetCurSel();

	lpddi = (LPCDIDEVICEINSTANCE)m_ctlJoystickName.GetItemDataPtr(x);

	GUID myguid;

	if (lpddi && ((int)lpddi != -1))
	{

		memcpy(&myguid, &(lpddi->guidInstance), sizeof(GUID));

		//bool flag=myJoystick.CreateDevice(&myguid);
		myJoystick.SetPreferredDevice(&myguid);

//		m_Buttons = myJoystick.HowManyButtons();

		TCHAR* name = NULL;
		name = myJoystick.GetFirstButtonName();


		while (name)
		{
			m_ctlButtonNames.AddString(name);
			name = myJoystick.GetNextButtonName();

		}
	}

	m_ctlButtonNames.SetCurSel(0);
}


void CstationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: Add your message handler code here and/or call default
	/************************************************************************/
	/* 显示摄像头                                                           */
	/************************************************************************/
	IplImage* m_Frame;
	m_Frame=cvQueryFrame(capture);
	CvvImage m_CvvImage;
	myJoystick.PollDevice();//遥控器设备

	LPDIJOYSTATE2 joy = myJoystick.GetJoystickStateInfo();
	
//	m_XAxis.Format("%d", joy->lX);
	m_XAxis.Format(_T("%d"), joy->lX);
	m_YAxis.Format(_T("%d"), joy->lY);
	m_ZAxis.Format(_T("%d"), joy->lRz);

//	m_RXAxis.Format(_T("%d"), joy->lRx);
//	m_RYAxis.Format(_T("%d"), joy->lRy);
	m_RZAxis.Format(_T("%d"), joy->lZ);


	switch (nIDEvent)
	{
	case 1:
		UpdateData(FALSE);//控件更新显示
		break;
	case 2:
		if(CommState==true)
			MyCommAgreeSend();
		UpdateData(FALSE);//控件更新显示
/*		m_ctrlComm.put_OutBufferCount(0);
		if (JoyFlag == 1)
			m_JoyComm.put_Output(COleVariant(hexdata));//发送数据
*/		break;
	case 3://摄像头刷新显示
		m_CvvImage.CopyOf(m_Frame,1);	
		if (true)
		{
			m_CvvImage.DrawToHDC(hDC, &rect);
			//cvWaitKey(10);
		}
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CstationDlg::OnBnClickedOpenCamera()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox("OK");
	if(!capture)
	{
		capture = cvCaptureFromCAM(0);
		//AfxMessageBox("OK");
	}
	if (!capture)
	{
		AfxMessageBox(_T("无法打开摄像头"));
		return;
	}

	// 测试
	IplImage* m_Frame;
	m_Frame=cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame,1);	
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);
	}

	// 设置计时器,每10ms触发一次事件
//	SetTimer(1,10,NULL);
}


void CstationDlg::OnBnClickedCloseCamera()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: Add your control notification handler code here
	cvReleaseCapture(&capture);
	CDC MemDC; 
	CBitmap m_Bitmap1;
//	m_Bitmap1.LoadBitmap(IDB_BITMAP1); 
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
}

//串口协议发送数据
void CstationDlg::MyCommAgreeSend(void)
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: Add your control notification handler code here

	LPDIJOYSTATE2 joy = myJoystick.GetJoystickStateInfo();
	
	unsigned char ch_send;
	unsigned int i_and=0;//
	UpdateData(true); //读取编辑框内容
	CByteArray tempArray;
	tempArray.RemoveAll();
	tempArray.SetSize(15);//

	
	tempArray.SetAt(0,0xA5);//A5和5A是数据包包头
	tempArray.SetAt(1,0x5A);//A5和5A是数据包包头

	ch_send=8+2+2;
	tempArray.SetAt(2,ch_send);//长度：4个字节的数据+2个字节的开始（长度和数据包ID）+2个字节的结束（校验和结束标志aa）
	i_and+=ch_send;//校验和累加
	
	ch_send=0xA3;
	tempArray.SetAt(3,ch_send);//A3，数据包ID
	i_and+=ch_send;//校验和累加

joy->lX=123;//测试串口通信
joy->lY=256;
joy->lRz=1000;
joy->lZ=2048;
	ch_send=joy->lX/256;//获取第1个测试数据
	tempArray.SetAt(4,ch_send);
	i_and+=ch_send;//校验和累加	
	ch_send=joy->lX%256;//获取第1个测试数据
	tempArray.SetAt(5,ch_send);
	i_and+=ch_send;//校验和累加


	ch_send=joy->lY/256;//获取第2个测试数据
	tempArray.SetAt(6,ch_send);
	i_and+=ch_send;//校验和累加	
	ch_send=joy->lY%256;//获取第2个测试数据
	tempArray.SetAt(7,ch_send);
	i_and+=ch_send;//校验和累加

	ch_send=joy->lRz/256;//获取第3个测试数据
	tempArray.SetAt(8,ch_send);
	i_and+=ch_send;//校验和累加
	ch_send=joy->lRz%256;//获取第3个测试数据
	tempArray.SetAt(9,ch_send);
	i_and+=ch_send;//校验和累加

	ch_send=joy->lZ/256;//获取第4个测试数据
	tempArray.SetAt(10,ch_send);
	i_and+=ch_send;//校验和累加
	ch_send=joy->lZ%256;//获取第4个测试数据
	tempArray.SetAt(11,ch_send);
	i_and+=ch_send;//校验和累加
	
	tempArray.SetAt(12,i_and%256);//校验和
	tempArray.SetAt(13,0xAA);//结束标志aa
	m_ctrlComm.put_Output(COleVariant(tempArray));//发送数据	
	m_ctrlComm.put_Output(COleVariant(tempArray));//发送数据

}
//--这个子程序需要在主程序中 定时调用,以检查 串口是否接收完一帧数据
/*
unsigned char UART_CommandRoute(unsigned char *com_rx_buffer,long len)
//unsigned char UART_CommandRoute(long len)
{
	unsigned int rx_wr_index = 0; //处理后的缓冲写指针	
	unsigned int loop_index; //循环指针	
	unsigned char res;
	float f_temp;
	signed short int temp;
//	AfxMessageBox("UART_CommandRoute");	
	for(loop_index=0;loop_index<len;loop_index++)
	{
		res = com_rx_buffer[loop_index];
		if(res==0xa5) 
		{
			RC_Flag|=b_uart_head; //如果接收到A5 置位帧头标专位
			data_buffer[rx_wr_index++]=res; //保存这个字节.
		}
		else if(res==0x5a)
		{ 
			if(RC_Flag&b_uart_head) //如果上一个字节是A5 那么认定 这个是帧起始字节
			{
				rx_wr_index=0;  //重置 缓冲区指针
				RC_Flag&=~b_rx_over; //这个帧才刚刚开始收
			}
			else //上一个字节不是A5
				data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head; //清帧头标志
		}
		else
		{ 
			data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head;
			if(rx_wr_index==data_buffer[0]) //收够了字节数.
			{  
				rx_wr_index = 0;
				RC_Flag|=b_rx_over; //置位 接收完整的一帧数据
			}
		}

		if(rx_wr_index==RX_BUFFER_SIZE) //防止缓冲区溢出
		rx_wr_index--;
	}


	if(RC_Flag&b_rx_over)
	{  //已经接收完一帧?

		RC_Flag&=~b_rx_over; //清标志先
//		if(Sum_check(data_buffer))
		if(Sum_check())
		{ 
			
			if(data_buffer[1]==0xA3) //REALL  解析ALL的数据
			{
//			AfxMessageBox(_T("Sum_check success!"));	
				
				temp = 0;
				temp = data_buffer[2];
				temp <<= 8;
				temp |= data_buffer[3];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //偏航角
				
			} 
		}//校验是否通过?
		else
		{		
//			AfxMessageBox(_T("Sum_check Fail!"));	

		}
		return 1;
	}
	else 
		return 0;
}

*/
unsigned char CstationDlg::UART_CommandRoute(unsigned char * com_rx_buffer, long len)
{
	unsigned int rx_wr_index = 0; //处理后的缓冲写指针	
	unsigned int loop_index; //循环指针	
	unsigned char res;
	float f_temp;
	signed short int temp=0;
	unsigned long int li_temp;
//	AfxMessageBox("UART_CommandRoute");	
	for(loop_index=0;loop_index<len;loop_index++)
	{
		res = com_rx_buffer[loop_index];
		if(res==0xa5) 
		{
			RC_Flag|=b_uart_head; //如果接收到A5 置位帧头标专位
			data_buffer[rx_wr_index++]=res; //保存这个字节.
		}
		else if(res==0x5a)
		{ 
			if(RC_Flag&b_uart_head) //如果上一个字节是A5 那么认定 这个是帧起始字节
			{
				rx_wr_index=0;  //重置 缓冲区指针
				RC_Flag&=~b_rx_over; //这个帧才刚刚开始收
			}
			else //上一个字节不是A5
				data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head; //清帧头标志
		}
		else
		{ 
			data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head;
			if(rx_wr_index==data_buffer[0]) //收够了字节数.
			{  
				rx_wr_index = 0;
				RC_Flag|=b_rx_over; //置位 接收完整的一帧数据
			}
		}

		if(rx_wr_index==RX_BUFFER_SIZE) //防止缓冲区溢出
		rx_wr_index--;
	}


	if(RC_Flag&b_rx_over)
	{  //已经接收完一帧?

		RC_Flag&=~b_rx_over; //清标志先
		if(Sum_check())
		{ 
			
			if(data_buffer[1]==0xA3) //REALL  解析ALL的数据
			{
//			AfxMessageBox(_T("Sum_check success!"));	
				
				temp=0;
				temp = data_buffer[2];//yaw
				temp <<= 8;
				temp |= data_buffer[3];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //偏航角
				m_str_yaw.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[4];//pitch
				temp <<= 8;
				temp |= data_buffer[5];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //俯仰角
				m_str_pitch.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[6];//roll
				temp <<= 8;
				temp |= data_buffer[7];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //滚转角
				m_str_roll.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[8];//tempr
				temp <<= 8;
				temp |= data_buffer[9];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //温度
				m_str_tempr.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[10];//press
				temp <<= 8;
				temp |= data_buffer[11];
				f_temp=(float)temp * 10.0f; //气压
				m_str_press.Format(_T("%.0f"),f_temp);

				li_temp=0;
				li_temp = data_buffer[12];//longitude	
				li_temp <<= 8;
				li_temp |= data_buffer[13];
				li_temp <<= 8;
				li_temp |= data_buffer[14];
				li_temp <<= 8;
				li_temp |= data_buffer[15];
				f_temp=(float)li_temp / 100000.0f; //经度
				m_str_lon.Format(_T("%.5f %1c   "),f_temp,data_buffer[16]);
				
				li_temp=0;
				li_temp = data_buffer[17];//latitude	
				li_temp <<= 8;
				li_temp |= data_buffer[18];
				li_temp <<= 8;
				li_temp |= data_buffer[19];
				li_temp <<= 8;
				li_temp |= data_buffer[20];
				f_temp=(float)li_temp / 100000.0f; //纬度
				m_str_lat.Format(_T("%.5f %1c   "),f_temp,data_buffer[21]);
				
				m_str_svnum.Format(_T("%d"),data_buffer[22]);//svnum		可见卫星数

				temp=0;
				temp = data_buffer[23];//speed		
				temp <<= 8;
				temp |= data_buffer[24];
				f_temp=(float)temp / 1000.0f; //速度
				m_str_speed.Format(_T("%.3fkm/h"),f_temp);

				temp=0;
				temp = data_buffer[25];//altitude		
				temp <<= 8;
				temp |= data_buffer[26];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //海拔
				m_str_altitud.Format(_T("%.1fm"),f_temp);

/*				str_rec_temp1.Format(_T("%d"),data_buffer[2]);
				str_rec_temp2.Format(_T("%d"),data_buffer[3]);
				str_rec_temp3.Format(_T("%d"),data_buffer[4]);
				str_rec_temp4.Format(_T("%d"),data_buffer[5]);
*/
/*				str_rec_temp1.Format(_T("%d"),data_buffer[2]*256+data_buffer[3]);//显示四个int变量
				str_rec_temp2.Format(_T("%d"),data_buffer[4]*256+data_buffer[5]);
				str_rec_temp3.Format(_T("%d"),data_buffer[6]*256+data_buffer[7]);
				str_rec_temp4.Format(_T("%d"),data_buffer[8]*256+data_buffer[9]);

*/	
				
			} 
		}//校验是否通过?
		else
		{		
//			AfxMessageBox(_T("Sum_check Fail!"));	

		}
		return 1;
	}
	else 
		return 0;
}


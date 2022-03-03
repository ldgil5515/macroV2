#pragma once

#define		NUL		0x00
#define		SOH		0x01
#define		STX		0x02
#define		ETX		0x03
#define		EOT		0x04
#define		ENQ		0x05
#define		ACK		0x06
#define		NAK		0x15
#define		XON		0x11
#define		XOFF	0x13
#define		ESC		0x1b
#define		CR		0x0d
#define		LF		0x0a

const	int	DEF_MAX_SERIAL_PORT						= 6;			///< Serial Port ¼ö·®
const	int ERR_SERIAL_PORT_SUCCESS					= 0;		// Success
const	int ERR_PORT_OPEN_FAIL						= 1;		
const	int ERR_TIME_OUT							= 2;		


class CRsPort  
{
public:
	BOOL           m_Connect;
    HANDLE         m_idComDev;
	CString		   m_ComPort;

public:
	BOOL initComport(CString m_portName);
	int ReadCommPort( unsigned char *message, DWORD length);
	int WriteCommPort( unsigned char* message, DWORD dwLength);
	bool IsCommPortOpen();
	static void loadListCommPort(CComboBox &listCommPort);
	CRsPort();
    CRsPort(DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits );
	virtual ~CRsPort();

protected:
	void CloseCommPort(void);
	BOOL SetupConnection(void);
    OVERLAPPED     osWrite;
    OVERLAPPED     osRead;
	DCB            dcb_setup;
};

// RsPort.cpp: implementation of the CRsPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RsPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRsPort::CRsPort()
{
   dcb_setup.BaudRate = CBR_19200;
   dcb_setup.ByteSize = 8;
   dcb_setup.Parity   = NOPARITY;
   dcb_setup.StopBits = ONESTOPBIT;
}


CRsPort::CRsPort(DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits )
{
   dcb_setup.BaudRate = BaudRate;
   dcb_setup.ByteSize = ByteSize;
   dcb_setup.Parity   = Parity;
   dcb_setup.StopBits = StopBits;
}


CRsPort::~CRsPort()
{
   CloseCommPort();
}

BOOL CRsPort::initComport(CString m_portName)
{
   COMMTIMEOUTS  commTimeOuts;

   m_idComDev = CreateFile( (LPCTSTR)m_portName, GENERIC_READ | GENERIC_WRITE,
	   0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );


   if( m_idComDev == (HANDLE) -1) 
   {
		CloseHandle( m_idComDev );
		m_Connect = FALSE;
   } 
   else 
   {
		SetCommMask( m_idComDev, EV_RXCHAR );
		SetupComm( m_idComDev, 4096,4096);
		PurgeComm( m_idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
		commTimeOuts.ReadIntervalTimeout = -1;
		commTimeOuts.ReadTotalTimeoutMultiplier = 0;
		commTimeOuts.ReadTotalTimeoutConstant = 1000;
		commTimeOuts.WriteTotalTimeoutMultiplier = 0;
		commTimeOuts.WriteTotalTimeoutConstant = 1000;
		SetCommTimeouts( m_idComDev, &commTimeOuts );
		m_Connect = SetupConnection();
		osWrite.Offset = 0;
		osWrite.OffsetHigh = 0;
		osRead.Offset = 0;
		osRead.OffsetHigh = 0;
		osRead.hEvent  = CreateEvent(NULL, TRUE, FALSE, NULL);
		osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   }

   return m_Connect;
}

BOOL CRsPort::SetupConnection()
{
   BOOL fRetVal;
   DCB  dcb;
   
   dcb.DCBlength = sizeof( DCB );
   GetCommState( m_idComDev, &dcb );
   dcb.BaudRate = dcb_setup.BaudRate;
   dcb.ByteSize = dcb_setup.ByteSize;
   dcb.Parity   = dcb_setup.Parity;
   dcb.StopBits = dcb_setup.StopBits;
   dcb.fOutxDsrFlow = 0;
   dcb.fDtrControl = DTR_CONTROL_ENABLE;
   dcb.fOutxCtsFlow = 0;
   dcb.fRtsControl = RTS_CONTROL_ENABLE;
   dcb.fInX = dcb.fOutX = 0; // XON/XOFF
   dcb.XonChar  = 0x11; // ASCII_XON;
   dcb.XoffChar = 0x13; // ASCII_XOFF;
   dcb.XonLim   = 100;
   dcb.XoffLim  = 100;
   dcb.fBinary  = TRUE;
   dcb.fParity  = TRUE;
   fRetVal = SetCommState( m_idComDev, &dcb );
   return fRetVal;
}

void CRsPort::CloseCommPort()
{
  if( m_Connect == FALSE ) return;
  CloseHandle( m_idComDev );
  CloseHandle( osRead.hEvent );
  CloseHandle( osWrite.hEvent );
}

int CRsPort::WriteCommPort(unsigned char *message, DWORD dwLength)
{
   int ret_code;
   ret_code = WriteFile( m_idComDev, message, dwLength, &dwLength, &osWrite);
   return ret_code;
}

int CRsPort::ReadCommPort(unsigned char *message, DWORD length)
{
   COMSTAT  ComStat;
   DWORD    dwErrorFlags;
   DWORD    dwLength;
   DWORD	dwReadLength = 0;

   CStringA strTemp;
   strTemp.Format("%s",message);

   if( m_Connect == FALSE )  return 0;
   else 
   {
	   ClearCommError( m_idComDev, &dwErrorFlags, &ComStat );
	   dwLength = min((DWORD) length, ComStat.cbInQue);
	   ReadFile( m_idComDev, message, dwLength, &dwReadLength, &osRead );
   }

   if(dwReadLength == 0)
   {
	   CStringA str;
	   str.Format("%s", message);

	   if(strTemp != str)
	   {	   
		   return str.GetLength();
	   }	   
   }
  

   return dwReadLength;
}

bool CRsPort::IsCommPortOpen()
{
	if(m_Connect)
		return true;
	return false;
}

void CRsPort::loadListCommPort(CComboBox &listCommPort)
{
	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);
	TCHAR szData[20], szName[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	listCommPort.ResetContent();
	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));

	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		listCommPort.AddString(CString(szData));

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}

	listCommPort.SetCurSel(0);
	RegCloseKey(hKey);
}
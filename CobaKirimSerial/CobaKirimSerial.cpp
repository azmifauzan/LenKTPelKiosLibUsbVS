// CobaKirimSerial.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "SerialPort.h"


int main()
{
	HANDLE hComm;
	boolean Status;
	char comport[100];
	strcpy(comport,"\\\\.\\COM8");
	hComm = CreateFile(comport,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		0,
		0);
	if (hComm == INVALID_HANDLE_VALUE) {
		printf("gagal create file");
	}
	else {
		printf("sukses create file\n");
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);

	if (Status == FALSE) {
		printf("gagal get comm state");
	}
	else {
		printf("sukses get comm state\n");
	}

	dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;      // Setting Parity = None
											//dcbSerialParams.fAbortOnError = TRUE;

	Status = SetCommState(hComm, &dcbSerialParams);

	if (Status == FALSE)
	{
		printf("gagal set comm state");
	}
	else {
		printf("sukses set comm state\n");
	}

	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE) {
		printf("gagal set comm timeouts");
	}
	else {
		printf("sukses set comm timeouts\n");
	}

	DWORD  dNoOfBytesWritten = 0;
	char text_to_send[100];
	strcpy(text_to_send,"ektpopen#;\n");

	if (!WriteFile(hComm, text_to_send, strlen(text_to_send),&dNoOfBytesWritten, NULL))
	{
		printf("Error send text\n");
	}
	else
	{
		printf("sukses send text\n");
	}

	Status = SetCommMask(hComm, EV_RXCHAR);
	if (Status == FALSE) {
		printf("Error set com mask\n");
	}
	else
	{
		printf("sukses set comm mask\n");
	}
	
	DWORD dwEventMask;
	char TempChar;
	DWORD NoBytesRead;
	char SerialBuffer[1024], hasil3[1024], isiBuffer[1024];
	int i=0;
	BOOL simpanData = FALSE;
	char dataSebelum = '\0';

	Status = WaitCommEvent(hComm, &dwEventMask, NULL);
	if (Status == FALSE) {
		printf("Error wait comm event\n");
	}
	else
	{
		printf("sukses wait comm event\n");

		int err;
		do
		{
			ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
			if (TempChar == '$') {
				simpanData = TRUE;
			}

			if (simpanData) {
				isiBuffer[i] = TempChar;
				//isiBuffer[i] = '0';
				i++;
			}

			if (TempChar == ';') {
				if (dataSebelum != ';') {
					simpanData = FALSE;
					break;
				}
				i = 0;
			}

			dataSebelum = TempChar;
		} while (NoBytesRead > 0);

		int j = 0;
		for (j = 0; j < i - 1; j++)
		{
			hasil3[j] = isiBuffer[j];
		}
		hasil3[i-1] = '\0';
	}

	printf("i:%d, hasil: %s",i,hasil3);

	std::cin.get();
    return 0;
}


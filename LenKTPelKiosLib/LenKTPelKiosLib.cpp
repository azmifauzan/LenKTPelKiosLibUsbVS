// LenKTPelKiosLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <direct.h>

using namespace std;

int portnumber = 999;
char isiBuffer[102400];
char isiParsing[102400];
int resput = 0, resinfo = 0, resclose = 0, respoll = 0, resreset = 0, resfinger = 0;
int reslog = 0, resdemog = 0, resswitch = 0, resdisplay = 0, ressetpow = 0, resrespow = 0, resopen = 0;
int resupdateinit = 0, resupdatefile = 0, resupdatefinish = 0;
int openstatus = 0;
HANDLE hComm, hCommGlobal;
int lastReaderStatus = 0, lastPutKtpStatus = 0, lastVerifyFingerStatus = 0, lastDeviceStatus = 0, portInUse = 0;
char commandGlobal[100];
char hasilOpen[1024];
char hasilCommand[102400];
int resultKirim = 0;
DWORD threadOpen;
HANDLE hOpen, hCommand;

void tulisLog(char tulis[102400])
{
	char folde[FILENAME_MAX];
	_getcwd(folde, FILENAME_MAX);
	char pathfile[5000];
	sprintf(pathfile, "%s\\LenKTPelKiosLibUsb.log", folde);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *fp;
	fp = fopen(pathfile, "a+");
	if (fp != NULL) {
		char mylog[10240];
		sprintf(mylog, "%d/%d/%d %d:%d:%d - %s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, tulis);
		fputs(mylog, fp);
	}
	fclose(fp);
}

void parsingIsi()
{
	//printf("\nisi buffer:%s\n",isiBuffer);
	char mylog[102400];
	//sprintf(mylog,"receive buffer:%s",isiBuffer);
	//tulisLog(mylog);
	//PurgeComm(hComm,PURGE_RXCLEAR|PURGE_TXCLEAR);
	//portInUse = 0;

	if (isiBuffer[0] == '$') {
		char *hdr = strtok(isiBuffer, "#");
		char *isi = strtok(NULL, "#");

		if (strcmp(hdr, "$demog") == 0) {
			sprintf(mylog, "receive buffer: data demografi");
		}
		else if (strcmp(hdr, "$machinelg") == 0) {
			sprintf(mylog, "receive buffer: data log");
		}
		else {
			sprintf(mylog, "receive buffer:%s", isi);
		}

		if (strcmp(hdr, "$poll") == 0) {
			strcpy(isiParsing, isi);
			respoll = 1;
		}
		else if (strcmp(hdr, "$info") == 0) {
			strcpy(isiParsing, isi);
			resinfo = 1;
		}
		else if (strcmp(hdr, "$close") == 0) {
			strcpy(isiParsing, isi);
			resclose = 1;
		}
		else if (strcmp(hdr, "$putktp") == 0) {
			strcpy(isiParsing, isi);
			resput = 1;
		}
		else if (strcmp(hdr, "$reset") == 0) {
			strcpy(isiParsing, isi);
			resreset = 1;
		}
		else if (strcmp(hdr, "$finger") == 0) {
			strcpy(isiParsing, isi);
			resfinger = 1;
		}
		else if (strcmp(hdr, "$demog") == 0) {
			strcpy(isiParsing, isi);
			resdemog = 1;
		}
		else if (strcmp(hdr, "$machinelg") == 0) {
			strcpy(isiParsing, isi);
			reslog = 1;
		}
		else if (strcmp(hdr, "$switch") == 0) {
			strcpy(isiParsing, isi);
			resswitch = 1;
		}
		else if (strcmp(hdr, "$display") == 0) {
			strcpy(isiParsing, isi);
			resdisplay = 1;
		}
		else if (strcmp(hdr, "$setpow") == 0) {
			strcpy(isiParsing, isi);
			ressetpow = 1;
		}
		else if (strcmp(hdr, "$respow") == 0) {
			strcpy(isiParsing, isi);
			resrespow = 1;
		}
		else if (strcmp(hdr, "$updateinit") == 0) {
			strcpy(isiParsing, isi);
			resupdateinit = 1;
		}
		else if (strcmp(hdr, "$updatefile") == 0) {
			strcpy(isiParsing, isi);
			resupdatefile = 1;
		}
		else if (strcmp(hdr, "$updatefinish") == 0) {
			strcpy(isiParsing, isi);
			resupdatefinish = 1;
		}

		memset(isiBuffer, 0, sizeof(isiBuffer));

		//tulisLog(mylog);
	}
}

void bacafile()
{
	FILE *fp;
	char buff[255];
	fp = fopen("D:\\test.txt", "r");
	if (fp == NULL) {
		printf("Could not open file");
	}
	else {
		printf("Hasil Read file: ");
		fscanf(fp, "%s", buff);
		printf("%s", buff);
	}
	fclose(fp);
}

int updateFinish()
{
	//br = TRUE;
	char command[100];
	sprintf(command, "ektpupdatefinish#qwerty#;");
	int result = 0;//sendCommandReceive(command);
	int timeout = 2;
	int x = -1;
	if (result == 0) {
		//br = FALSE;
		Sleep(500);
		int cnt = 0;
		int tt = timeout * 10;

		while (resupdatefinish == 0 && cnt < tt) {
			Sleep(100);
			cnt++;
		}

		if (resupdatefinish == 0) {
			x = -1;
		}
		else {

			if (strcmp(isiParsing, "error") == 0) {
				x = -1;
			}
			else if (strcmp(isiParsing, "0") == 0) {
				x = 0;
			}
			else {
				x = atoi(isiParsing);
			}

			resupdatefinish = 0;
		}
	}
	return x;
}

void kirimFile(int fileLen, char* buffer)
{
	int x = -1;
	//br = TRUE;
	//int arr = (fileLen*2)+100;
	char command[20000];
	sprintf(command, "ektpupdatefile#%d#%d#%d#%s#;", fileLen, 0, fileLen, buffer);
	int result = 0;// sendCommandOnly(command);
	free(buffer);
	int timeout = 2;
	if (result == 0) {
		//br = FALSE;
		Sleep(500);
		int cnt = 0;
		int tt = timeout * 10;

		while (resupdatefile == 0 && cnt < tt) {
			Sleep(100);
			cnt++;
		}

		if (resupdatefile == 0) {
			x = -1;
		}
		else {
			if (strcmp(isiParsing, "error") == 0) {
				x = -1;
			}
			else if (strcmp(isiParsing, "0") == 0) {
				x = updateFinish();
				//printf("\nyyy:%d",x);
			}
			else {
				x = -1;
			}

			resupdatefile = 0;
		}
	}
	//nunggurespon = 1;
	//Sleep(1000);
	//printf("sebelum retrurn:%d",x);
	//return x;
}

int kirimFile2(int fileLen, char* buffer)
{
	//int x = -1;
	//br = TRUE;
	char command[4100];
	int jumdata = 2000;
	int jum = jumdata;
	int divlen = (fileLen * 2) / jumdata;
	int modlen = (fileLen * 2) % jumdata;
	char dt1[4100];
	int lenbaru = jum / 2;
	int i, awal = 0, z;
	for (i = 0; i<divlen; i++) {
		awal = i * jumdata;
		//free(dt1);
		for (z = 0; z<jumdata; z++) {
			dt1[z] = buffer[awal + z];
		}
		dt1[jum] = '\0';

		sprintf(command, "ektpupdatefile#%d#%d#%d#%s#;", lenbaru, awal / 2, ((awal / 2) + lenbaru - 1), dt1);

		//sendCommandOnly(command);
		fflush(stdin);
		fflush(stdout);
		Sleep(15);
	}

	if (modlen > 0) {
		char dt2[1024];
		awal = awal + jumdata;
		for (z = 0; z<modlen; z++) {
			dt2[z] = buffer[awal + z];
		}
		dt2[modlen] = '\0';
		sprintf(command, "ektpupdatefile#%d#%d#%d#%s#;", modlen / 2, (awal / 2), fileLen, dt2);
		//sendCommandOnly(command);
	}
	Sleep(100);

	return updateFinish();
}

void sendCommandReceiveOpTr()
{
	HANDLE hComm;
	BOOL Status;
	char comport[100];
	//strcpy(comport, "\\\\.\\COM8");
	sprintf(comport,"\\\\.\\COM%i",portnumber);
	hComm = CreateFile(comport,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		0,
		0);
	if (hComm == INVALID_HANDLE_VALUE) {
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses create file\n");
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);

	if (Status == FALSE) {
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses get comm state\n");
	}

	dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;      // Setting Parity = None
											//dcbSerialParams.fAbortOnError = TRUE;

	Status = SetCommState(hComm, &dcbSerialParams);

	if (Status == FALSE)
	{
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses set comm state\n");
	}

	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE) {
		//printf("gagal set comm timeouts");
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses set comm timeouts\n");
	}

	DWORD  dNoOfBytesWritten = 0;
	char text_to_send[100];
	strcpy(text_to_send, commandGlobal);

	if (!WriteFile(hComm, text_to_send, strlen(text_to_send), &dNoOfBytesWritten, NULL))
	{
		sprintf(hasilOpen, "writefailed#");
		resultKirim = 99;
	}
	else
	{
		resultKirim = 0;
	}

	Status = SetCommMask(hComm, EV_RXCHAR);
	if (Status == FALSE) {
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else
	{
		//printf("sukses set comm mask\n");
	}

	DWORD dwEventMask;
	char TempChar;
	DWORD NoBytesRead;
	char SerialBuffer[1024], hasil3[1024], isiBuffer[1024];
	int i = 0;
	BOOL simpanData = FALSE;
	char dataSebelum = '\0';

	/*Status = WaitCommEvent(hComm, &dwEventMask, NULL);
	if (Status == FALSE) {
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else
	{*/
		//printf("sukses wait comm event\n");

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
					resopen = 1;
					break;
				}
				i = 0;
			}

			dataSebelum = TempChar;
		} while (NoBytesRead > 0);

		int j = 0;
		for (j = 0; j < i - 1; j++)
		{
			hasilOpen[j] = isiBuffer[j];
		}
		hasilOpen[i - 1] = '\0';
	//}
		CloseHandle(hComm);
}

void sendCommandReceiveOp(const char* command)
{
	strcpy(commandGlobal, command);
	strcpy(hasilOpen,"");
	DWORD threadOpen;
	hOpen = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendCommandReceiveOpTr, 0, 0, &threadOpen);
}

void sendCommandReceiveTr()
{
	PurgeComm(hCommGlobal, PURGE_RXCLEAR | PURGE_TXCLEAR);
	DWORD  dNoOfBytesWritten = 0;
	if (!WriteFile(hCommGlobal, commandGlobal, strlen(commandGlobal), &dNoOfBytesWritten, NULL))
	{
		sprintf(hasilCommand, "writefailed#");
		resultKirim = 99;
	}
	else
	{
		resultKirim = 0;
	}

	DWORD dwEventMask;
	char TempChar;
	DWORD NoBytesRead;
	char SerialBuffer[1024], hasil3[1024];
	int i = 0;
	BOOL simpanData = FALSE;
	char dataSebelum = '\0';

	do
	{
		ReadFile(hCommGlobal, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
		//printf("tempchar:%c\n",TempChar);
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
				//rescommand = 1;
				//printf();
				parsingIsi();
				break;
			}
			i = 0;
		}

		dataSebelum = TempChar;
	} while (true);
}

void sendCommandReceive(const char* command)
{
	strcpy(commandGlobal, command);
	strcpy(hasilOpen, "");
	DWORD threadOpen;
	hCommand = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendCommandReceiveTr, 0, 0, &threadOpen);
}

void openDeviceOnly()
{
	//HANDLE hComm;
	BOOL Status;
	char comport[100];
	//strcpy(comport, "\\\\.\\COM8");
	sprintf(comport, "\\\\.\\COM%i", portnumber);
	hCommGlobal = CreateFile(comport,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		0,
		0);
	if (hCommGlobal == INVALID_HANDLE_VALUE) {
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses create file\n");
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hCommGlobal, &dcbSerialParams);

	if (Status == FALSE) {
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses get comm state\n");
	}

	dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;      // Setting Parity = None
											//dcbSerialParams.fAbortOnError = TRUE;

	Status = SetCommState(hCommGlobal, &dcbSerialParams);

	if (Status == FALSE)
	{
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses set comm state\n");
	}

	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hCommGlobal, &timeouts) == FALSE) {
		//printf("gagal set comm timeouts");
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else {
		//printf("sukses set comm timeouts\n");
	}

	Status = SetCommMask(hCommGlobal, EV_RXCHAR);
	if (Status == FALSE) {
		sprintf(hasilOpen, "opendevicefailed#");
	}
	else
	{
		//printf("sukses set comm mask\n");
	}
}

extern "C" {

	__declspec(dllexport) int ektp_getDLL(char error[100], char dllVersion[100])
	{
		strcpy(error, "ERR_OK");
		strcpy(dllVersion, "5.2.190.301");
		return 0;
	}

	//extern "C" __declspec(dllexport) int __cdecl ektp_getAPI(char error[100], char dllVersion[100]);

	__declspec(dllexport) int ektp_getAPI(char error[100], char dllVersion[100])
	{
		strcpy(error, "ERR_OK");
		strcpy(dllVersion, "01.02.00");
		return 0;
	}

	__declspec(dllexport) int ektp_open(char error[100])
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");
		if (openstatus == 0)
		{
			char folde[FILENAME_MAX];
			_getcwd(folde, FILENAME_MAX);
			char pathfile[1024];
			sprintf(pathfile, "%s\\LenKTPelKiosLibUsb.l", folde);
			FILE *fp;
			char buff[255];
			int porttemp = 1;
			fp = fopen(pathfile, "r+");
			if (fp != NULL) {
				fscanf(fp, "%s", buff);
				portnumber = atoi(buff);
				fclose(fp);
			}

			resultKirim = 88;
			sendCommandReceiveOp("ektpopen#;\n");
			/*int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}*/
			Sleep(500);

			int timeout = 2;
			resopen = 0;
			if (resultKirim == 0) {
				int cnt = 0;
				int tt = timeout * 10;

				while (resopen == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				char *split1 = strtok(hasilOpen, "#");

				if (strcmp(split1, "timeoutwrite") == 0)
				{
					hasil = -1005;
					strcpy(error, "Send Data Timeout");
				}
				else if (strcmp(split1, "timeoutread") == 0)
				{
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}
				else if (strcmp(split1, "opendevicefailed") == 0)
				{
					hasil = -1004;
					strcpy(error, "Setup Device Param Failed");
				}
				else if (strcmp(split1, "$open") == 0)
				{
					char *split2 = strtok(NULL, "#");
					if (strcmp(split2, "0") == 0)
					{
						hasil = 0;
						strcpy(error, "ERR_OK");
						openstatus = 1;
						openDeviceOnly();
					}
					else {
						hasil = -1003;
						strcpy(error, "Open Device Failed");
					}
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
			}

			DWORD  dwExitCode = 0;
			TerminateThread(hOpen, dwExitCode);
		}
		else {
			hasil = 0;
			strcpy(error, "ERR_OK");
		}

		return hasil;
	}
			
	__declspec(dllexport) int ektp_close(char error[100])
	{
		int hasil = 99;
		if (openstatus == 1)
		{
			while (portInUse == 1) {
				Sleep(500);
			}

			portInUse = 1;
			resultKirim = 88;
			sendCommandReceive("ektpclose#;\n");

			int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}

			int timeout = 2;
			if (resultKirim == 0) {
				int cnt = 0;
				int tt = timeout * 10;

				while (resclose == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (resclose == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}

				else {
					if (strcmp(isiParsing, "0") == 0) {
						hasil = 0;
						strcpy(error, "ERR_OK");
						CloseHandle(hCommGlobal);
						openstatus = 0;
					}
					else {
						hasil = -1;
						strcpy(error, "ERR_ERROR");
					}
					resclose = 0;
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
				//cAOReaderAgain();
			}
			portInUse = 0;
			DWORD  dwExitCode = 0;
			TerminateThread(hCommand, dwExitCode);
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}
		
		return hasil;
	}

	__declspec(dllexport) int ektp_info(char error[100], char hid[50], char sn[50], char fw[50], char status[30]) 
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");

		if (openstatus == 1)
		{
			while (portInUse == 1) {
				Sleep(500);
			}

			portInUse = 1;
			resultKirim = 88;
			sendCommandReceive("ektpinfo#;\n");

			int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}

			int timeout = 2;
			if (resultKirim == 0) {
				int cnt = 0;
				int tt = timeout * 10;

				while (resinfo == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (resinfo == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
					//cAOReaderAgain();
				}

				else {
					char *split1 = strtok(isiParsing, ",");
					if (strcmp(split1, "error") == 0) {
						hasil = -1006;
						strcpy(error, "Send Data Failed");
					}
					else {
						strcpy(error, "ERR_OK");
						strcpy(hid, split1);
						strcpy(sn, strtok(NULL, ","));
						strcpy(fw, strtok(NULL, ","));
						strcpy(status, "01");
					}
					resinfo = 0;
					memset(isiParsing, 0, sizeof(isiParsing));
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
				//cAOReaderAgain();
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
				//cAOReaderAgain();
			}
			portInUse = 0;
			DWORD  dwExitCode = 0;
			TerminateThread(hCommand, dwExitCode);
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}	

	__declspec(dllexport) int ektp_putKTP(char error[100], char dm[80], int timeout, char ft[6])
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");

		if (openstatus == 1)
		{
			while (portInUse == 1) {
				Sleep(500);
			}

			lastPutKtpStatus = 18;
			portInUse = 1;
			resultKirim = 88;
			char command[100];
			sprintf(command, "ektpput#%d#%s#;\n", timeout, dm);
			sendCommandReceive(command);

			int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}

			if (resultKirim == 0) {
				int cnt = 0;
				int tt = timeout * 10;

				while (resput == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (resput == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}

				else {
					char *split1 = strtok(isiParsing, ",");
					char *split2 = strtok(NULL, ",");
					char *split3 = strtok(NULL, ",");
					if (strcmp(split1, "error") == 0) {
						hasil = -1;
						strcpy(error, split2);
						lastPutKtpStatus = 27;
						lastReaderStatus = 24;
					}
					else {
						strcpy(error, "ERR_OK");
						char jari[10];
						sprintf(jari, "%s,%s", split2, split3);
						strcpy(ft, jari);
						lastPutKtpStatus = 34;
						lastReaderStatus = 0;
					}
					memset(isiParsing, 0, sizeof(isiParsing));
					resput = 0;
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
			}
			portInUse = 0;
			DWORD  dwExitCode = 0;
			TerminateThread(hCommand, dwExitCode);
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}	

	__declspec(dllexport) int ektp_poll(char error[100], unsigned char *readerStatus, unsigned char *putKTPStatus, unsigned char *verifyFingerStatus, unsigned char *deviceStatus)
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");

		if (openstatus == 1)
		{
			if (portInUse == 1)
			{
				*readerStatus = lastReaderStatus;
				*putKTPStatus = lastPutKtpStatus;
				*verifyFingerStatus = lastVerifyFingerStatus;
				*deviceStatus = lastDeviceStatus;
				Sleep(1000);
			}
			else
			{
				portInUse = 1;
				resultKirim = 88;
				sendCommandReceive("ektppoll#;\n");
				*readerStatus = '\0';
				*putKTPStatus = '\0';
				*verifyFingerStatus = '\0';
				*deviceStatus = '\0';

				int cnt2 = 0;
				int tt2 = 5;
				while (resultKirim == 88 && cnt2 < tt2) {
					Sleep(50);
					cnt2++;
				}

				if (resultKirim == 0) {
					int cnt = 0;
					int tt = 2 * 10;

					while (respoll == 0 && cnt < tt) {
						Sleep(100);
						cnt++;
					}

					if (respoll == 0) {
						hasil = -1007;
						strcpy(error, "Receive Data Timeout");
						//cAOReaderAgain();
					}
					else {
						strcpy(error, "ERR_OK");
						char *split1 = strtok(isiParsing, ",");
						if (strcmp(split1, "error") == 0) {
							hasil = -1012;
							strcpy(error, "Device Not Open");
						}
						else {
							int rdst = atoi(split1);
							int ptst = atoi(strtok(NULL, ","));
							int vrst = atoi(strtok(NULL, ","));
							int dvst = atoi(strtok(NULL, ","));;

							*readerStatus = rdst;
							*putKTPStatus = ptst;
							*verifyFingerStatus = vrst;
							*deviceStatus = dvst;

							lastReaderStatus = rdst;
							lastPutKtpStatus = ptst;
							lastVerifyFingerStatus = vrst;
							lastDeviceStatus = dvst;
						}
						memset(isiParsing, 0, sizeof(isiParsing));
						respoll = 0;
					}
				}
				else if (resultKirim == 88) {
					hasil = -1005;
					strcpy(error, "Send Data Timeout");
				}
				else {
					hasil = -1006;
					strcpy(error, "Send Data Failed");
				}
				portInUse = 0;
				DWORD  dwExitCode = 0;
				TerminateThread(hCommand, dwExitCode);
			}
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}

	__declspec(dllexport) int ektp_reset(char error[100], char type[2]) 
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");

		if (openstatus == 1)
		{
			while (portInUse == 1) {
				Sleep(500);
			}

			portInUse = 1;
			resultKirim = 88;
			char command[100];
			sprintf(command, "ektpreset#%s#\n;", type);
			sendCommandReceive(command);

			int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}

			if (resultKirim == 0) {
				int cnt = 0;
				int tt = 10 * 10;

				while (resreset == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (resreset == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}
				else {
					if (strcmp(isiParsing, "0") == 0) {
						hasil = 0;
						strcpy(error, "ERR_OK");
						Sleep(1000);
					}
					else {
						hasil = -1;
						strcpy(error, "ERR_ERROR");
					}
					resreset = 0;
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
			}

			portInUse = 0;
			DWORD  dwExitCode = 0;
			TerminateThread(hCommand, dwExitCode);
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}

	__declspec(dllexport) int ektp_verifyFinger(char error[100], char dm[80], int timeout, char ft[6], char opid[20], char opnik[20])
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");

		if (openstatus == 1)
		{
			while (portInUse == 1) {
				Sleep(500);
			}

			portInUse = 1;
			resultKirim = 88;
			char command[100];
			lastVerifyFingerStatus = 19;
			sprintf(command, "ektpfinger#%d#%s#%s#%s#%s#;\n", timeout, ft, opid, opnik, dm);
			sendCommandReceive(command);

			int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}

			if (resultKirim == 0) {
				int cnt = 0;
				int tt = (timeout+1) * 10;

				while (resfinger == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (resfinger == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}
				else {
					char *split1 = strtok(isiParsing, ",");
					if (strcmp(split1, "error") == 0) {
						hasil = -1;
						strcpy(error, strtok(NULL, ","));
						lastVerifyFingerStatus = 0;
						lastReaderStatus = 0;
					}
					else {
						if (strcmp(isiParsing, "Y") == 0) {
							strcpy(error, "Fingerprint match");
							lastVerifyFingerStatus = 35;
							lastReaderStatus = 0;
						}
						else {
							hasil = -1;
							strcpy(error, "Fingerprint not match");
							lastVerifyFingerStatus = 30;
						}
					}

					resfinger = 0;
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
				lastVerifyFingerStatus = 24;
				lastReaderStatus = 0;
			}

			portInUse = 0;
			DWORD  dwExitCode = 0;
			TerminateThread(hCommand, dwExitCode);
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}

	__declspec(dllexport) int ektp_getDataDemography(char error[100], int timeout, char opid[20], char opnik[20], char auid[20], char aunik[20], char** ektpdata)
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");

		if (openstatus == 1)
		{
			while (portInUse == 1) {
				Sleep(500);
			}

			portInUse = 1;
			resultKirim = 88;
			char command[100];
			lastVerifyFingerStatus = 19;
			sprintf(command, "ektpdemog#%d#%s#%s#;\n", timeout, opid, opnik);
			sendCommandReceive(command);

			int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}

			if (resultKirim == 0) {
				int cnt = 0;
				int tt = (timeout + 1) * 10;

				while (resdemog == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (resdemog == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}
				else {
					char *p = isiParsing;
					int i = 0;
					char arr[3][1024];
					int x = 0;
					char temp[1024];
					char c = ',';

					while (*p != '\0')
					{
						if (*p != c && *p != '\0')
						{
							temp[x] = *p;
							x++;
						}
						else
						{
							temp[x] = '\0';
							strcpy(arr[i], temp);
							x = 0;
							i++;
							if (i == 2) {
								break;
							}
						}
						p++;
					}

					if (strcmp(arr[0], "error") == 0) {
						hasil = -1;
						strcpy(error, arr[1]);
					}
					else {
						strcpy(error, "ERR_OK");
						//strcpy(ektpdata,isiParsing);
						*ektpdata = isiParsing;
					}
					resdemog = 0;
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
			}

			portInUse = 0;
			DWORD  dwExitCode = 0;
			TerminateThread(hCommand, dwExitCode);
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}

	__declspec(dllexport) int ektp_getMachineLog(char error[100], char date[10], char** map)
	{
		int hasil = 0;
		strcpy(error, "ERR_OK");

		if (openstatus == 1)
		{
			while (portInUse == 1) {
				Sleep(500);
			}

			portInUse = 1;
			resultKirim = 88;
			char command[100];
			sprintf(command, "ektplog#%s#;\n", date);
			sendCommandReceive(command);

			int cnt2 = 0;
			int tt2 = 5;
			while (resultKirim == 88 && cnt2 < tt2) {
				Sleep(50);
				cnt2++;
			}

			if (resultKirim == 0) {
				int cnt = 0;
				int tt = 30 * 10;

				while (reslog == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (reslog == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}
				else {
					char isiLog2[102400];
					strcpy(isiLog2, isiParsing);
					char *split1 = strtok(isiParsing, ",");
					char *split2 = strtok(NULL, ",");
					if (strcmp(split1, "error") == 0) {
						hasil = -1;
						strcpy(error, split2);
					}
					else if (strcmp(split1, "kosong") == 0) {
						strcpy(error, "ERR_OK");
					}
					else {
						strcpy(error, "ERR_OK");
						*map = isiLog2;
					}

					reslog = 0;
				}
			}
			else if (resultKirim == 88) {
				hasil = -1005;
				strcpy(error, "Send Data Timeout");
			}
			else {
				hasil = -1006;
				strcpy(error, "Send Data Failed");
			}

			portInUse = 0;
			DWORD  dwExitCode = 0;
			TerminateThread(hCommand, dwExitCode);
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}
		
	__declspec(dllexport) int ektp_dispMessage(char error[100], char dspMessage[50])
	{
		int hasil = 0;

		if (openstatus == 1)
		{
			hasil = 1;
			strcpy(error, "Device not supported");
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}

	__declspec(dllexport) int ektp_switchMode(char error[100], int mode)
	{
		int hasil = 0;

		if (openstatus == 1)
		{
			hasil = 1;
			strcpy(error, "Device not supported");
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}

	__declspec(dllexport) int ektp_setPowText(char error[100], char powText[256])
	{
		int hasil = 0;

		if (openstatus == 1)
		{
			hasil = 1;
			strcpy(error, "Device not supported");
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}

	__declspec(dllexport) int ektp_resPowText(char error[100])
	{
		int hasil = 0;

		if (openstatus == 1)
		{
			hasil = 1;
			strcpy(error, "Device not supported");
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}

		return hasil;
	}
	__declspec(dllexport) int ektp_update(char error[100], char updateApp[256])
	{
		int hasil = -1;
		FILE *file;
		char *buffer;
		unsigned long fileLen;

		//Open file
		file = fopen(updateApp, "rb");
		if (!file)
		{
			hasil = -1;
			strcpy(error, "Unable to open file");
			return hasil;
		}

		//Get file length
		fseek(file, 0, SEEK_END);
		fileLen = ftell(file);
		fseek(file, 0, SEEK_SET);
		//printf("Panjang file:%d\n",fileLen);

		//Allocate memory
		buffer = (char *)malloc(fileLen + 1);
		if (!buffer)
		{
			//fprintf(stderr, "Memory error!");
			fclose(file);
			hasil = -1;
			strcpy(error, "Cannot alocate memory to update");
			return hasil;
		}

		//Read file contents into buffer
		fread(buffer, fileLen, 1, file);
		fclose(file);

		//Rubah byte ke hexa string
		int i;
		char* hasils = (char*)malloc(((fileLen - 1) * 2) - 1 * sizeof(char *));
		//char hasils[xx];
		char hex_char[17] = "0123456789abcdef";

		for (i = 0; i < fileLen; ++i)
		{
			// High nybble
			hasils[i << 1] = hex_char[(buffer[i] >> 4) & 0x0f];
			// Low nybble
			hasils[(i << 1) + 1] = hex_char[buffer[i] & 0x0f];
		}
		//printf("setelah convert\nHasil convert:%s",hasils);
		free(buffer);

		int hasilkirim;

		char dt[100];
		int z;
		for (z = 0; z<100; z++) {
			//printf("%c,",buffer[awal+z]);
			dt[z] = hasils[z];
		}
		dt[100] = '\0';

		if (openstatus == 1)
		{
			//br = TRUE;
			char command[100];
			fflush(stdin);
			sprintf(command, "ektpupdateinit#%d#%s#;", fileLen, dt);
			int result = 0;// sendCommandOnly(command);
			int timeout = 2;
			if (result == 0) {
				//br = FALSE;
				Sleep(500);
				int cnt = 0;
				int tt = timeout * 10;

				while (resupdateinit == 0 && cnt < tt) {
					Sleep(100);
					cnt++;
				}

				if (resupdateinit == 0) {
					hasil = -1007;
					strcpy(error, "Receive Data Timeout");
				}
				else {

					if (strcmp(isiParsing, "error") == 0) {
						hasil = -1;
						strcpy(error, "ERR_ERROR");
					}
					else if (strcmp(isiParsing, "0") == 0) {
						hasilkirim = kirimFile2(fileLen, hasils);
						if (hasilkirim == 0) {
							hasil = hasilkirim;
							strcpy(error, "ERR_OK");
							//nunggurespon = 1;
							CloseHandle(hComm);
							openstatus = 0;
						}
						else {
							hasil = hasilkirim;
							strcpy(error, "Cannot send update file");
						}
					}
					else if (strcmp(isiParsing, "2") == 0) {
						hasil = -1018;
						strcpy(error, "Invalid file update version");
					}
					else if (strcmp(isiParsing, "3") == 0) {
						hasil = -1017;
						strcpy(error, "Invalid file update type");
					}
					else {
						hasil = -1;
						strcpy(error, "Cannot update reader");
						//printf("isi:%s",isiUpdateInit);
					}

					resupdateinit = 0;
				}
			}
		}
		else {
			hasil = -1012;
			strcpy(error, "Device Not Open");
		}
		free(hasils);

		return hasil;
	}
}
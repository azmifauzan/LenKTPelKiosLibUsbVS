// SampleApiVS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>

typedef int(*getDllFunc)(char[100], char[100]);
typedef int(*openCloseDllFunc)(char[100]);
typedef int(*infoDllFunc)(char[100], char[50], char[50], char[50], char[30]);
typedef int(*pollDllFunc)(char[100], unsigned char*, unsigned char*, unsigned char*, unsigned char*);
typedef int(*putDllFunc)(char[100], char[80], int, char[6]);
typedef int(*fingerDllFunc)(char[100], char[80], int, char[6], char[20], char[20]);
typedef int(*demogDllFunc)(char[100], int, char[20], char[20], char[20], char[20], char**);

HINSTANCE hInstLibrary;

void open() {
	openCloseDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (openCloseDllFunc)GetProcAddress(hInstLibrary, "ektp_open");
		if (_getDllFunc) {
			char error[100];
			char version[100];
			int status = _getDllFunc(error);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void close() {
	openCloseDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (openCloseDllFunc)GetProcAddress(hInstLibrary, "ektp_close");
		if (_getDllFunc) {
			char error[100];
			char version[100];
			int status = _getDllFunc(error);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void info() {
	infoDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (infoDllFunc)GetProcAddress(hInstLibrary, "ektp_info");
		if (_getDllFunc) {
			char error[100];
			char hardwareId[50];
			char sn[50];
			char firmwareVersion[50];
			char statusDevice[50];
			int status = _getDllFunc(error, hardwareId, sn, firmwareVersion, statusDevice);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
			printf("Hardware Id = %s\n", hardwareId);
			printf("S/N = %s\n", sn);
			printf("Firmware = %s\n", firmwareVersion);
			printf("Status Device = %s\n", statusDevice);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void poll() {
	pollDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (pollDllFunc)GetProcAddress(hInstLibrary, "ektp_poll");
		if (_getDllFunc) {
			char error[100];
			unsigned char readerStatus = 0;
			unsigned char putKTPStatus = 0;
			unsigned char verifyFingerStatus = 0;
			unsigned char deviceStatus = 0;
			int status = _getDllFunc(error, &readerStatus, &putKTPStatus, &verifyFingerStatus, &deviceStatus);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
			printf("Reader Status = %d\n", readerStatus);
			printf("PutKTP Status = %d\n", putKTPStatus);
			printf("Verify Status = %d\n", verifyFingerStatus);
			printf("Device Status = %d\n", deviceStatus);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}


void getdll() {
	getDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (getDllFunc)GetProcAddress(hInstLibrary, "ektp_getDLL");
		if (_getDllFunc) {
			char error[100];
			char version[100];
			int status = _getDllFunc(error, version);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
			printf("Version = %s\n", version);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void getapi() {
	getDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (getDllFunc)GetProcAddress(hInstLibrary, "ektp_getAPI");
		if (_getDllFunc) {
			char error[100];
			char version[100];
			int status = _getDllFunc(error, version);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
			printf("Version = %s\n", version);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void putktp() {
	putDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (putDllFunc)GetProcAddress(hInstLibrary, "ektp_putKTP");
		if (_getDllFunc) {
			char error[100];
			char dspMessage[80];
			int timeout = 10;
			char fingerType[6];
			strcpy(dspMessage,"put ktp");
			int status = _getDllFunc(error, dspMessage, timeout, fingerType);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
			printf("Finger Type = %s\n", fingerType);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void verify() {
	fingerDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (fingerDllFunc)GetProcAddress(hInstLibrary, "ektp_verifyFinger");
		if (_getDllFunc) {
			char error[100];
			char dspMessage[80];
			int timeout = 10;
			char fingerType[6];
			char operatorId[20];
			char operatorNIK[20];
			strcpy(fingerType, "2R");
			strcpy(operatorId, "id1234");
			strcpy(operatorNIK, "12345");
			strcpy(dspMessage, "my finger");
			int status = _getDllFunc(error, dspMessage, timeout, fingerType, operatorId, operatorNIK);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void verify2() {
	fingerDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (fingerDllFunc)GetProcAddress(hInstLibrary, "ektp_verifyFinger");
		if (_getDllFunc) {
			char error[100];
			char dspMessage[80];
			int timeout = 10;
			char fingerType[6];
			char operatorId[20];
			char operatorNIK[20];
			strcpy(fingerType, "2L");
			strcpy(operatorId, "id1234");
			strcpy(operatorNIK, "12345");
			strcpy(dspMessage, "my finger");
			int status = _getDllFunc(error, dspMessage, timeout, fingerType, operatorId, operatorNIK);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

void demog() {
	demogDllFunc _getDllFunc;
	if (hInstLibrary)
	{
		_getDllFunc = (demogDllFunc)GetProcAddress(hInstLibrary, "ektp_getDataDemography");
		if (_getDllFunc) {
			char error[100];
			char dspMessage[80];
			int timeout = 10;
			char operatorId[20];
			char operatorNIK[20];
			char authorizatorId[20];
			char authorizatorNIK[20];
			char* ektpData = nullptr;
			strcpy(authorizatorId, "auid1234");
			strcpy(authorizatorNIK, "54321");
			strcpy(operatorId, "id1234");
			strcpy(operatorNIK, "12345");
			int status = _getDllFunc(error, timeout, operatorId, operatorNIK, authorizatorId, authorizatorNIK, &ektpData);
			printf("Status = %d\n", status);
			printf("Error = %s\n", error);
			printf("Data = %s\n", ektpData);
		}
	}
	else
	{
		std::cout << "DLL Failed To Load!" << std::endl;
	}
}

int main()
{
	hInstLibrary = LoadLibrary("LenKTPelKiosLibUsb.dll");

	int pil = 0;
	while (pil != 99)
	{
		puts("Sample API VS Len KTP-el Reader BCA");
		puts("================================");
		puts("1. ektp_getDLL");
		puts("2. ektp_getAPI");
		puts("3. ektp_open");
		puts("4. ektp_close");
		puts("5. ektp_info");
		puts("6. ektp_poll");
		puts("7. ektp_reset");
		puts("8. ektp_getMachineLog");
		puts("9. ektp_dispMessage");
		puts("10. ektp_putKTP");
		puts("11. ektp_verifyFinger (Right)");
		puts("12. ektp_verifyFinger (Left)");
		puts("13. ektp_getDataDemography");
		puts("14. ektp_switchMode");
		puts("15. ektp_setPowText");
		puts("16. ektp_resPowText");
		puts("17. clrscr");
		puts("18. Put KTP + Poll");
		puts("19. Put KTP + KILL");
		puts("20. ektp_update");
		puts("21. continuous poll");
		puts("77. always poll with read ktp");
		puts("88. continuous poll with read ktp cycle");
		puts("99. Exit");
		puts("================================");
		printf("Your choice : "); scanf("%d", &pil);
		puts("");
		switch (pil)
		{
			case 1: getdll();
				break;
			case 2: getapi();
				break;
			case 3: open();
				break;
			case 4: close();
				break;
			case 5: info();
				break;
			case 6: poll();
				break;
			/*case 7: resetktp2();
				break;
			case 8: getlog2();
				break;
			case 9: display2();
				break;			
			*/
			case 10: putktp();
				break;
			case 11: verify();
				break;
			case 12: verify2();
				break;
			case 13: demog();
				break;
			/*case 14: switchmode2();
				break;
			case 15: setpow2();
				break;
			case 16: respow2();
				break;
			case 17: system("cls");
				break;
			case 18: cobalagi();
				break;
			case 19: coba3();
				break;
			case 20: updatefile();
				break;
			case 21: keeppoll();
				break;
			case 22: resetvar();
				break;
			case 23: keeppolltanparespon();
				break;
			case 24: getVersi();
				break;
			case 66: hello();
				break;
			case 77: alwayspollktpcycle();
				break;
			case 88: ktpcycle();
				break;*/
			default: pil = 99;
				break;
		}
		puts(""); puts("");
	}
	FreeLibrary(hInstLibrary);

	std::cin.get();
    return 0;
}


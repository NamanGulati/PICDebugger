// PICDebugger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ftd2xx.h"
#include "iostream"

using namespace std;

int setup();
int readData();
int transmit(char[]);

FT_HANDLE ftHandle;
FT_STATUS ftStatus;

int main()
{	
	if (setup() == 0) {
		while (1) {
			printf("1. Read PIC Status\n2. Send Console Input To PIC\n");
			int input;
			cin >> input;
			if (input == 1) {
				if (readData() != 0) {
					cout << "read failed\n";
				}
			}
			else {
				cout << "Enter input:\n";
				char input[256];
				cin >> input;
				if (transmit(input) != 0) {
					cout << "transmit failed";
				}
			}
		}
	}
	else {
		printf("setup failed\n");
	}

	while (true)
	{

	}
	return 0;
}
int setup() {
	FT_HANDLE ftHandleTemp;
	DWORD numDevs;
	DWORD Flags;
	DWORD ID;
	DWORD Type;
	DWORD LocId;
	char SerialNumber[16];
	char Description[64];
	// create the device information list
	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus == FT_OK) {
		printf("Number of devices is %d\n", numDevs);
	}
	else {
		return -1;
	}
	if (numDevs > 0) {
		// get information for device 0
		ftStatus = FT_GetDeviceInfoDetail(0, &Flags, &Type, &ID, &LocId, SerialNumber,
			Description, &ftHandleTemp);
		if (ftStatus == FT_OK) {
			printf("Dev 0:\n");
			printf(" Flags=0x%x\n", Flags);
			printf(" Type=0x%x\n", Type);
			printf(" ID=0x%x\n", ID);
			printf(" LocId=0x%x\n", LocId);
			printf(" SerialNumber=%s\n", SerialNumber);
			printf(" Description=%s\n", Description);
			printf(" ftHandle=0x%x\n", ftHandleTemp);
		}
		else {
			return -1;
		}




		ftStatus = FT_Open(0, &ftHandle);
		if (ftStatus != FT_OK) {
			return -1;
		}
		ftStatus = FT_ClrRts(ftHandle);
		if (ftStatus == FT_OK) {
			//RTS cleared
			printf("RTS Cleared\n");

		}
		else {
			return -1;
		}
		ftStatus = FT_SetBaudRate(ftHandle, 2400); // Set baud rate to 115200
		if (ftStatus == FT_OK) {
			// FT_SetBaudRate OK
			printf("baud ok\n");
		}
		else {
			// FT_SetBaudRate Failed
			printf("baud failed\n");
			return -1;

		}

	}
	return 0;
}

	int readData() {
		DWORD RxBytes = 3;
		DWORD BytesReceived;
		char RxBuffer[256];

			FT_SetRts(ftHandle);
			if (ftStatus == FT_OK) {
				//RTS cleared
				printf("RTS sent\n");

			}
			else {
				return -1;
			}
		FT_SetTimeouts(ftHandle, 1000, 0);
		ftStatus = FT_Read(ftHandle, RxBuffer, RxBytes, &BytesReceived);
			
		string ports[3] = { "PORTA","PORTB","PORTC" };
		if (ftStatus == FT_OK) {
			if (BytesReceived == 3) {
				printf("# Bytes Received == %d\n", BytesReceived);
				for (int i = 0;i < 3;i++) {
					printf("%s====", ports[i]);
					for (int j = 0;j < 8;j++) {
						printf("%c ", (RxBuffer[i] & (1 << j)) ? '1' : '0');
					}
				}

			}
		}
		else {
				// FT_Read Failed
				printf("Read failed\n");
				return -1;
			}
		FT_ClrRts(ftHandle);
		if (ftStatus == FT_OK) {
				//RTS cleared
				printf("RTS cleared\n");

			}
		else {
				return -1;
			}


		FT_Close(ftHandle);
		return 0;
	}
	
	int transmit(char input []) {
		
		DWORD BytesWritten;
		FT_PROGRAM_DATA data;
		
		ftStatus = FT_Open(0, &ftHandle);
		if (ftStatus != FT_OK) {
			// FT_Open failed
			return -1;
		}
		ftStatus = FT_Write(ftHandle, input, sizeof(input), &BytesWritten);
		if (ftStatus == FT_OK) {
			// FT_Write OK
		}
		else {
			// FT_Write Failed
			return -1;
		}
		FT_Close(ftHandle);
		return 0;
	}

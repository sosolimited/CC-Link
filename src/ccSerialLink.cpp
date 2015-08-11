//
//  ccSerialLink.cpp
//  emptyExample
//
//  Created by Sosolimited on 8/11/15.
//
//

#include "ccSerialLink.h"

using namespace ccLink;

ccSerialLink::ccSerialLink(){
	
}


ccSerialLink::~ccSerialLink(){
	
}



void ccSerialLink::fetchSerialData(){
	
	newChars.clear();
	
	//grab all available CC chars -- this could also be the test chars that the device outputs
	static float ccTimer = 0;
	while (serial.available() > 0)
	{
		unsigned char myByte = serial.readByte();
		
		//replace new line with space
		if ((myByte == 13) || (myByte == 10)) myByte = 32;
		
		if ((myByte != 32) || (lastChar != 32))
		{
			//PEND: disabling the printf for testing purposes
			//printf("%c", myByte);
			newChars.push_back(myByte);
			lastChar = myByte;
		}
		
		ccTimer = 0; //reset the timer
		isPDRSetup = true; //we also know it has been propely setup
		string status = "Serial is active at " + kComPort;

	}
	
}

void ccSerialLink::testPDR(){
	
	printf("Testing PDR-870 \n");
	
	messageQueue.push_back(1);
	messageQueue.push_back('?');
	
}

void ccSerialLink::setupPDR(){
	
	printf("Setup PDR-870 \n");
	
	messageQueue.push_back(1);
	messageQueue.push_back('@');
	messageQueue.push_back(1);
	messageQueue.push_back('A');
	messageQueue.push_back(1);
	messageQueue.push_back('M');
	
}

void ccSerialLink::updateMessageQueue(float iElapsed){
	
	static float messageTimer = 0;
	float messagePeriod = 0.25;
	
	messageTimer += iElapsed;
	if (messageTimer > messagePeriod)
	{
		messageTimer = 0;
		if (messageQueue.size() > 0)
		{
			unsigned char c = messageQueue.at(0);
			serial.writeByte(c);
			messageQueue.erase(messageQueue.begin());
		}
	}
}

void ccSerialLink::setup(string iIPAddress, int iPort){
	
	kServerIP = iIPAddress;
	
	// Setup Serial Devices
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

	if (serial.setup(kComPort, 9600))
	{
		serial.flush(true, true);
		printf("Serial is setup \n");
	}
}


void ccSerialLink::idle( float iTime ){
	
	fetchSerialData();
	
	//setup the PDR at the start, if we haven't already received chars
	static float setupTimer = 0;
	if (!isPDRSetup)
	{
//		setupTimer += elapsed;
//		if (setupTimer > 2.0)
//		{
			printf("\nInitial Setup of PDR-870 \n");
			setupPDR();
			isPDRSetup = true;
//		}
	}
	
	//automatically try to reconnect to PDR if we don't see any CC
//	if (isPDRSetup)
//	{
//		ccTimer += elapsed;
//		if (ccTimer > 30.0)
//		{
//			printf("\n\nERROR No CC: attempting to reconnect \n\n");
//			serialStatus->setString("No incoming CC in the past 30s");
//			setupPDR();
//			ccTimer = 0;
//		}
//	}
	
	//send messages over serial, if necessary
	if (messageQueue.size() > 0)updateMessageQueue(0);
	
	
	
	
}

bool ccSerialLink::getIsSerialConnected(){
	
	return true;
	
}

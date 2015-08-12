//
//  ccSerialLink.cpp
//  emptyExample
//
//  Created by Sosolimited on 8/11/15.
//
//

#include "ccSerialLink.h"

using namespace ccLink;
using namespace asio;
using namespace std;

ccSerialLink::ccSerialLink( std::shared_ptr<io_service> iService ){
	
}


ccSerialLink::~ccSerialLink(){
	
}



void ccSerialLink::listenForSerialData(){
	
	newChars.clear();
	
	//grab all available CC chars -- this could also be the test chars that the device outputs
	static float ccTimer = 0;
	
	// Check if serial is available?
	
//	
//	serial->async_read_some(asio::buffer(),
//									 [this] (std::error_code ec, std::size_t /*length*/)
//									 {
//
//										 
//									 }
	
											 
//	while (serial.available() > 0)
//	{
//		unsigned char myByte = serial.readByte();
//		
//		//replace new line with space
//		if ((myByte == 13) || (myByte == 10)) myByte = 32;
//		
//		if ((myByte != 32) || (lastChar != 32))
//		{
//			//PEND: disabling the printf for testing purposes
//			//printf("%c", myByte);
//			newChars.push_back(myByte);
//			lastChar = myByte;
//		}
//		
//		ccTimer = 0; //reset the timer
//		isPDRSetup = true; //we also know it has been propely setup
//		string status = "Serial is active at " + kComPort;
//
//	}
	
}

void ccSerialLink::testPDR(){
	
	printf("Testing PDR-870 \n");
	
	
	unsigned char o = 1;
	
	std::vector<asio::const_buffer> buffers;
	buffers.push_back(asio::const_buffer(&o, 1));
	buffers.push_back(asio::const_buffer("?", 1));

	serial->async_write_some(buffers, [this] (std::error_code ec, std::size_t) {
		
		cout << "WROTE TEST BUFF" << endl;
		
	});
	
}

void ccSerialLink::setupPDR(){
	
	printf("Setup PDR-870 \n");

	
	unsigned char o = 1;
	
	
	std::vector<asio::const_buffer> buffers;
	buffers.push_back(asio::const_buffer(&o, 1));
	buffers.push_back(asio::const_buffer("@", 1));
	buffers.push_back(asio::const_buffer(&o, 1));
	buffers.push_back(asio::const_buffer("A", 1));
	buffers.push_back(asio::const_buffer(&o, 1));
	buffers.push_back(asio::const_buffer("M", 1));
	
	
	serial->async_write_some(buffers, [this] (std::error_code ec, std::size_t) {
		
		cout << "WROTE DEM BUFFS" << endl;
		
	});
}



void ccSerialLink::setup(){

	io_service io;
	make_shared<serial_port>( io, kComPort );
	serial->set_option( serial_port_base::baud_rate( 9600 ));
	
	// Flush it
	if (serial->is_open()){
		::tcflush(serial->lowest_layer().native_handle(), TCIOFLUSH);
	}
}



void ccSerialLink::idle( float iTime ){
	
//	fetchSerialData();
	
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

	
}

bool ccSerialLink::getIsSerialConnected(){
	
	return true;
	
}

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

ccSerialLink::ccSerialLink( io_service &iService, string iName ) :
appIOService( iService ),
kComPort( iName )
{
	setup();

}


ccSerialLink::~ccSerialLink(){
	
}


void ccSerialLink::addNewCharHandler(const std::function<void ()> &iFn){
	
	newCharHandlers.push_back( iFn );
}

void ccSerialLink::addSetupHandler(const std::function<void ()> &iFn){
	
	setupHandlers.push_back( iFn );
}

void ccSerialLink::addSerialIdleHandler(const std::function<void ()> &iFn){
	
	serialIdleHandlers.push_back( iFn );
}



void ccSerialLink::callSetupHandlers(){

	
	for (auto f : setupHandlers){
		
		cout << "Dispatching handler" << endl;
		appIOService.post( f );
		

	}
}

void ccSerialLink::callNewCharHandlers(){

	for (auto f : newCharHandlers){
		
		appIOService.post( f );
	}
}

void ccSerialLink::callSerialIdleHandlers(){
	
	for (auto f : serialIdleHandlers){
		
		appIOService.post( f );
	}
}


void ccSerialLink::listenForSerialData(){
	
	cout << "Listening for serial data" << endl;
	
	array<char, 8> buf;
	
	serial->async_read_some(asio::buffer( buf, 8), [this] (std::error_code ec, std::size_t) {

		cout << "got a char" << endl;
		
		if (!isPDRSetup){
			cout << "pdr wasn't good" << endl;
			isPDRSetup = true;
		}
		
		this->listenForSerialData();
		
	});
	
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
		this->isPDRSetup = true;
		// listen for serial
		listenForSerialData();
		
	});

	callSetupHandlers();
	
}



void ccSerialLink::setup(){

	// This also opens the serial port
	serial = make_shared<serial_port>( appIOService, kComPort );

	serial->set_option( serial_port_base::baud_rate( 9600 ));
	// Flush it
	if (serial->is_open()){
		
		cout << "Serial port open, flushing buffers." << endl;
		::tcflush(serial->lowest_layer().native_handle(), TCIOFLUSH);
	}
}



void ccSerialLink::idle( float iTime ){

	if (!isPDRSetup)
	{
			printf("\nInitial Setup of PDR-870 \n");
			setupPDR();

	}
}

bool ccSerialLink::getIsSerialConnected(){
	
	return true;
	
}

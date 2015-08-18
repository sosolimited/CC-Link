//
//  ccSerialLink.cpp
//  emptyExample
//
//  Created by Sosolimited on 8/11/15.
//
//  Uses asio::serial to open a serial report and receive data
//  byte by byte from CC box (Model Link Electronics PDR-870)
//  Will notify subsribers of important serial events


#include "ccSerialLink.h"
#include <iostream>

using namespace soso;
using namespace asio;
using namespace std;

ccSerialLink::ccSerialLink( io_service &iService, const string &iComPort, int iBaudRate ) :
appIOService( iService ),
comPort( iComPort ),
baudRate( iBaudRate )
{
	serial = make_shared<serial_port>( appIOService, comPort );
	serial->set_option( serial_port_base::baud_rate( baudRate ) );

	if (serial->is_open()){
		// Flush serial port's input and output data
		// Gets rid of any data left over in serial buffer from previous sessions.
		::tcflush(serial->lowest_layer().native_handle(), TCIOFLUSH);
	}
}

void ccSerialLink::addNewCharHandler(const std::function<void (char)> &iFn){

	newCharHandlers.push_back( iFn );
}

void ccSerialLink::addSetupHandler(const std::function<void ()> &iFn){
	
	setupHandlers.push_back( iFn );
}

void ccSerialLink::addSerialIdleHandler(const std::function<void ()> &iFn){
	
	serialIdleHandlers.push_back( iFn );
}

void ccSerialLink::addSerialClosedHandler(const std::function<void ()> &iFn){
	
	serialClosedHandlers.push_back( iFn );
}


void ccSerialLink::callSetupHandlers(){

	for (auto f : setupHandlers){
		appIOService.post( f);
		
	}
}

void ccSerialLink::callNewCharHandlers(char iNewChar){

	for (auto f : newCharHandlers){
		
		auto boundFunction = [f, iNewChar] () {
			
			f( iNewChar );
			
		};
		
		appIOService.post( boundFunction );
	}
}

void ccSerialLink::callSerialIdleHandlers(){
	
	for (auto f : serialIdleHandlers){
		
		appIOService.post( f );
	}
}

void ccSerialLink::callSerialClosedHandlers(){
	
	for (auto f : serialClosedHandlers){
		
		appIOService.post( f );
	}
}

// Wait for data to be receievd via serial port
void ccSerialLink::listenForSerialData(){

	// Making it a shared pointer so it doesn't get deleted!
	auto data = make_shared<char>();
	
	// if we read 1 byte at a time, seems too slow.  we miss data
	serial->async_read_some(asio::buffer( data.get(), 1), [this, data] (std::error_code ec, std::size_t) {

		if (!ec){
			
			callNewCharHandlers( *data );
		

		// Reset serial timer
		this->serialTimer = 0;
		
		// If we receive serial data, our connection is properly set up
		if (!isPDRSetup){
			isPDRSetup = true;
		}
		
		}else{
			
			cout << "Error receiving serial data " << ec << endl;
			
		}
		
		// Keep listening for serial data
		this->listenForSerialData();
		
	});
}

void ccSerialLink::testPDR(){
	
	cout << "Testing PDR-870" << endl;

	messageQueue.push_back( 1 );
	messageQueue.push_back( '?' );
	
}

void ccSerialLink::setupPDR(){
	
	cout << "Setup PDR-870" << endl;

	messageQueue.push_back( 1 );
	messageQueue.push_back( '@' );
	messageQueue.push_back( 1 );
	messageQueue.push_back( 'A' );
	messageQueue.push_back( 1 );
	messageQueue.push_back( 'M' );
	
	isPDRSetup = true;
	
}

void ccSerialLink::updateMessageQueue(float iDt){
	
	messageTimer += iDt;
	
	if (messageTimer > messagePeriod)
	{
		messageTimer = 0;
		if (messageQueue.size() > 0)
		{
			unsigned char c = messageQueue.at(0);
			
			// Write a message queue.
			std::vector<asio::const_buffer> buffers;
			buffers.push_back(asio::const_buffer(&c, 1));
			
			serial->write_some( buffers );
			
			messageQueue.erase(messageQueue.begin());
		}
	}
}

void ccSerialLink::update( float iDt ){

	currTime += iDt;
	serialTimer += iDt;
	
	if (!serial->is_open()){
		
	}
	
	if (serialTimer > serialTimeout){
		
		// Attempt to reconnect
		if (messageQueue.empty()){
			
			// Call serial timeout
			callSerialIdleHandlers();

			setupPDR();
			
			// Reset timer
			serialTimer = 0;
		
		}
	}
	
	// Don't call setupPDR if we're in the middle of
	// sending setup messages
	if (!isPDRSetup && messageQueue.empty())
	{
			printf("\nInitial Setup of PDR-870 \n");
			setupPDR();

	}
	
	// Send messages in the queue
	updateMessageQueue( iDt );
	
	if (isPDRSetup && messageQueue.empty() ){
	
		if (!calledSetup){
		
			callSetupHandlers();
			listenForSerialData();
			calledSetup = true;
		}
	}
}

bool ccSerialLink::getIsSerialConnected(){
	
	return serial->is_open();
	
}

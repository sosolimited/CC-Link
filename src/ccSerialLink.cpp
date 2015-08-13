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


void ccSerialLink::addNewCharHandler(const std::function<void (char)> &iFn){
	
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


void ccSerialLink::listenForSerialData(){
	
//	cout << "Listening for serial data" << endl;
	
	enum { max_length = 1024 };
	char data_[max_length];
	
	serial->async_read_some(asio::buffer( data_, max_length), [this, data_] (std::error_code ec, std::size_t) {

		if (!ec){
			
		}
		cout << data_ << endl;
		//cout << "got a char" <<  &buf << endl;
		
		if (!isPDRSetup){
			cout << "pdr wasn't good" << endl;
			isPDRSetup = true;
		}
		
		this->listenForSerialData();
		
	});
	
}

void ccSerialLink::testPDR(){
	
	printf("Testing PDR-870 \n");

	messageQueue.push_back( 1 );
	messageQueue.push_back( '?' );
	
}

void ccSerialLink::setupPDR(){
	
	printf("Setup PDR-870 \n");

	unsigned char o = 1;
	
	messageQueue.push_back( 1 );
	messageQueue.push_back( '@' );
	messageQueue.push_back( 1 );
	messageQueue.push_back( 'A' );
	messageQueue.push_back( 1 );
	messageQueue.push_back( 'M' );
	
	isPDRSetup = true;
	
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

	float messagePeriod = 0.25;
	
	if (!isPDRSetup)
	{
			printf("\nInitial Setup of PDR-870 \n");
			setupPDR();

	}
	
	messageTimer += iTime;
	
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
	
	if (isPDRSetup && messageQueue.empty() ){
	
		if (!calledSetup){
		
			callSetupHandlers();
			listenForSerialData();
			calledSetup = true;
		}
	}
}

bool ccSerialLink::getIsSerialConnected(){
	
	return true;
	
}

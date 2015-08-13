//
//  ccSerialLink.h
//  emptyExample
//
//  Created by Sosolimited on 8/11/15.
//
//

#pragma once

#include <string>
#include "ofSerial.h"

// Using non-Boost ASIO
#define ASIO_STANDALONE 1
#include "asio.hpp"

namespace ccLink {

	class ccSerialLink {
		
	public:
		
		ccSerialLink( asio::io_service &iService, string iComPort );
		~ccSerialLink();
		
		void update( float dt );
		
		bool getIsSerialConnected();
		
		void addNewCharHandler( const std::function<void (char)> &iFn );
		void addSetupHandler( const std::function<void ()> &iFn );
		void addSerialIdleHandler( const std::function<void ()> &iFn );
		
		void setBaudRate(float iBaud);

	private:
		
		void callSetupHandlers();
		void callNewCharHandlers(char iNewChar);
		void callSerialIdleHandlers();

		void listenForSerialData();
		void setupPDR();
		void testPDR();
		
		void updateMessageQueue(float iDt);
	
		std::vector< unsigned char>			messageQueue;
		std::string comPort = "/dev/tty.usbserial-AL00APKE"; // default. should change.

		std::shared_ptr<asio::serial_port> serial;
		
		bool isPDRSetup =	false;
		
		asio::io_service &appIOService;
		
		std::vector< const std::function<void ()> >setupHandlers;
		std::vector< const std::function<void (char)> > newCharHandlers;
		std::vector< const std::function<void ()> > serialIdleHandlers;
		
		bool calledSetup = false;
		
		int baudRate = 9600;
		
		// Timer information
		float currTime = 0;
		float serialTimer = 0;
		float serialTimeout = 30.0f; // In seconds
		
		float messagePeriod = 0.25;
		float messageTimer = 0;
		
		
		
	};
	
} // namespace ccLink
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

// Is this needed?
#define RECONNECT_TIME 400

namespace ccLink {

	class ccSerialLink {
		
	public:
		
		ccSerialLink( asio::io_service &iService, string iName );
		~ccSerialLink();
		
		void setup();
		void idle( float dt );
		
		bool getIsSerialConnected();
		
		void addNewCharHandler( const std::function<void (char)> &iFn );
		void addSetupHandler( const std::function<void ()> &iFn );
		void addSerialIdleHandler( const std::function<void ()> &iFn );

	private:
		
		void callSetupHandlers();
		void callNewCharHandlers(char iNewChar);
		void callSerialIdleHandlers();

		void listenForSerialData();
		void setupPDR();
		void testPDR();
	
		std::vector< unsigned char>			messageQueue;
		
		float messageTimer = 0;
		
		std::string kComPort = "/dev/tty.usbserial-AL00APKE"; // default. should change.

		std::shared_ptr<asio::serial_port> serial;
		
		bool isPDRSetup =	false;
		
		asio::io_service &appIOService;
		
		std::vector< const std::function<void ()> >setupHandlers;
		std::vector< const std::function<void (char)> > newCharHandlers;
		std::vector< const std::function<void ()> > serialIdleHandlers;
		
		bool calledSetup = false;

	};
	
} // namespace ccLink
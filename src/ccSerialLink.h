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


#define ASIO_STANDALONE 1
#include "asio.hpp"

#define RECONNECT_TIME 400

namespace ccLink {

	class ccSerialLink {
		
	public:
		
		ccSerialLink( std::shared_ptr<asio::io_service> iService );
		~ccSerialLink();
		
		void setup();
		void idle( float dt );
		
		bool getIsSerialConnected();

	private:

		void listenForSerialData();
		void setupPDR();
		void testPDR();
	
		
		std::string kComPort = "/dev/tty.PL2303-000013FA";
		std::string kServerIP = "198.61.191.97";

		std::shared_ptr<asio::serial_port> serial;
		
		char								lastChar = 0;
		string							newChars;
		
		bool isPDRSetup =	false;
		
	};
	
	
} // namespace ccLink
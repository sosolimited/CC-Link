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
		
		ccSerialLink();
		~ccSerialLink();
		
		void setup(string iIPAddress, int iSerialPort);
		void idle( float dt );
		
		bool getIsSerialConnected();

	private:

		void fetchSerialData();
		void setupPDR();
		void testPDR();
		
		void updateMessageQueue( float iElapsed );
		
		std::string kComPort = "/dev/tty.PL2303-000013FA";
		std::string kServerIP = "198.61.191.97";

		ofSerial serial;

		
		char								lastChar = 0;
		string							newChars;
		
		bool isPDRSetup =	false;
		
		vector< unsigned char>			messageQueue;
		
	};
	
	
} // namespace ccLink
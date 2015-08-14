#pragma once

#include "ofMain.h"
#include "ccSerialLink.h"

class serialApp : public ofBaseApp{
  
public:
  
  void setup();
  void update();
  void draw();
  
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
	
	void buildOnscreenGraphics();
	void updateStatusText();
	
	// Functions to handle serial events
	void onSerialSetup();
	void onSerialIdle();
	void onReceivedChar( char iNewChar );
	
public:

	std::shared_ptr<soso::ccSerialLink> serialLink;

	asio::io_service ioService;
	std::shared_ptr< asio::io_service::work > constantWork;

	ofTrueTypeFont	arial14;
	
	// Number of chars to display onscreen
	int maxCharBufferSize = 400;
	
	// Store chars to display onscreen
	std::deque<char> charBuffer;
	
};


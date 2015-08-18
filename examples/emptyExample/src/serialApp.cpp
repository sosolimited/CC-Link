#include "serialApp.h"

using namespace soso;
using namespace std;
using namespace asio;

//--------------------------------------------------------------
void serialApp::setup(){
	
	// Give io_service work so it keeps running
	// Cinder does this behind the scenes
	constantWork = make_shared<asio::io_service::work>( ioService );

	// Create serialLink object with io service
	// An io service lets us do the work in our asynchronous functions
	// Cinder has a built-in io service
	serialLink = std::make_shared<ccSerialLink>( ioService, "/dev/tty.usbserial-AL00APKE", 14400 );
	serialLink = std::make_shared<ccSerialLink>( ioService, "/dev/tty.usbserial-AL00APKE", 9600 );
	
	// Add event handlers
	// These will be called when different serial events occur
	serialLink->addSetupHandler( [this] () {
		
		onSerialSetup();
		
	});
	
	serialLink->addSerialIdleHandler( [this] () {
		
		onSerialIdle();
		
	});
	
	serialLink->addNewCharHandler( [this] (char iNewChar) {
		
		onReceivedChar( iNewChar );
		
	});
	
	buildOnscreenGraphics();
}

// Serial port has been opened and CC
// box has been configured to send data
void serialApp::onSerialSetup(){
	
	ofLogNotice("Device has been setup.  Serial has been initialized.");
}

// Serial has not received data in > 30s
void serialApp::onSerialIdle(){
	
	ofLogNotice("Serial device is idle.");
}

// Serial port receives a char
void serialApp::onReceivedChar( char iNewChar ){
	
	cout << iNewChar;
	
	charBuffer.push_back( iNewChar );
	
	if (charBuffer.size() > maxCharBufferSize){
		
		charBuffer.pop_front();
	}
}

// Load fonts
void serialApp::buildOnscreenGraphics(){

	arial14.load("arial.ttf", 14);
	arial14.setLineHeight(18.0f);
	arial14.setLetterSpacing(1.037);

	ofSetColor(0,0,0);
	arial14.drawString("Status", 50, 50);
	
}

// Draw a simple status graphic
void serialApp::updateStatusText(){
	
	// Draw status text
	ofBackground(255,255,255);
	
	float startX = 15;
	float startY = 40;
	
	ofSetColor(0,0,0);
	
	string status = "Connected";
	
	if (!serialLink->getIsSerialConnected()){
		status = "Disconnected";
	}
	
	arial14.drawString("Serial Status: " + status, startX, startY);
	
	startY+=30;
	arial14.drawString("Time since last received: " + ofToString( serialLink->getSecondsSinceDataReceived(), 2) + " s", startX, startY);
	
	// Draw black rect with most
	// recent chars received
	ofSetColor(0,0,0);
	ofFill();
	
	startY+=30;
	ofRect(0, startY, 500,440);
	startY+=30;
	
	// Draw text in white
	ofSetColor(255,255,255, 255);
	
	// Make string from char buffer
	auto makeString = [this] (std::deque<char> d) {
		
		string s = "";
		for (auto c : d){
			s+=c;
		}
		
		return s;
	};
	
	arial14.drawString(makeString( charBuffer ), startX, startY);
	
}

//--------------------------------------------------------------
void serialApp::update(){
	
	// Elapsed time per frame for all our timers
	static float current = ofGetElapsedTimef();
	static float past = ofGetElapsedTimef();
	
	current = ofGetElapsedTimef();
	float elapsed = current - past;
	past = current;
	

	// Attempt to do any pending work in the ioService
	try {

		ioService.poll();
	
	} catch (exception &e) {
		
		ofLogNotice("Exception with io service polling " + ofToString(e.what()));
		
	}
	
	// Call update on the serialLink (to update timers)
	serialLink->update( elapsed );
	
	
}

//--------------------------------------------------------------
void serialApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  updateStatusText();
	
}

//--------------------------------------------------------------
void serialApp::keyPressed  (int key){
  
}

//--------------------------------------------------------------
void serialApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void serialApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void serialApp::mouseDragged(int x, int y, int button){
  
  
}

//--------------------------------------------------------------
void serialApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void serialApp::mouseReleased(int x, int y, int button){
  
  
}


//--------------------------------------------------------------
void serialApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void serialApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void serialApp::dragEvent(ofDragInfo dragInfo){ 
  
}



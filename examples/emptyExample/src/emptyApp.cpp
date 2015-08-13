#include "emptyApp.h"

using namespace ccLink;
using namespace std;
using namespace asio;

//--------------------------------------------------------------
void emptyApp::setup(){
	
	// Give io_service work so it keeps running
	// Cinder does this behind the scenes
	constantWork = make_shared<asio::io_service::work>( ioService );

	// To determine which COM port string to use, it's helpful to list serial devices
	// Mac format is "/dev/tty.usbserial-AL00APKE"
	// You can also type "ls /dev/tty.*" in terminal
	//		ofSerial s;
	//	s.listDevices();

	// Create serialLink object with io service
	// An io service lets us do the work in our asynchronous functions
	// Cinder has a built-in io service
	serialLink = std::make_shared<ccSerialLink>( ioService, "/dev/tty.usbserial-AL00APKE" );
	
	
	// Add event handlers
	serialLink->addSetupHandler( [this] () {
		
		onSerialSetup();
		
	});
	
	serialLink->addSerialIdleHandler( [this] () {
		
		onSerialIdle();
		
	});
	
	serialLink->addNewCharHandler( [this] (char iNewChar) {
		
		onReceivedChar( iNewChar );
		
	});
}

void emptyApp::onSerialSetup(){
	
	ofLogNotice("Device has been setup.  Serial has been initialized.");
}

void emptyApp::onSerialIdle(){
	
	ofLogNotice("Serial device is idle.");
}

void emptyApp::onReceivedChar( char iNewChar ){
	
	cout << iNewChar;
	
}


void emptyApp::buildOnscreenGraphics(){
	

	
	
}

void emptyApp::updateStatusText(){
	
	
}

//--------------------------------------------------------------
void emptyApp::update(){
	
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
	
//	// Call update on the serialLink (to update timers)
	serialLink->update( elapsed );
	
	updateStatusText();
}

//--------------------------------------------------------------
void emptyApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.

}

//--------------------------------------------------------------
void emptyApp::keyPressed  (int key){
  
}

//--------------------------------------------------------------
void emptyApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void emptyApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void emptyApp::mouseDragged(int x, int y, int button){
  
  
}

//--------------------------------------------------------------
void emptyApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void emptyApp::mouseReleased(int x, int y, int button){
  
  
}


//--------------------------------------------------------------
void emptyApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void emptyApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void emptyApp::dragEvent(ofDragInfo dragInfo){ 
  
}



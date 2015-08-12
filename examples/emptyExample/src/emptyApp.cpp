#include "emptyApp.h"

using namespace ccLink;
using namespace std;
using namespace asio;

//--------------------------------------------------------------
void emptyApp::setup(){
	
	// Give io_service work so it keeps running
	constantWork = make_shared<asio::io_service::work>( ioService );
  
  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();
	

	ofSerial s;
	
	s.listDevices();

	
	// Make serial object with io service
	serialLink = std::make_shared<ccSerialLink>( ioService, "/dev/tty.usbserial-AL00APKE" );
	
	serialLink->addSetupHandler( [this] () {
		
		onSerialSetup();
		
	});
}

void emptyApp::onSerialSetup(){
	
	ofLogNotice("Device has been setup.  Serial has been initialized.");
}

void emptyApp::onSerialIdle(){
	
}

void emptyApp::onReceivedByte(){
	
}


void emptyApp::buildOnscreenGraphics(){
	

	
	
}

void emptyApp::updateStatusText(){
	
	
}

//--------------------------------------------------------------
void emptyApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.

	try {
		
		//update
//		if (ioService.stopped()){
//			ioService.reset();
//		}
		ioService.poll();
	
	} catch (exception &e) {
		
		ofLogNotice("Exception with io service polling " + ofToString(e.what()));
		
	}
	
	serialLink->idle( ofGetElapsedTimef() );
	
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



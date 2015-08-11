#include "emptyApp.h"

using namespace ccLink;
using namespace std;

//--------------------------------------------------------------
void emptyApp::setup(){
  
  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();
	

	
	
	// Make text objects
	
	//serialLink = std::make_shared<ccSerialLink>();
	
	
}

void emptyApp::buildOnscreenGraphics(){
	

	
	
}

void emptyApp::updateStatusText(){
	
	
}

//--------------------------------------------------------------
void emptyApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.

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



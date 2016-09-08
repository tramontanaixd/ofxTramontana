#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    //OPEN THE CONNECTION WITH YOUR DEVICE
    iphone.start("192.168.1.2");
    
    //subscribe for the on Connection Open event
    iphone.onConnectionOpen(this, &testApp::connectionOpenHandler);
    
    
    ///DRAWING STUFF
    
    ofSetBackgroundAuto(false);
    ofBackground(255);
    
    xposition=0;
}
//--------------------------------------------------------------
void testApp::connectionOpenHandler(string &ipAddress){
    
    
    
    iphone.subscribeAttitude(20,this, &testApp::attitudeUpdate);
    /*
     You can call the subscribe attitude without the first numerical parameter that is the frequency update, default is 1Hz
     iphone.subscribeAttitude(this, &ofApp::attitudeUpdate);
     */
    
}
//--------------------------------------------------------------
void testApp::attitudeUpdate(vec3d &vec){
    printf("attitude %f, %f, %f\n",vec.x,vec.y,vec.z);
    prevAttitude=attitude;
    attitude=vec;
    
}
//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
   
    
    ofSetColor(255,0,0);
    ofDrawLine(xposition,ofMap(attitude.x,-3.14,3.14,ofGetHeight(),0.0),xposition-1,ofMap(prevAttitude.x,-3.14,3.14,ofGetHeight(),0.0));
    ofSetColor(0,255,0);
    ofDrawLine(xposition,ofMap(attitude.y,-3.14,3.14,ofGetHeight(),0.0),xposition-1,ofMap(prevAttitude.y,-3.14,3.14,ofGetHeight(),0.0));
    ofSetColor(0,0,255);
    ofDrawLine(xposition,ofMap(attitude.z,-3.14,3.14,ofGetHeight(),0.0),xposition-1,ofMap(prevAttitude.z,-3.14,3.14,ofGetHeight(),0.0));
    
    
    xposition=(xposition>=ofGetWidth())?0:xposition+1;
    if(xposition==0)
    {
        ofBackground(255);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

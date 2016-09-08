#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //OPEN THE CONNECTION WITH YOUR DEVICE
    iphone.start("192.168.1.2");
    
    iphone.onConnectionOpen(this, &testApp::connectionOpenHandler);
    
    
}
//--------------------------------------------------------------
void testApp::connectionOpenHandler(string &ipAddress){
    
    timePassedFromLastPing=ofGetElapsedTimef();
    
    /*Every device pings every 5seconds to notify that it's still on and connected, you can subscribe to the alive event*/
    iphone.isAlive(this,&testApp::iphoneAlive);
    
    /*
     It's a best practice to subscribe in the handler of the connection open event. 
     */
     iphone.subscribeTouch(this, &testApp::touched);
     iphone.subscribeDistance(this,&testApp::distanceChanged);
     iphone.subscribeAudioJack(this, &testApp::audioJackChanged);
     iphone.subscribePowerSource(this, &testApp::powerSourceChanged);
    
    
    /*Note that the magnetometer is giving you the intensity of the magnetic field and transmits just when a magnet is nearby.
     To test just put a small magnet close to your smartphone and you will get messages with the intensity of the magnetic field.
     */
     iphone.subscribeMagnetometer(this,&testApp::magnetometerUpdate);
     iphone.subscribeOrientation(this, &testApp::orientationChanged);
    
    /*This event is triggered when playing a video, the method videoEnded will be trieggered at the end of the playback.*/
    iphone.playVideo("http://www.sample-videos.com/video/mp4/240/big_buck_bunny_240p_1mb.mp4",this,&testApp::videoEnded);
    
}
//--------------------------------------------------------------
void testApp::iphoneAlive(int &device){
    timePassedFromLastPing=ofGetElapsedTimef();
}
//--------------------------------------------------------------
void testApp::audioJackChanged(int &audioJack){
    printf("audioJack %s\n",(audioJack==0.0)?"not in":"in");
}
//--------------------------------------------------------------
void testApp::powerSourceChanged(int &powerSource){
    printf("power source %d\n",powerSource);
}
//--------------------------------------------------------------
void testApp::touched(vec2d &vec){
    printf("touched %f, %f\n",vec.x,vec.y);
    touchCoordinates.x=vec.x;
    touchCoordinates.y=vec.y;
}

//--------------------------------------------------------------
void testApp::distanceChanged(int &proximity){
    printf("distance %d\n",proximity);
    
}
//--------------------------------------------------------------
void testApp::magnetometerUpdate(int &intensity){
    printf("magnetometer %d\n",intensity);
    
}
//--------------------------------------------------------------
void testApp::orientationChanged(int &orientation){
    printf("orientation %d\n",orientation);
    
}
//--------------------------------------------------------------
void testApp::videoEnded(int &end){
    printf("video ended\n");
    iphone.setColor(ofColor(123,123,123,123));
    
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(255);
    
    //TOUCH FEEDBACK
    ofSetColor(123,123,123);
    ofDrawCircle(touchCoordinates.x, touchCoordinates.y, 10);
    
    //IPHONE CONNECTED
    if(ofGetElapsedTimef()-timePassedFromLastPing>6.0 && ofGetElapsedTimef()-timePassedFromLastPing<10.0)
    {
        ofDrawBitmapString("device lost", 25, 15);
        ofSetColor(120,120,0);
        ofDrawCircle(10,10,5);
    }
    else if(ofGetElapsedTimef()-timePassedFromLastPing>10.0)
    {
        ofDrawBitmapString("device disconneced", 25, 15);
        ofSetColor(200,20,20);
        ofDrawCircle(10,10,5);
    }
    else{
        ofDrawBitmapString("device connected", 25, 15);
        ofSetColor(10,200,80);
        ofDrawCircle(10,10,5);
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='c')
    {
        iphone.setColor(ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
    }
    if(key=='v')
    {
        iphone.playVideo("http://www.sample-videos.com/video/mp4/240/big_buck_bunny_240p_1mb.mp4",this,&testApp::videoEnded);
    }
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

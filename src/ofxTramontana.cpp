/**********
 Developed by Pierluigi Dalla Rosa
 **********/

#import "ofxTramontana.h"
#include <sys/types.h> 

#pragma mark tramontana
ofxTramontana::ofxTramontana(){
    
    readyTosend=false;
    battery=-1;
}

void ofxTramontana::start(string newIpAddress)
{
    client=new ofxLibwebsockets::Client();
    ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();
    
    ipAddress=newIpAddress;
    options.host = newIpAddress;
    
    options.port =TRAMONTANA_DEFAULT_PORT;

    client->connect(options);
    client->addListener(this);
    readyTosend=false;
}
void ofxTramontana::reconnect()
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 10 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        if(!readyTosend)
        {
            start(ipAddress);
            reconnect();
        }
        
    });
}
void ofxTramontana::sendMessage(string message)
{
    client->send(message);
}
Boolean ofxTramontana::isConnected()
{
    return readyTosend;
}
#pragma mark WEBSOCKET METHODS
void ofxTramontana::onConnect( ofxLibwebsockets::Event& args ){
    readyTosend=true;
    printf("connected");
}

void ofxTramontana::onError( ofxLibwebsockets::Event& args ){
    readyTosend=false;
    reconnect();
    printf("%s\n",args.message.c_str());
    
}
void ofxTramontana::onOpen( ofxLibwebsockets::Event& args ){
    string s=args.conn.getClientIP();
    ofNotifyEvent(onOpenEvent,s,this);
}
void ofxTramontana::onClose( ofxLibwebsockets::Event& args ){
    readyTosend=false;
    reconnect();
}
void ofxTramontana::onIdle( ofxLibwebsockets::Event& args ){
    
}
void ofxTramontana::onMessage( ofxLibwebsockets::Event& args ){
  
    
    string directive=args.json["m"].asString();
    //*** ALIVE ping ***//
    if(ofIsStringInString(directive,"xm") || ofIsStringInString(directive, "xt") || ofIsStringInString(directive, "xa") || ofIsStringInString(directive, "x"))
    {
        int a= ofIsStringInString(directive,"xm")?TRAMONTANA_IOS:ofIsStringInString(directive,"xt")?TRAMONTANA_TVOS:ofIsStringInString(directive,"xa")?TRAMONTANA_ANDROID:TRAMONTANA_NANO;
        ofNotifyEvent(alive,a, this);
    }
    //*** batteryGet ***//
    else if(ofIsStringInString(directive, "batteryGet")){
        float batteryLevel=args.json["value"].asFloat();
    }
    //*** TOUCHED ***//
    else if(ofIsStringInString(directive, "touched")){
        vec2d vecTmp;
        vecTmp.x=args.json["x"].asFloat();
        vecTmp.y=args.json["y"].asFloat();
        ofNotifyEvent(touchEvent, vecTmp, this);
    }
    //*** ATTITUDE ***//
    else if(directive=="a"){
        vec3d vecTmp;
        vecTmp.x=args.json["r"].asFloat();
        vecTmp.y=args.json["p"].asFloat();
        vecTmp.z=args.json["y"].asFloat();
        ofNotifyEvent(attitudeEvent, vecTmp, this);
        
    }
    //*** DISTANCE ***//
    else if(ofIsStringInString(directive, "distanceChanged")){
        int iTmp=args.json["proximity"].asInt();
        ofNotifyEvent(distanceEvent,iTmp , this);
    }
    //*** AUDIO JACK ***//
    else if(ofIsStringInString(directive, "audioJackChanged")){
        int iTmp=args.json["in"].asInt();
        ofNotifyEvent(audioJackEvent,iTmp , this);
    }
    //*** POWER SOURCE ***//
    else if(ofIsStringInString(directive, "powerSourceChanged")){
        int iTmp=args.json["source"].asInt();
        ofNotifyEvent(powerSourceEvent,iTmp , this);
    }
    //*** MAGNETOMETER ***//
    else if(ofIsStringInString(directive, "magnetometerUpdate")){
        int iTmp=args.json["i"].asInt();
        ofNotifyEvent(magnetometerEvent,iTmp , this);
    }
    //*** ORIENTATION ***//
    else if(ofIsStringInString(directive, "orientationChanged")){
        int iTmp=args.json["value"].asInt();
        ofNotifyEvent(orientationEvent,iTmp , this);
    }
    //*** VIDEO ENDED ***//
    else if(ofIsStringInString(directive, "videoEnded")){
        int iTmp=1;
        ofNotifyEvent(videoEndedEvent,iTmp, this);
    }
    //*** BATTERY LEVEL ***//
    else if(ofIsStringInString(directive, "batteryGet")){
        
        float fTmp=args.json["value"].asFloat();
        battery=fTmp;
        
    }
}
void ofxTramontana::onBroadcast( ofxLibwebsockets::Event& args ){
    
}
#pragma mark TRAMONTANA NANO METHODS
void ofxTramontanaNANO::setServoEmbedded(int servoIndex, int value){
    sendMessage("{\"m\":\"srv\",\"n\":"+ofToString(servoIndex)+",\"v\":"+ofToString(value)+"}");
}
void ofxTramontanaNANO::setLedEmbedded(int ledIndex,ofColor ledColor){
    sendMessage("{\"m\":\"col\",\"n\":\""+ofToString(ledIndex)+"\",\"r\":\""+ofToString(floor(ledColor.r))+"\",\"g\":\""+ofToString(floor(ledColor.g))+"\",\"b\":\""+ofToString(floor(ledColor.b))+"\",\"a\":\""+ofToString(floor(ledColor.a))+"\"}");
}
void ofxTramontanaNANO::setLedsEmbedded(ofColor ledColor){
    sendMessage("{\"m\":\"all\",\"r\":\""+ofToString(floor(ledColor.r))+"\",\"g\":\""+ofToString(floor(ledColor.g))+"\",\"b\":\""+ofToString(floor(ledColor.b))+"\",\"a\":\""+ofToString(floor(ledColor.a))+"\"}");
}
void ofxTramontanaNANO::setLedsEmbedded(int startingLedIndex, int endLedIndex,ofColor ledColor){
    
}
void ofxTramontanaNANO::sendSerialMessageEmbedded(string message){
    sendMessage("{\"m\":\"tx\",\"v\":\""+message+"\"}");
}
void ofxTramontanaNANO::setRelayEmbedded(int relayIndex,Boolean value){
    sendMessage("{\"m\":\"rel\",\"n\":"+ofToString(relayIndex)+",\"v\":"+ofToString((value)?0.0:1.0)+"}");
}



#pragma mark TRAMONTANA APP METHODS
void ofxTramontanaAPP::setColor(ofColor screenColor){
    float red, green,blue,intensity;
    red=screenColor.r;
    green=screenColor.g;
    blue=screenColor.b;
    intensity=screenColor.a;
    if(red>1.0 || green>1.0 || blue>1.0)
    {
        red=red/255;
        green=green/255;
        blue=blue/255;
        intensity=intensity/255;
    }
    this->sendMessage("{\"m\":\"setColor\",\"r\":\""+ofToString(red)+"\",\"g\":\""+ofToString(green)+"\",\"b\":\""+ofToString(blue)+"\",\"a\":\""+ofToString(intensity)+"\"}");
    
}
void ofxTramontanaAPP::transitionColors(ofColor color1, ofColor color2, float duration){
    float r1, g1,b1,a1;
    r1=color1.r;
    g1=color1.g;
    b1=color1.b;
    a1=color1.a;
    if(r1>1.0 || g1>1.0 || b1>1.0)
    {
        r1=r1/255;
        g1=g1/255;
        b1=b1/255;
        a1=a1/255;
    }
    float r2, g2,b2,a2;
    r2=color2.r;
    g2=color2.g;
    b2=color2.b;
    a2=color2.a;
    if(r2>1.0 || g1>1.0 || b1>1.0)
    {
        r2=r2/255;
        g2=g2/255;
        b2=b2/255;
        a2=a2/255;
    }

    sendMessage("{\"m\":\"transitionColors\",\"r1\":\""+ofToString(r1)+"\",\"g1\":\""+ofToString(g1)+"\",\"b1\":\""+ofToString(b2)+"\",\"a1\":\""+ofToString(a1)+"\",\"r2\":\""+ofToString(r2)+"\",\"g2\":\""+ofToString(g2)+"\",\"b2\":\""+ofToString(b2)+"\",\"a2\":\""+ofToString(a2)+"\",\"duration\":\""+ofToString(duration)+"\"}");
}
void ofxTramontanaAPP::showImage(string url){
    sendMessage("{\"m\":\"showImage\",\"url\":\""+url+"\"}");
}
void ofxTramontanaAPP::playVideo(string url){
    sendMessage("{\"m\":\"playVideo\",\"url\":\""+url+"\"}");
}
void ofxTramontanaAPP::playAudio(string url){
    sendMessage("{\"m\":\"playAudio\",\"url\":\""+url+"\"}");
}
void ofxTramontanaAPP::setBrightness(float brightness){
    sendMessage("{\"m\":\"setBrightness\",\"b\":\""+ofToString(brightness)+"\"}");
}
#pragma mark TRAMONTANA IOS METHODS
void ofxTramontanaIOS::setFlashLight(float intensity){
    
    sendMessage("{\"m\":\"setLED\",\"value\":\"1\",\"in\":\""+ofToString(ofClamp(intensity, 0, 1.0))+"\"}");
}
void ofxTramontanaIOS::pulseFlashLight(int numberOfPulses,float duration ,float intensity){
    sendMessage("{\"m\":\"pulseLED\",\"t\":\""+ofToString(numberOfPulses)+"\",\"d\":\""+ofToString(duration)+"\",\"i\":\""+ofToString(ofClamp(intensity,0,1.0))+"\"}");
}
void ofxTramontanaIOS::takePicture(){
    sendMessage("{\"m\":\"takePicture\"}");
}
void ofxTramontanaIOS::takePicture(int cameraId){
    
    sendMessage("{\"m\":\"takePicture\",\"c\":\""+ofToString(cameraId)+"\"}");
}
void ofxTramontanaIOS::takePicture(int cameraId, Boolean interface){
    
    sendMessage("{\"m\":\"takePicture\",\"c\":\""+ofToString(cameraId)+"\""+ofToString((interface)?",\"i\":\"ui\"":"")+"}");
}
void ofxTramontanaIOS::makeVibrate(){
    sendMessage("{\"m\":\"makeVibrate\"}");
}

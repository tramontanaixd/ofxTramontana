/**********
 Developed by Pierluigi Dalla Rosa
 
 **********/



#include "ofMain.h"

#include "ofxLibwebsockets.h"

#define TRAMONTANA_DEFAULT_PORT 9092

#define TRAMONTANA_NANO        0
#define TRAMONTANA_TOUCH       1
#define TRAMONTANA_IOS         2
#define TRAMONTANA_TVOS        3
#define TRAMONTANA_ANDROID     4

struct vec2d{
    float x, y;
};
struct vec3d{
    float x, y,z;
};

class ofxTramontana{
    
public:
    ofxTramontana();
    void start(string newIpAddress);
    int tramontanaType;
    string ipAddress;
    Boolean readyTosend;
    void reconnect();
    Boolean isConnected();
    void sendMessage(string message);
    
    
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    void onError( ofxLibwebsockets::Event& args );
    
    //PING ALIVE
    ofEvent<int> alive;
    template <class ListenerClass, typename ListenerMethod>
    void isAlive(ListenerClass  * listener, ListenerMethod method){
        ofAddListener(alive, listener, method);
    }
    //ON OPEN
    ofEvent<string>   onOpenEvent;
    template <class ListenerClass, typename ListenerMethod>
    void onConnectionOpen(ListenerClass  * listener, ListenerMethod method){
        ofAddListener(onOpenEvent, listener, method);
    }
    //EVENTS
    
    ofEvent<string>     rxEvent;
    ofEvent<vec2d>      touchEvent;
    ofEvent<vec3d>      attitudeEvent;
    ofEvent<int>        audioJackEvent;
    ofEvent<int>        powerSourceEvent;
    ofEvent<int>        magnetometerEvent;
    ofEvent<int>        orientationEvent;
    ofEvent<int>        distanceEvent;
    ofEvent<int>        videoEndedEvent;
    
    //BATTERY FOR IOS
    float battery;
    
protected:
    ofxLibwebsockets::Client *client;

};
class ofxTramontanaNANO: public ofxTramontana{
public:
    
    
    void setServoEmbedded(int servoIndex, int value);
    void setLedEmbedded(int ledIndex,ofColor ledColor);
    void setLedsEmbedded(ofColor ledColor);
    void setLedsEmbedded(int startingLedIndex, int endLedIndex,ofColor ledColor);
    void sendSerialMessageEmbedded(string message);
    void setRelayEmbedded(int relayIndex,Boolean value);
    
    
    //subscribe method
  
    template <class ListenerClass, typename ListenerMethod>
    void subscribeRxEmbedded(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"srx\"}");
        ofAddListener(rxEvent, listener, method);
    }
    template <class ListenerClass, typename ListenerMethod>
    void releaseRxEmbedded(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"drx\"}");
        ofRemoveListener(rxEvent,listener,method);
    }
};
class ofxTramontanaAPP: public ofxTramontana{
    public:
    
        //subscribe method
  
        template <class ListenerClass, typename ListenerMethod>
        void subscribeTouch(ListenerClass  * listener, ListenerMethod method){
            sendMessage("{\"m\":\"registerTouch\"}");
            ofAddListener(touchEvent, listener, method);
        }
        template <class ListenerClass, typename ListenerMethod>
        void releaseTouch(ListenerClass  * listener, ListenerMethod method){
             sendMessage("{\"m\":\"releaseTouch\"}");
            ofRemoveListener(touchEvent,listener,method);
        }
    
        void setColor(ofColor screenColor);
        void transitionColors(ofColor color1, ofColor color2, float duration);
        void showImage(string url);
    
        template <class ListenerClass, typename ListenerMethod>
        void playVideo(string url,ListenerClass  * listener, ListenerMethod method){
            playVideo(url);
            ofAddListener(videoEndedEvent, listener, method);
        }
        void playVideo(string url);
        void playAudio(string url);
        void setBrightness(float brightness);
    
    
};
class ofxTramontanaIOS: public ofxTramontanaAPP{
    public:
    
    void setFlashLight(float intensity);
    void pulseFlashLight(int numberOfPulses,float duration ,float intensity);
    void takePicture();
    void takePicture(int cameraId);
    void takePicture(int cameraId, Boolean interface);
    void makeVibrate();
    
    //GET METHODS
    void getBatteryLevel();
    
    
    //subscribe method
    //*** ATTITUDE ***//
    
    template <class ListenerClass, typename ListenerMethod>
    void subscribeAttitude(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"registerAttitude\"}");
        ofAddListener(attitudeEvent, listener, method);
    }
    template <class ListenerClass, typename ListenerMethod>
    void subscribeAttitude(float frequency, ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"registerAttitude\",\"f\":\""+ofToString(frequency)+"\"}");
        ofAddListener(attitudeEvent, listener, method);
    }
    template <class ListenerClass, typename ListenerMethod>
    void releaseAttitude(ListenerClass*listener,ListenerMethod method){
        sendMessage("{\"m\":\"releaseAttitude\"}");
        ofRemoveListener(attitudeEvent,listener,method);
    }
    //*** AUDIO JACK ***//
 
    template <class ListenerClass, typename ListenerMethod>
    void subscribeAudioJack(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"registerAudioJack\"}");
        ofAddListener(audioJackEvent, listener, method);
    }
    template <class ListenerClass, typename ListenerMethod>
    void releaseAudioJack(ListenerClass*listener,ListenerMethod method){
        sendMessage("{\"m\":\"releaseAudioJack\"}");
        ofRemoveListener(audioJackEvent,listener,method);
    }
    //*** POWER SOURCE ***//
    template <class ListenerClass, typename ListenerMethod>
    void subscribePowerSource(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"registerPowerSource\"}");
        ofAddListener(powerSourceEvent, listener, method);
    };
    template <class ListenerClass, typename ListenerMethod>
    void releasePowerSource(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"releasePowerSource\"}");
        ofRemoveListener(powerSourceEvent,listener,method);

    };
    //*** MAGNETOMETER ***//
    template <class ListenerClass, typename ListenerMethod>
    void subscribeMagnetometer(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"registerMagnetometer\"}");
        ofAddListener(magnetometerEvent, listener, method);
    }
    template <class ListenerClass, typename ListenerMethod>
    void releaseMagnetometer(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"releaseMagnetometer\"}");
        ofRemoveListener(magnetometerEvent,listener,method);

    };
    //*** ORIENTATION ***//
    template <class ListenerClass, typename ListenerMethod>
    void subscribeOrientation(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"registerOrientation\"}");
        ofAddListener(orientationEvent, listener, method);
    }
    template <class ListenerClass, typename ListenerMethod>
    void releaseOrientation(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"releaseOrientation\"}");
        ofRemoveListener(orientationEvent,listener,method);

    }
    //*** DISTANCE ***//
    template <class ListenerClass, typename ListenerMethod>
    void subscribeDistance(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"registerDistance\"}");
         ofAddListener(distanceEvent, listener, method);
    }
    template <class ListenerClass, typename ListenerMethod>
    void releaseDistance(ListenerClass  * listener, ListenerMethod method){
        sendMessage("{\"m\":\"releaseDistance\"}");
        ofRemoveListener(distanceEvent,listener,method);

    }
    
};

class ofxTramontanaTVOS: public ofxTramontanaAPP{
public:
    //subscribe method
    void subsribeToEvent(int eventId);
};
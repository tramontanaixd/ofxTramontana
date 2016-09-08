#pragma once

#include "ofMain.h"
#include "ofxTramontana.h"

class testApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxTramontanaIOS iphone;
        
        //listeners
        void    connectionOpenHandler(string &ipAddress);
        
        void    iphoneAlive(int &device);
        float   timePassedFromLastPing=-10.0;
        void    touched(vec2d &vec);
        vec2d   touchCoordinates;
        void    distanceChanged(int &proximity);
        void    audioJackChanged(int &audioJack);
        void    powerSourceChanged(int &powerSource);
        void    magnetometerUpdate(int &intensity);
        void    orientationChanged(int &orientation);
        void    videoEnded(int &end);

		
};

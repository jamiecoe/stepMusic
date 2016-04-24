#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxOsc.h"

#define PORT 9999

class ofApp : public ofBaseApp{
    
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
    void audioOut(float * output, int bufferSize, int nChannels);
    
    /* OSC & Movement Data Section */
    ofxOscReceiver receive;
    float _accelZ, accelZ;
    bool checkMove;
    int stepCounter;
    
    
    /* Visualizer Sections */
    float circleX, circleY;
    vector<float> cX, cY;
    float radius, angle;
    int numPoints, cVectorSize;
    vector<ofColor> c;
    int hue, saturation, brightness, opacity;
    float hueIncr;
    
    
    /* Sound Section */
    maxiFilter filter, globalFilter;
    maxiOsc osc;
    maxiEnv env;
    maxiDelayline delay;
    float cutoff;
    int delayTime;
    int bufferSize, sampleRate;
    
    
    /* Musical Notation Section */
    double scale[12][5] = {
        {261.626, 311.127, 349.228, 391.995, 466.164},{261.626, 293.665, 349.228, 391.995, 466.164},{261.626, 293.665, 349.228, 391.995, 440.00},{261.626, 293.665, 329.628, 391.995, 440.00},{246.942, 293.665, 329.628, 391.995, 440.00},{246.942, 293.665, 329.628, 369.994, 440.00},{246.942, 277.183, 329.628, 369.994, 440.00},{246.942, 277.183, 329.628, 369.994, 415.305},{246.942, 277.183, 311.127, 369.994, 415.305},{233.082, 277.183, 311.127, 369.994, 415.305},{233.082, 277.183, 311.127, 349.228, 415.305},{233.082, 261.626, 311.127, 349.228, 415.305}
    }; //Frequenices for the 12 scales in the Circle Of Fifths
    double currentScale[5];
    int freqIndex, scaleStep, circleNoteIndex;
    
};

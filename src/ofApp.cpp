#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Black background
    ofSetBackgroundColor(0);
    
    /* OSC & Movement detection setup */
    receive.setup(PORT);
    ofSetFrameRate(60);
    
    checkMove = true;   // Decides if we're ready to check for movement
    stepCounter = 0;    // Keep track of number of steps taken
    
    
    /* Musical Notation & Sound Setup */
    
    // Initialise starting scale and notes
    freqIndex = 0;
    circleNoteIndex = 2;
    scaleStep = 0;
    
    for (int i = 0; i < 5; i++) {
        currentScale[i] = scale[0][i]; // Start with 1st scale
    }
    
    // Initialise filter cutoff, delay, oscillator envelope and output sound
    cutoff = 200;
    delayTime = 80000;
    
    env.setAttack(500);
    env.setDecay(10);
    env.setSustain(1);
    env.setRelease(1000);
    
    bufferSize = 512;
    sampleRate = 48000;
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4);
    
    
    
    /* Visuals Setup */
    
    // Color setup
    hue = 0;
    hueIncr = 255/12;
    saturation = 100;
    brightness = 255;
    opacity = 30;
    
    
    // Circles setup
    radius = 300;
    numPoints = 13;
    cVectorSize = 12000;  // Controls size of the fade-out tail of the coloured circle
    angle = 360 / numPoints;
    circleX = radius*sin(ofDegToRad(angle*circleNoteIndex));
    circleY = radius*cos(ofDegToRad(angle*circleNoteIndex));
    // Creates position and colour vectors for coloured circle
    for (int i = 0; i < cVectorSize; i++) {
        cX.push_back(circleX);
        cY.push_back(circleY);
        c.push_back(ofColor::fromHsb(hue, saturation, brightness, opacity));
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    /* iPhone movement data via OSC */
    
    while (receive.hasWaitingMessages()) {
        
        
        // Create message to store data
        ofxOscMessage m;
        receive.getNextMessage(&m);
        
        
        // Get Z-Axis movement data from accelerometer
        if (m.getAddress() == "/gyrosc/accel") {
            
            //Use a low-pass filter to smooth noisy data
            _accelZ = fabs(filter.lopass(m.getArgAsFloat(2),0.2));
        }
        
    }
    //Map accelerometer values to a more readable range of 0-100
    accelZ = (int)ofMap(_accelZ, 0, 0.5, 0, 100, true);
    
    
    
    /* Movement Speed Update Section */
    
    
    // If speeding up, gradually opens filter and increase saturation
    if (accelZ > 50) {
        cutoff += 20;
        if (saturation < 255) {
            saturation+=5;
        }
        else saturation = 255;
    }
    
    
    // If slowing down, gradually close filter and decrease saturation to starting positions
    if (accelZ < 20) {
        checkMove = true; // Re-opens gate to check for another step
        if (cutoff > 200) {
            cutoff -= 20;
        }
        else cutoff = 200;
        if (saturation > 100) {
            saturation--;
        }
        else saturation = 100;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    /* Musical Notation Section */
    
    //If we are ready, check for movement
    if (checkMove) {
        // An accelZ movement above 20 is considered a step
        if (accelZ > 20) {
            
            //add one to stepcounter
            stepCounter++;
            
            //every five steps, play a note
            if ((stepCounter % 5 == 0) && (env.trigger == 0)) {
                freqIndex = ofRandom(5);    // Assigns a random note from within current scale
                
                // work out which note has been played
                if (currentScale[freqIndex] == 233.082) circleNoteIndex = 0;
                else if (currentScale[freqIndex] == 246.942) circleNoteIndex = 1;
                else if(currentScale[freqIndex] == 261.626) circleNoteIndex = 2;
                else if(currentScale[freqIndex] == 277.183) circleNoteIndex = 3;
                else if (currentScale[freqIndex] == 293.665) circleNoteIndex = 4;
                else if (currentScale[freqIndex] == 311.127) circleNoteIndex = 5;
                else if (currentScale[freqIndex] == 329.628)
                    circleNoteIndex = 6;
                else if (currentScale[freqIndex] == 349.228)
                    circleNoteIndex = 7;
                else if (currentScale[freqIndex] == 369.994)
                    circleNoteIndex = 8;
                else if (currentScale[freqIndex] == 391.995) circleNoteIndex = 9;
                else if (currentScale[freqIndex] == 415.305) circleNoteIndex = 10;
                else if (currentScale[freqIndex] == 440.00) circleNoteIndex = 11;
                else if (currentScale[freqIndex] == 466.164) circleNoteIndex = 12;
                
                // Triggers note to play
                env.trigger = 1;
                
            }
            
            // Every fifty steps, changes scale and moves colour along spectrum
            if (stepCounter % 50 == 0) {
                scaleStep++;
                if (scaleStep > 11) scaleStep = 0;
                for (int i = 0; i < 5; i++) {
                    currentScale[i] = scale[scaleStep][i];
                }
                hue += hueIncr;
            }
            
            // Turns off movement checker so we can exit statement and untrigger the note
            checkMove = false;
        }
    }
    // Untriggers the note stright after triggering so we get short notes
    else env.trigger = 0;
    
    
    
    /* Visuals Section */
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    // calculate the distance between most recently triggered musical note and colour circle
    float xDist = radius*sin(ofDegToRad(angle*circleNoteIndex)) - circleX;
    float yDist = radius*cos(ofDegToRad(angle*circleNoteIndex)) - circleY;
    
    // creates a increment variable of 1/10th the distance
    float xIncr = xDist / 10;
    float yIncr = yDist / 10;
    
    // updates the position of the coloured circle so it travels to most recently triggered note
    circleX += xIncr;
    circleY += yIncr;
    
    // Updates vector postions and colour to the value of the one before it
    for (int i = 0; i < cX.size()-1; i++)
    {
        cX[i] = cX[i+1];
        cY[i] = cY[i+1];
        c[i] = c[i+1];
    }
    
    // Sets final vector to the most recent position / colour of circle
    cX[cX.size()-1] = circleX;
    cY[cY.size()-1] = circleY;
    c[c.size()-1] = ofColor::fromHsb(hue, saturation, brightness, opacity);
    
    
    // Draws coloured circle vectors
    for (int i = 0; i < cX.size()-1; i++)
    {
        
        // sets the colour of the circle depending on the current scale and draws it
        ofSetColor(c[i]);
        
        // Circle vectors get gradually smaller, creating effected of them fading out
        float circleSize = ofMap(i, 0, cX.size()-1, 2, 30);
        ofDrawCircle(cX[i], cY[i], circleSize);
    }
    
    ofSetColor(255); //set musical note points to colour white
    
    
    // draws musical note points in a circle
    for (int i = 0; i < numPoints; i++)
    {
        
        // Extra bit added after video - makes most recently note played turn red
        if (radius*sin(ofDegToRad(angle*i)) == radius*sin(ofDegToRad(angle*circleNoteIndex)) && circleX != radius*sin(ofDegToRad(angle*circleNoteIndex))) {
            ofSetColor(255,0,0);
        }
        else ofSetColor(255);
        ofDrawCircle(radius*sin(ofDegToRad(angle*i)), radius*cos(ofDegToRad(angle*i)),10);
    }
    
    cout<<_accelZ<<" "<<env.trigger<<endl;
    
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    /* Sound Output Section*/
    
    for (int i = 0; i < bufferSize; i++)
    {
        // Play the current note from the current scale
        double wave = env.adsr(osc.saw(currentScale[freqIndex]),env.trigger);
        
        // Run this note through a long delay
        double wave2 = delay.dl(wave, delayTime, 0.8);
        
        // Finally run this through a global lopass filter
        double final = globalFilter.lores(wave + wave2, cutoff, 0) * 0.9;
        
        // Output result
        output[i*nChannels    ]= final;
        output[i*nChannels + 1]= final;
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // Take screen shot images with 's' key
    if (key=='s') {
        ofSaveScreen(ofToString(ofGetFrameNum()) + ".png");
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

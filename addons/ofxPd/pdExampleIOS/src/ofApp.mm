/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxPd for documentation
 *
 */
#include "ofApp.h"

#import "MyViewController.h"
#import "AlertViewDelegate.h"

MyViewController * viewController = nil;
AlertViewDelegate * alertViewDelegate = nil;

#import <AVFoundation/AVFoundation.h>

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(127, 127, 127);
	//ofSetLogLevel("Pd", OF_LOG_VERBOSE); // see verbose info inside
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //-- create buttons.
    int buttonW = 140;
    int buttonH = 76;
    buttonExternalDisplayRect.width = buttonW;
    buttonExternalDisplayRect.height = buttonH;
    buttonExternalDisplayRect.x = (int)((ofGetWidth() - buttonW * 2) / 3);
    buttonExternalDisplayRect.y = (int)(ofGetHeight() * 0.75);
    
    buttonMirrorDisplayRect.width = buttonW;
    buttonMirrorDisplayRect.height = buttonH;
    buttonMirrorDisplayRect.x = (int)(buttonExternalDisplayRect.x * 2 + buttonW);
    buttonMirrorDisplayRect.y = buttonExternalDisplayRect.y;

	// double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;

	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// iOSAlerts will be sent to this
	ofxiOSAlerts.addListener(this);
	
	// set landscape
	//ofSetOrientation(OF_ORIENTATION_90_RIGHT;
	
	// try to set the preferred iOS sample rate, but get the actual sample rate
	// being used by the AVSession since newer devices like the iPhone 6S only
	// want specific values (ie 48000 instead of 44100)
	float sampleRate = setAVSessionSampleRate(44100);
	
	// the number if libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
	int ticksPerBuffer = 8; // 8 * 64 = buffer len of 512

	// setup OF sound stream using the current *actual* samplerate
	ofSoundStreamSetup(2, 1, this, sampleRate, ofxPd::blockSize()*ticksPerBuffer, 3);

	// setup Pd
	//
	// set 4th arg to true for queued message passing using an internal ringbuffer,
	// this is useful if you need to control where and when the message callbacks
	// happen (ie. within a GUI thread)
	//
	// note: you won't see any message prints until update() is called since
	// the queued messages are processed there, this is normal
	//
	if(!pd.init(2, 1, sampleRate, ticksPerBuffer-1, false)) {
		OF_EXIT_APP(1);
	}

	midiChan = 1; // midi channels are 1-16

	// subscribe to receive source names
	pd.subscribe("toOF");
	pd.subscribe("env");

	// add message receiver, required if you want to receieve messages
	pd.addReceiver(*this);   // automatically receives from all subscribed sources
	pd.ignoreSource(*this, "env");      // don't receive from "env"
	//pd.ignoreSource(*this);           // ignore all sources
	//pd.receiveSource(*this, "toOF");  // receive only from "toOF"

	// add midi receiver, required if you want to recieve midi messages
	pd.addMidiReceiver(*this);  // automatically receives from all channels
	//pd.ignoreMidiChannel(*this, 1);     // ignore midi channel 1
	//pd.ignoreMidiChannel(*this);        // ignore all channels
	//pd.receiveMidiChannel(*this, 1);    // receive only from channel 1

	// add the data/pd folder to the search path
	pd.addToSearchPath("pd/abs");

	// audio processing on
	pd.start();


}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(100, 100, 100);
	
	// since this is a test and we don't know if init() was called with
	// queued = true or not, we check it here
	if(pd.isQueued()) {
		// process any received messages, if you're using the queue and *do not*
		// call these, you won't receieve any messages or midi!
		pd.receiveMessages();
		pd.receiveMidi();
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {

	switch(key) {

		// musical keyboard if you have a usb keyboard
		case 'a':
			playTone(60);
			break;
		case 'w':
			playTone(61);
			break;
		case 's':
			playTone(62);
			break;
		case 'e':
			playTone(63);
			break;
		case 'd':
			playTone(64);
			break;
		case 'f':
			playTone(65);
			break;
		case 't':
			playTone(66);
			break;
		case 'g':
			playTone(67);
			break;
		case 'y':
			playTone(68);
			break;
		case 'h':
			playTone(69);
			break;
		case 'u':
			playTone(70);
			break;
		case 'j':
			playTone(71);
			break;
		case 'k':
			playTone(72);
			break;

		case ' ':
			if(pd.isReceivingSource(*this, "env")) {
				pd.ignoreSource(*this, "env");
				cout << "ignoring env" << endl;
			}
			else {
				pd.receiveSource(*this, "env");
				cout << "receiving from env" << endl;
			}
			break;

		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::presentExternalDisplayPopup(){
    
    alertViewDelegate = [[[AlertViewDelegate alloc] init] retain];
    
    UIAlertView * alert = [[[UIAlertView alloc] initWithTitle:@"External Display"
                                                      message:@"Select a External Display Mode"
                                                     delegate:alertViewDelegate
                                            cancelButtonTitle:@"Cancel"
                                            otherButtonTitles:nil] retain];
    
    vector<ofxiOSExternalDisplayMode> displayModes;
    displayModes = ofxiOSExternalDisplay::getExternalDisplayModes();
    
    [alert addButtonWithTitle:@"Preferred Mode"];
    
    for(int i = 0; i < displayModes.size(); i++){
        string buttonText = ofToString(displayModes[i].width) + " x " + ofToString(displayModes[i].height);
        [alert addButtonWithTitle:ofxStringToNSString(buttonText)];
    }
    
    [alert show];
    [alert release];
}

//--------------------------------------------------------------
void ofApp::presentExternalDisplayNotFoundPopup(){
    UIAlertView * alert = [[[UIAlertView alloc] initWithTitle:@"External Display"
                                                      message:@"External Display not found.\nConnect to an external display using a VGA adapter or AirPlay."
                                                     delegate:nil
                                            cancelButtonTitle:@"OK"
                                            otherButtonTitles:nil] retain];
    [alert show];
    [alert release];
}

void ofApp::presentMirroringFailedPopup(){
    UIAlertView * alert = [[[UIAlertView alloc] initWithTitle:@"Mirroring Failed"
                                                      message:@"Either you are not connected to an external display or your device does not support mirroring."
                                                     delegate:nil
                                            cancelButtonTitle:@"OK"
                                            otherButtonTitles:nil] retain];
    [alert show];
    [alert release];
}

//--------------------------------------------------------------
void ofApp::popupDismissed(){
    if(alertViewDelegate){
        [alertViewDelegate release];
        alertViewDelegate = nil;
    }
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    
    if(buttonExternalDisplayRect.inside(touch.x, touch.y)){
        if(ofxiOSExternalDisplay::isExternalScreenConnected()){
            presentExternalDisplayPopup();
        } else {
            presentExternalDisplayNotFoundPopup();
        }
    }
    
    if(buttonMirrorDisplayRect.inside(touch.x, touch.y)){
        if(ofxiOSExternalDisplay::isExternalScreenConnected()){
            if(ofxiOSExternalDisplay::isMirroring()){
                ofxiOSExternalDisplay::mirrorOff();
            } else {
                if(!ofxiOSExternalDisplay::mirrorOn()){
                    presentMirroringFailedPopup();
                }
                
            }
        } else {
            presentMirroringFailedPopup();
        }
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs &touch) {}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs &touch) {}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs &touch) {}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs& args) {}

//--------------------------------------------------------------
void ofApp::lostFocus() {}

//--------------------------------------------------------------
void ofApp::gotFocus() {}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning() {}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation) {}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::print(const std::string& message) {
	cout << message << endl;
}

//--------------------------------------------------------------
void ofApp::receiveBang(const std::string& dest) {
	cout << "OF: bang " << dest << endl;
}

void ofApp::receiveFloat(const std::string& dest, float value) {
	cout << "OF: float " << dest << ": " << value << endl;
}

void ofApp::receiveSymbol(const std::string& dest, const std::string& symbol) {
	cout << "OF: symbol " << dest << ": " << symbol << endl;
}

void ofApp::receiveList(const std::string& dest, const List& list) {
	cout << "OF: list " << dest << ": ";

	// step through the list
	for(int i = 0; i < list.len(); ++i) {
		if(list.isFloat(i))
			cout << list.getFloat(i) << " ";
		else if(list.isSymbol(i))
			cout << list.getSymbol(i) << " ";
	}

	// you can also use the built in toString function or simply stream it out
	// cout << list.toString();
	// cout << list;

	// print an OSC-style type string
	cout << list.types() << endl;
}

void ofApp::receiveMessage(const std::string& dest, const std::string& msg, const List& list) {
	cout << "OF: message " << dest << ": " << msg << " " << list.toString() << list.types() << endl;
}

//--------------------------------------------------------------
void ofApp::receiveNoteOn(const int channel, const int pitch, const int velocity) {
	cout << "OF MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
}

void ofApp::receiveControlChange(const int channel, const int controller, const int value) {
	cout << "OF MIDI: control change: " << channel << " " << controller << " " << value << endl;
}

// note: pgm nums are 1-128 to match pd
void ofApp::receiveProgramChange(const int channel, const int value) {
	cout << "OF MIDI: program change: " << channel << " " << value << endl;
}

void ofApp::receivePitchBend(const int channel, const int value) {
	cout << "OF MIDI: pitch bend: " << channel << " " << value << endl;
}

void ofApp::receiveAftertouch(const int channel, const int value) {
	cout << "OF MIDI: aftertouch: " << channel << " " << value << endl;
}

void ofApp::receivePolyAftertouch(const int channel, const int pitch, const int value) {
	cout << "OF MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << endl;
}

// note: pd adds +2 to the port num, so sending to port 3 in pd to [midiout],
//       shows up at port 1 in ofxPd
void ofApp::receiveMidiByte(const int port, const int byte) {
	cout << "OF MIDI: midi byte: " << port << " " << byte << endl;
}

//--------------------------------------------------------------
void ofApp::playTone(int pitch) {
	pd << StartMessage() << "pitch" << pitch << FinishList("tone") << Bang("tone");
}

//--------------------------------------------------------------
// set the samplerate the Apple approved way since newer devices
// like the iPhone 6S only allow certain sample rates,
// the following code may not be needed once this functionality is
// incorporated into the ofxiOSSoundStream
// thanks to Seth aka cerupcat
float ofApp::setAVSessionSampleRate(float preferredSampleRate) {
	
	NSError *audioSessionError = nil;
	AVAudioSession *session = [AVAudioSession sharedInstance];

	// disable active
	[session setActive:NO error:&audioSessionError];
	if (audioSessionError) {
		NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);
	}

	// set category
	[session setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionAllowBluetooth|AVAudioSessionCategoryOptionMixWithOthers|AVAudioSessionCategoryOptionDefaultToSpeaker error:&audioSessionError];
	if(audioSessionError) {
		NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);
	}

	// try to set the preferred sample rate
	[session setPreferredSampleRate:preferredSampleRate error:&audioSessionError];
	if(audioSessionError) {
		NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);
	}

	// *** Activate the audio session before asking for the "current" values ***
	[session setActive:YES error:&audioSessionError];
	if (audioSessionError) {
		NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);
	}
	ofLogNotice() << "AVSession samplerate: " << session.sampleRate << ", I/O buffer duration: " << session.IOBufferDuration;

	// our actual samplerate, might be differnt aka 48k on iPhone 6S
	return session.sampleRate;
}

//--------------------------------------------------------------
void ofApp::externalDisplayConnected(){
    [ofxiOSGetAppDelegate() createExternalWindowWithPreferredMode]; // create external window as soon as external screen is connected to prevent unwanted mirroring.
    ofLogVerbose("external display connected.");
    presentExternalDisplayPopup();
}

//--------------------------------------------------------------
void ofApp::externalDisplayDisconnected(){
    ofLogVerbose("external display disconnected.");
}

//--------------------------------------------------------------
void ofApp::externalDisplayChanged(){
    ofLogVerbose("external display changed.");
    if(ofxiOSExternalDisplay::isDisplayingOnDeviceScreen()){
        if(viewController){
            [viewController.view removeFromSuperview];
            [viewController release];
            viewController = nil;
        }
    } else if(ofxiOSExternalDisplay::isDisplayingOnExternalScreen()) {
        if(!viewController){
            viewController = [[[MyViewController alloc] init] retain];
            [ofxiOSGetAppDelegate().window addSubview:viewController.view];  // add to device window.
        }
    }
}


#include "testApp.h"
#include "ofxSimpleGuiToo.h"

#define RECONNECT_TIME 400

bool	sendResetButton = false;
bool	sendNewVisitorButton = false;
bool	sendStatusButton = false;
bool	sendSetInactivityButton = false;

float   visitorHeight, visitorWeight;


//--------------------------------------------------------------
void testApp::setup(){
	// we don't want to be running too fast
	ofSetVerticalSync(true);

	//some variables:
	//have we typed
	typed	= false;

	//our typing position
	pos		= 0;

	//our send and recieve strings
	msgTx	= "";
	msgRx	= "";

	//are we connected to the server - if this fails we
	//will check every few seconds to see if the server exists
	weConnected = tcpClient.setup("127.0.0.1", 21211);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
	tcpClient.setMessageDelimiter("\n");
	//tcpClient.setMessageDelimiter(";");

	connectTime = 0;
	deltaTime = 0;

	tcpClient.setVerbose(true);

    gui.addTitle("Send messages");
	gui.addButton("Send RESET", sendResetButton);
	gui.addButton("Send RECORD", sendNewVisitorButton);
	gui.addButton("Send STATUS", sendStatusButton);
    gui.addButton("Send SET INACTIVITY", sendSetInactivityButton);

    gui.addTitle("Sensors").setNewColumn(true);
    gui.addSlider("Height in inches", visitorHeight, 1.0, 96.0);
    gui.addSlider("Weight in pounds", visitorWeight, 1.0, 1000.0);


	gui.loadFromXML();

    gui.setAlignRight(true);

	gui.show();

    ofEnableAlphaBlending();
	HHLlogo.loadImage("HHL_logo.png");

}

//--------------------------------------------------------------
void testApp::update(){

    if(sendResetButton) {
		sendResetButton = false;
        tcpClient.send("RESET");
    }

    if(sendNewVisitorButton) {
		sendNewVisitorButton = false;

		string randomVisitorID = ofToString(int(ofRandom(0,9.999))) +
                                 ofToString(int(ofRandom(0,9.999))) +
                                 ofToString(int(ofRandom(0,9.999))) +
                                 ofToString(int(ofRandom(0,9.999))) +
                                 ofToString(int(ofRandom(0,9.999))) +
                                 ofToString(int(ofRandom(0,9.999))) +
                                 ofToString(int(ofRandom(0,9.999))) +
                                 ofToString(int(ofRandom(0,9.999)));

        tcpClient.send("RECORD: " + randomVisitorID + ", " + ofToString(visitorHeight) + ", " + ofToString(visitorWeight));
    }

    if(sendStatusButton) {
		sendStatusButton = false;
        tcpClient.send("STATUS");
    }

    if(sendSetInactivityButton) {
		sendSetInactivityButton = false;
        tcpClient.send("SET INACTIVITY 60");
    }

	ofBackground(230, 230, 230);

	//we are connected - lets send our text and check what we get back
	if(weConnected){//}&& msgTx.at(msgTx.length()) == ';'){
        //cout << "found a semicolon in msg i'm sending server" << endl;
		if(tcpClient.send(msgTx)){
            msgTx = "";

			//if data has been sent lets update our text
			string str = tcpClient.receive();
			if( str.length() > 0 ){
				msgRx = str;
                //cout << "updated msgRx from server: " << msgRx << endl;
			}
		}else if(!tcpClient.isConnected()){
			weConnected = false;
		}
	}else{
		//if we are not connected lets try and reconnect every 5 seconds
		deltaTime = ofGetElapsedTimeMillis() - connectTime;

		if( deltaTime > 5000 ){
			weConnected = tcpClient.setup("127.0.0.1", 11999);
			connectTime = ofGetElapsedTimeMillis();
		}

	}
}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetColor(255);
    HHLlogo.draw( ofGetWindowWidth() - HHLlogo.getWidth() - 20, ofGetWindowHeight() - HHLlogo.getHeight() - 20 );

	ofSetColor(20, 20, 20);
	ofDrawBitmapString("openFrameworks TCP Send Example", 15, 30+101);

	if(typed){
		ofDrawBitmapString("sending:", 15, 55+101);
		ofDrawBitmapString(msgTx, 85, 55+101);
	}
	else{
		if(weConnected)ofDrawBitmapString("status: type something to send data to port 11999", 15, 55+101);
		else ofDrawBitmapString("status: server not found. launch server app and check ports!\n\nreconnecting in "+ofToString( (5000 - deltaTime) / 1000 )+" seconds", 15, 55+101);
	}

	ofDrawBitmapString("from server: \n"+msgRx, 15, 130+101);

	gui.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	//you can only type if you're connected
	if(weConnected){
		if(key == 13)key = '\n'; // CR
		if(key == 8 || key == 127){ // BS or DEL
			if( pos != 0 ){pos--;
				msgTx = msgTx.substr(0,pos);
			}else msgTx = "";
		}else{
			msgTx.append(1, (char) key);
			pos++;
		}
		typed = true;
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
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

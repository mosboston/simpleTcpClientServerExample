#include "testApp.h"
#include "ofxSimpleGuiToo.h"

#define RECONNECT_TIME 400

bool	sendHelloButton = false;
bool	sendNewVisitorButton = false;
bool	sendStatusButton = false;


//--------------------------------------------------------------
void testApp::setup(){
	// we don't want to be running to fast
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
	weConnected = tcpClient.setup("192.168.1.108", 11999);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
	tcpClient.setMessageDelimiter("\n");
	//tcpClient.setMessageDelimiter(";");

	connectTime = 0;
	deltaTime = 0;

	tcpClient.setVerbose(true);


	gui.addButton("Send Hello", sendHelloButton);
	gui.addButton("Send NewVisitor", sendNewVisitorButton);
	gui.addButton("Send Status", sendStatusButton);

	gui.loadFromXML();

    gui.setAlignRight(true);

	gui.show();

}

//--------------------------------------------------------------
void testApp::update(){

    if(sendHelloButton) {
		sendHelloButton = false;
        tcpClient.send("hello");
    }

    if(sendNewVisitorButton) {
		sendNewVisitorButton = false;
        tcpClient.send("new visitor");
    }

    if(sendStatusButton) {
		sendStatusButton = false;
        tcpClient.send("status");
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

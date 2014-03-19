#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

    printer.open("/dev/tty.PL2303-00002014");
    
//    printer.println("Hello World!");
    
    printer.setReverse(true);
    printer.println("Reverse ON");
    printer.setReverse(false);
    
//    printer.setBold(true);
//    printer.println("Bold ON");
//    printer.setBold(false);

//    printer.setUpDown(true);
//    printer.println("UpDown ON");
//    printer.setUpDown(false);

//    printer.setUnderline(true);
//    printer.println("Underline ON");
//    printer.setUnderline(false);

//    printer.setDoubleWidth(true);
//    printer.println("Double Width ON");
//    printer.setDoubleWidth(false);
    
//    printer.setAlign(MIDDLE);
//    printer.println("middle align");
//    printer.setAlign(RIGHT);
//    printer.println("right align");
//    printer.setAlign(LEFT);
    
//    printer.printBarcode("12345678", EAN8);
    
    img.loadImage("logo.jpg");
    img.resize(img.getWidth()*2,img.getHeight()*2);
    cout << "Image at " << img.getWidth() << "x" << img.getHeight() << endl;
    
    video.initGrabber(640, 480);
}

//--------------------------------------------------------------
void ofApp::update(){
	video.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    img.draw(0, 0);
    video.draw(img.getWidth(),0);
    
}

void ofApp::exit(){
    printer.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	if(key == ' '){
        printer.printDitherImage(img,80);
    } else if (key == 't'){
        printer.println("A little tree");
    } else if (key == 'v'){
        printer.printDitherImage(video,180);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	
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
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}


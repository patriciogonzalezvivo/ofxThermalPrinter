#include "ofxThermalPrinter.h"

ofxThermalPrinter::ofxThermalPrinter(){
    bConnected = false;
    bPrinting = false;
}

bool ofxThermalPrinter::open(const std::string& portName){
    try {
        port = SharedSerial(new serial::Serial( portName,
                                               BAUDRATE,
                                               serial::Timeout::simpleTimeout(1000),
                                               serial::eightbits,
                                               serial::parity_none,
                                               serial::stopbits_one,
                                               serial::flowcontrol_none ));
    }
    
    catch (const std::exception& exc){
        ofLogError("ofxThermalPrinter:: Fail to open") << exc.what();
        bConnected = false;
        return bConnected;
    }
    
    bConnected = true;
    usleep(50000);
    reset();
    usleep(50000);
    
    setControlParameter();
    setPrintDensity();
    setStatus(true);
    
    port->flushOutput();
    
    setReverse(true);
    println("Reverse ON");
    setReverse(false);
    
    return bConnected;
}

void ofxThermalPrinter::write(const uint8_t &_a){
    if(bConnected){
        port->write(&_a, 1);
        usleep(BYTE_TIME);
    }
}

void ofxThermalPrinter::write(const uint8_t &_a,const uint8_t &_b ){
    const uint8_t command[2] = { _a, _b };
    write(command, 2);
    usleep(BYTE_TIME*2);
}

void ofxThermalPrinter::write(const uint8_t &_a, const uint8_t &_b, const uint8_t &_c ){
    const uint8_t command[3] = { _a, _b, _c };
    write(command, 3);
    usleep(BYTE_TIME*3);
}

void ofxThermalPrinter::write(const uint8_t &_a, const uint8_t &_b, const uint8_t &_c, const uint8_t &_d){
    const uint8_t command[4] = { _a, _b, _c, _b };
    write(command, 4);
    usleep(BYTE_TIME*4);
}

void ofxThermalPrinter::write(const uint8_t *_array, int _size){
    if(bConnected){
        port->write(_array, _size);
        usleep(BYTE_TIME*_size);
    }
}

void ofxThermalPrinter::close(){
    if(bConnected){
        port->close();
    }
}

// reset the printer
void ofxThermalPrinter::reset() {
    write(27,'@');
}

// sets the printer online (true) or ofline (false)
void ofxThermalPrinter::setStatus(bool state) {
    write(27,61,state);
}

// set control parameters: heatingDots, heatingTime, heatingInterval
void ofxThermalPrinter::setControlParameter(uint8_t heatingDots, uint8_t heatingTime, uint8_t heatingInterval) {
    write(27,55);
    write(heatingDots);
    write(heatingTime);
    write(heatingInterval);
}

// set sleep Time in seconds, time after last print the printer should stay awake
void ofxThermalPrinter::setSleepTime(uint8_t seconds) {
    write(27, 56, seconds);
    write(0xFF);
}

// set double width mode: on=true, off=false
void ofxThermalPrinter::setDoubleWidth(bool state) {
    write(27, state?14:20);
}


// set the print density and break time
void ofxThermalPrinter::setPrintDensity(uint8_t printDensity, uint8_t printBreakTime) {
    write(18, 35, (printBreakTime << 5) | printDensity );
}

// set the used character set
void ofxThermalPrinter::setCharacterSet(CharacterSet set) {
    write(27, 82, set);
}

// set the used code table
void ofxThermalPrinter::setCodeTable(CodeTable table) {
    write(27, 116, table);
}

// feed single line
void ofxThermalPrinter::feed(void) {
    write(10);
}

// feed <<lines>> lines
void ofxThermalPrinter::feed(uint8_t lines) {
    write(27, 74, lines);
}

// set line spacing
void ofxThermalPrinter::setLineSpacing(uint8_t spacing) {
    write(27, 51, spacing);
}

// set Align Mode: LEFT, MIDDLE, RIGHT
void ofxThermalPrinter::setAlign(AlignMode align) {
    write(27, 97, align);
}

// set how many blanks should be kept on the left side
void ofxThermalPrinter::setLeftBlankCharNums(uint8_t space) {
    if (space >= 47) space = 47;
    write(27, 66, space);
}

// set Bold Mode: on=true, off=false
void ofxThermalPrinter::setBold(bool state) {
    write(27, 32, (uint8_t)state);
    write(27, 69, (uint8_t)state);
}

// set Reverse printing Mode
void ofxThermalPrinter::setReverse(bool state) {
    write(29, 66, (uint8_t)state);
}

// set Up/Down Mode
void ofxThermalPrinter::setUpDown(bool state) {
    write(27, 123, (uint8_t)state);
}

// set Underline printing
void ofxThermalPrinter::setUnderline(bool state) {
    write(27, 45, (uint8_t) state);
}

// enable / disable the key on the frontpanel
void ofxThermalPrinter::setKeyPanel(bool state) {
    write( 27, 99, 53, (uint8_t) state );
}

// where should a readable barcode code be printed
void ofxThermalPrinter::setBarcodePrintReadable(PrintReadable n) {
    write(29, 72, n);
}

// sets the height of the barcode in pixels
void ofxThermalPrinter::setBarcodeHeight(uint8_t height) {
    if (height <= 1) height = 1;
    write(29, 104, height);
}

// sets the barcode line widths (only 2 or 3)
void ofxThermalPrinter::setBarCodeWidth(uint8_t width) {
    if (width <= 2) width=2;
    else if (width >= 3) width=3;
    
    write(29, 119, width);
}

void ofxThermalPrinter::print(const std::string& text){
    if(bConnected){
        port->write(text);
        usleep(BYTE_TIME*text.size());
    }
}


void ofxThermalPrinter::println(const std::string& text){
    print(text+"\n");
}

// prints a barcode
void ofxThermalPrinter::printBarcode(const std::string &data, BarcodeType type) {
    if(bConnected){
        write(29, 107, type);
        port->write(data);
        usleep(BYTE_TIME*data.size());
        write(0);
    }
}

void ofxThermalPrinter::print(ofBaseHasPixels &_img, int _threshold){
    print(_img.getPixelsRef(),_threshold);
}

void ofxThermalPrinter::print(ofPixels &_pixels, int _threshold){
    ofPixels pixels = _pixels;
    
    int width = pixels.getWidth();
    int height = pixels.getHeight();
    
    int GrayArrayLength = width * height;
    unsigned char * GrayArray = new unsigned char[GrayArrayLength];
    memset(GrayArray,0,GrayArrayLength);
    
    for (int y = 0; y < height;y++) {
        vector<bool> data;
        for (int x = 0; x < width; x++) {
            int loc = y*width + x;
            
            int pixelCt = 0;
            float brightTot = 0;
            
            ofColor c = pixels.getColor(x, y);
            float brightTemp = c.getBrightness();
            
            // Brightness correction curve:
            brightTemp =  sqrt(255) * sqrt (brightTemp);
            if (brightTemp > 255) brightTemp = 255;
            if (brightTemp < 0) brightTemp = 0;
            
            int darkness = 255 - floor(brightTemp);
            
            int idx = y*width + x;
            darkness += GrayArray[idx];
            
            if( darkness >= _threshold){
                darkness -= _threshold;
                data.push_back(true);
            } else {
                data.push_back(false);
            }
            
            int darkn8 = round(darkness / 8);
            
            // Atkinson dithering algorithm:  http://verlagmartinkoch.at/software/dither/index.html
            // Distribute error as follows:
            //     [ ]  1/8  1/8
            //1/8  1/8  1/8
            //     1/8
            
            if ((idx + 1) < GrayArrayLength)
                GrayArray[idx + 1] += darkn8;
            if ((idx + 2) < GrayArrayLength)
                GrayArray[idx + 2] += darkn8;
            if ((idx + width - 1) < GrayArrayLength)
                GrayArray[idx + width - 1] += darkn8;
            if ((idx + width) < GrayArrayLength)
                GrayArray[idx + width] += darkn8;
            if ((idx + width + 1) < GrayArrayLength)
                GrayArray[idx + width + 1 ] += darkn8;
            if ((idx + 2 * width) < GrayArrayLength)
                GrayArray[idx + 2 * width] += darkn8;
        }
        addToBuffer(data);
    }
    
    delete []GrayArray;
}

void ofxThermalPrinter::addToBuffer(vector<bool> _vector){
    if(bConnected){
        if(isThreadRunning()){
            if(lock()){
                buffer.push_back(_vector);
                unlock();
            }
        } else {
            buffer.push_back(_vector);
            bPrinting = true;
            startThread();
        }
    }
}

void ofxThermalPrinter::threadedFunction(){
    if(bConnected){
        while(isThreadRunning()){
            if(buffer.size()>0){
                printPixelRow(buffer[0]);
                buffer.erase(buffer.begin());
            } else {
                stopThread();
                bPrinting = false;
            }
        }
    }
}

void ofxThermalPrinter::printPixelRow(vector<bool> _line){
    if(bConnected){
        int width = _line.size();
        if(width>384)
            width = 384;
        
        int rowBytes        = (width + 7) / 8;                 // Round up to next byte boundary
        int rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width
        
        uint8_t data[rowBytesClipped];
        memset(data,0x00,rowBytesClipped);
        
        for (int i = 0; i < width; i++) {
            uint8_t bit = 0x00;
            if (_line[i]){
                bit = 0x01;
            }
            data[i/8] += (bit&0x01)<<(7-i%8);
        }
        
        const uint8_t command[4] = {18, 42, 1, rowBytesClipped};
        port->write(command, 4);
        usleep(BYTE_TIME*4);
        
        port->write(data,rowBytesClipped);
        usleep(BYTE_TIME*rowBytesClipped);
    }
}




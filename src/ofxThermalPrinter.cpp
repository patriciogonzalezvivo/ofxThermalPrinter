#include "ofxThermalPrinter.h"

ofxThermalPrinter::ofxThermalPrinter(){
}

void ofxThermalPrinter::open(const std::string& portName){
    port = SharedSerial(new serial::Serial( portName,
                                            BAUDRATE,
                                            serial::Timeout::simpleTimeout(1000),
                                            serial::eightbits,
                                            serial::parity_none,
                                            serial::stopbits_one,
                                            serial::flowcontrol_none ));
    
//    port->setDTR(true);
//    port->setRTS(true);
    
    usleep(50000);
    
    reset();
    usleep(50000);
    
    setControlParameter();
    setPrintDensity();
    setStatus(true);
//    setSleepTime();
//    setCodeTable();
//    setCharacterSet();
//    setBarcodePrintReadable();
    
    port->flushOutput();
}

void ofxThermalPrinter::close(){
    port->close();
}


// reset the printer
void ofxThermalPrinter::reset() {
    const uint8_t command[2] = { 27, '@' };
    port->write(command, 2);
    usleep(BYTE_TIME*2);
}

// sets the printer online (true) or ofline (false)
void ofxThermalPrinter::setStatus(bool state) {
    const uint8_t command[3] = { 27, 61, state };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set control parameters: heatingDots, heatingTime, heatingInterval
void ofxThermalPrinter::setControlParameter(uint8_t heatingDots, uint8_t heatingTime, uint8_t heatingInterval) {
    const uint8_t command[5] = { 27, 55, heatingDots, heatingTime, heatingInterval};
    port->write(command, 5);
    usleep(BYTE_TIME*5);
}

// set sleep Time in seconds, time after last print the printer should stay awake
void ofxThermalPrinter::setSleepTime(uint8_t seconds) {
    const uint8_t command[3] = { 27, 56, seconds };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
    const uint8_t a = 0xFF;
    port->write(&a, 1);
    usleep(BYTE_TIME);
}

// set double width mode: on=true, off=false
void ofxThermalPrinter::setDoubleWidth(bool state) {
    const uint8_t command[2] = { 27, state?14:20 };
    port->write(command, 2);
    usleep(BYTE_TIME*2);
}


// set the print density and break time
void ofxThermalPrinter::setPrintDensity(uint8_t printDensity, uint8_t printBreakTime) {
    const uint8_t command[3] = { 18, 35, (printBreakTime << 5) | printDensity };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set the used character set
void ofxThermalPrinter::setCharacterSet(CharacterSet set) {
    const uint8_t command[3] = { 27, 82, set };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set the used code table
void ofxThermalPrinter::setCodeTable(CodeTable table) {
    const uint8_t command[3] = { 27, 116, table };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// feed single line
void ofxThermalPrinter::feed(void) {
    const uint8_t nl = 10;
    port->write(&nl, 1);
    usleep(BYTE_TIME);
}

// feed <<lines>> lines
void ofxThermalPrinter::feed(uint8_t lines) {
    const uint8_t command[3] = { 27, 74, lines };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set line spacing
void ofxThermalPrinter::setLineSpacing(uint8_t spacing) {
    const uint8_t command[3] = { 27, 51, spacing };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set Align Mode: LEFT, MIDDLE, RIGHT
void ofxThermalPrinter::setAlign(AlignMode align) {
    const uint8_t command[3] = { 27, 97, align };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set how many blanks should be kept on the left side
void ofxThermalPrinter::setLeftBlankCharNums(uint8_t space) {
    if (space >= 47) space = 47;
    
    const uint8_t command[3] = { 27, 66, space };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set Bold Mode: on=true, off=false
void ofxThermalPrinter::setBold(bool state) {
    const uint8_t command[6] = { 27, 32, (uint8_t) state, 27, 69, (uint8_t) state };
    port->write(command, 6);
    usleep(BYTE_TIME*6);
}

// set Reverse printing Mode
void ofxThermalPrinter::setReverse(bool state) {
    const uint8_t command[3] = { 29, 66, (uint8_t)state };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set Up/Down Mode
void ofxThermalPrinter::setUpDown(bool state) {
    const uint8_t command[3] = { 27, 123, (uint8_t) state };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// set Underline printing
void ofxThermalPrinter::setUnderline(bool state) {
    const uint8_t command[3] = { 27, 45, (uint8_t) state };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// enable / disable the key on the frontpanel
void ofxThermalPrinter::setKeyPanel(bool state) {
    const uint8_t command[4] = { 27, 99, 53, (uint8_t) state };
    port->write(command, 4);
    usleep(BYTE_TIME*4);
}

// where should a readable barcode code be printed
void ofxThermalPrinter::setBarcodePrintReadable(PrintReadable n) {
    const uint8_t command[3] = { 29, 72, n };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// sets the height of the barcode in pixels
void ofxThermalPrinter::setBarcodeHeight(uint8_t height) {
    if (height <= 1) height = 1;
    
    const uint8_t command[3] = { 29, 104, height };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

// sets the barcode line widths (only 2 or 3)
void ofxThermalPrinter::setBarCodeWidth(uint8_t width) {
    if (width <= 2) width=2;
    else if (width >= 3) width=3;
    
    const uint8_t command[3] = { 29, 119, width };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
}

void ofxThermalPrinter::print(const std::string& text){
//    for (int i = 0; i < text.size(); i++) {
//        const uint8_t c = text[i];
//        port->write(&c,1);
//        usleep(BYTE_TIME);
//    }
    port->write(text);
    usleep(BYTE_TIME*text.size());
}


void ofxThermalPrinter::println(const std::string& text){
    print(text+"\n");
}

// prints a barcode
void ofxThermalPrinter::printBarcode(const std::string &data, BarcodeType type) {
    const uint8_t command[3] = { 29, 107, type };
    port->write(command, 3);
    usleep(BYTE_TIME*3);
    port->write(data);
    usleep(BYTE_TIME*data.size());
    const uint8_t z = 0;
    port->write(&z,1);
    usleep(BYTE_TIME);
}

void ofxThermalPrinter::printDitherImage(ofBaseHasPixels &_img, int _threshold){
    ofPixels pixels = _img.getPixelsRef();
    
    if(_img.getPixelsRef().getWidth() >= 384){
        float w = 1.0;
        float h = pixels.getHeight()/pixels.getWidth();
        pixels.resize(384.0, h*384.0);
    } else {
        pixels = _img.getPixelsRef();
    }
    
    int width = pixels.getWidth();
    int height = pixels.getHeight();
    
    int GrayArrayLength = width * height;
    unsigned char * GrayArray = new unsigned char[GrayArrayLength];
    memset(GrayArray,0x00,GrayArrayLength);
    
    int rowBytes        = (width + 7) / 8;                  // Round up to next byte boundary
    int rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width
    
    int totalBytes = rowBytesClipped*height;
    uint8_t data[totalBytes];
    memset(data,0x00,totalBytes);
    
    for (int i = 0; i < GrayArrayLength; i++){
        GrayArray[i] = 0;
    }
    
    for (int x = 0; x < width;x++) {
        for (int y = 0; y < height; y++) {
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
            
            if(x<rowBytesClipped*8){
                
                uint8_t pixel;
                if( darkness >= _threshold){
                    darkness -= _threshold;
                    pixel = 0x01;
                } else {
                    pixel = 0x00;
                }
                
                data[y*rowBytesClipped+x/8] += (pixel&0x01)<<(7-x%8);
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
    }
    
    for (int y=0; y<height; y++) {
        const uint8_t command[4] = {18, 42, 1, rowBytesClipped};
        port->write(command, 4);
        usleep(BYTE_TIME*4);
        
        for (int x=0; x<rowBytesClipped; x++) {
            port->write(&data[y*rowBytesClipped+x],1);
            usleep(BYTE_TIME);
        }
    }
}

// print Image, threshold defines grayscale to black&withe threshold level
void ofxThermalPrinter::printThresholdImage(ofBaseHasPixels &_img, int threshold) {
    ofPixels pixels = _img.getPixelsRef();

    if(_img.getPixelsRef().getWidth() >= 384){
        float w = 1.0;
        float h = pixels.getHeight()/pixels.getWidth();
        pixels.resize(384.0, h*384.0);
    } else {
        pixels = _img.getPixelsRef();
    }

    int width = pixels.getWidth();
    int height = pixels.getHeight();
    
    int rowBytes        = (width + 7) / 8;                  // Round up to next byte boundary
    int rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width
    
    int totalBytes = rowBytesClipped*height;
    uint8_t data[totalBytes];
    memset(data,0x00,totalBytes);
    
    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            if(x<rowBytesClipped*8){
                uint8_t pixel;
                if(pixels.getColor(x, y).getBrightness()>threshold){
                    pixel = 0x00;
                } else {
                    pixel = 0x01;
                }
                data[y*rowBytesClipped+x/8] += (pixel&0x01)<<(7-x%8);
            }
        }
    }
    
    // split images with height > 255 into parts (from Adafruit)
//    for (int rowStart=0; rowStart<height; rowStart+=256) {
//        
//        int chunkHeight = height - rowStart;
//        if (chunkHeight > 255) chunkHeight = 255;
//        
//        const uint8_t command[4] = {18, 42, chunkHeight, rowBytesClipped};
//        port->write(command, 4);
//        usleep(BYTE_TIME*4);
//        
//        for (int i=0; i<(rowBytesClipped*chunkHeight); i++) {
//            port->write(&data[rowStart*rowBytesClipped+i],1);
//            usleep(BYTE_TIME);
//        }
//    }
    
    for (int y=0; y<height; y++) {
        const uint8_t command[4] = {18, 42, 1, rowBytesClipped};
        port->write(command, 4);
        usleep(BYTE_TIME*4);
        
        for (int x=0; x<rowBytesClipped; x++) {
            port->write(&data[y*rowBytesClipped+x],1);
            usleep(BYTE_TIME);
        }
    }
}


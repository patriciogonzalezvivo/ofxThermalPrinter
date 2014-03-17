#include "ofxThermalPrinter.h"

ofxThermalPrinter::ofxThermalPrinter(){
}

void ofxThermalPrinter::open(const std::string& portName){
    port = SharedSerial(new serial::Serial( portName,
                                            19200,
                                            serial::Timeout::simpleTimeout(1000),
                                            serial::eightbits,
                                            serial::parity_none,
                                            serial::stopbits_one,
                                            serial::flowcontrol_none ));
    
    port->setDTR(true);
    port->setRTS(true);
    
    usleep(10000);
    
    reset();
    usleep(10000);
    
    setStatus(true);
    setControlParameter();
    setPrintDensity();
    setSleepTime();
    setCodeTable();
    setCharacterSet();
    setBarcodePrintReadable();
}

void ofxThermalPrinter::close(){
    port->close();
}


// reset the printer
void ofxThermalPrinter::reset() {
    const uint8_t command[2] = { 27, '@' };
    port->write(command, 2);
    usleep(50000);
}

// sets the printer online (true) or ofline (false)
void ofxThermalPrinter::setStatus(bool state) {
    const uint8_t command[3] = { 27, 61, state };
    port->write(command, 3);
    usleep(50000);
}

// set control parameters: heatingDots, heatingTime, heatingInterval
void ofxThermalPrinter::setControlParameter(uint8_t heatingDots, uint8_t heatingTime, uint8_t heatingInterval) {
    const uint8_t command[5] = { 27, 55, heatingDots, heatingTime, heatingInterval};
    port->write(command, 5);
    usleep(50000);
}

// set sleep Time in seconds, time after last print the printer should stay awake
void ofxThermalPrinter::setSleepTime(uint8_t seconds) {
    const uint8_t command[3] = { 27, 56, seconds };
    port->write(command, 3);
    usleep(50000);
    const uint8_t a = 0xFF;
    port->write(&a, 1);
    usleep(50000);
}

// set double width mode: on=true, off=false
void ofxThermalPrinter::setDoubleWidth(bool state) {
    const uint8_t command[2] = { 27, state?14:20 };
    port->write(command, 2);
    usleep(50000);
}


// set the print density and break time
void ofxThermalPrinter::setPrintDensity(uint8_t printDensity, uint8_t printBreakTime) {
    const uint8_t command[3] = { 18, 35, (printBreakTime << 5) | printDensity };
    port->write(command, 3);
    usleep(50000);
}

// set the used character set
void ofxThermalPrinter::setCharacterSet(CharacterSet set) {
    const uint8_t command[3] = { 27, 82, set };
    port->write(command, 3);
    usleep(50000);
}

// set the used code table
void ofxThermalPrinter::setCodeTable(CodeTable table) {
    const uint8_t command[3] = { 27, 116, table };
    port->write(command, 3);
    usleep(50000);
}

// feed single line
void ofxThermalPrinter::feed(void) {
    const uint8_t nl = 10;
    port->write(&nl, 1);
    usleep(50000);
}

// feed <<lines>> lines
void ofxThermalPrinter::feed(uint8_t lines) {
    const uint8_t command[3] = { 27, 74, lines };
    port->write(command, 3);
    usleep(50000);
}

// set line spacing
void ofxThermalPrinter::setLineSpacing(uint8_t spacing) {
    const uint8_t command[3] = { 27, 51, spacing };
    port->write(command, 3);
    usleep(50000);
}

// set Align Mode: LEFT, MIDDLE, RIGHT
void ofxThermalPrinter::setAlign(AlignMode align) {
    const uint8_t command[3] = { 27, 97, align };
    port->write(command, 3);
    usleep(50000);
}

// set how many blanks should be kept on the left side
void ofxThermalPrinter::setLeftBlankCharNums(uint8_t space) {
    if (space >= 47) space = 47;
    
    const uint8_t command[3] = { 27, 66, space };
    port->write(command, 3);
    usleep(50000);
}

// set Bold Mode: on=true, off=false
void ofxThermalPrinter::setBold(bool state) {
    const uint8_t command[6] = { 27, 32, (uint8_t) state, 27, 69, (uint8_t) state };
    port->write(command, 6);
    usleep(50000);
}

// set Reverse printing Mode
void ofxThermalPrinter::setReverse(bool state) {
    const uint8_t command[3] = { 29, 66, (uint8_t)state };
    port->write(command, 3);
    usleep(50000);
}

// set Up/Down Mode
void ofxThermalPrinter::setUpDown(bool state) {
    const uint8_t command[3] = { 27, 123, (uint8_t) state };
    port->write(command, 3);
    usleep(50000);
}

// set Underline printing
void ofxThermalPrinter::setUnderline(bool state) {
    const uint8_t command[3] = { 27, 45, (uint8_t) state };
    port->write(command, 3);
    usleep(50000);
}

// enable / disable the key on the frontpanel
void ofxThermalPrinter::setKeyPanel(bool state) {
    const uint8_t command[4] = { 27, 99, 53, (uint8_t) state };
    port->write(command, 4);
    usleep(50000);
}

// where should a readable barcode code be printed
void ofxThermalPrinter::setBarcodePrintReadable(PrintReadable n) {
    const uint8_t command[3] = { 29, 72, n };
    port->write(command, 3);
    usleep(50000);
}

// sets the height of the barcode in pixels
void ofxThermalPrinter::setBarcodeHeight(uint8_t height) {
    if (height <= 1) height = 1;
    
    const uint8_t command[3] = { 29, 104, height };
    port->write(command, 3);
    usleep(50000);
}

// sets the barcode line widths (only 2 or 3)
void ofxThermalPrinter::setBarCodeWidth(uint8_t width) {
    if (width <= 2) width=2;
    else if (width >= 3) width=3;
    
    const uint8_t command[3] = { 29, 119, width };
    port->write(command, 3);
    usleep(50000);
}

void ofxThermalPrinter::print(const std::string& text){
    port->write(text);
    usleep(50000);
}


void ofxThermalPrinter::println(const std::string& text){
    port->write(text+"\n");
    usleep(50000);
}

// prints a barcode
void ofxThermalPrinter::printBarcode(const std::string &data, BarcodeType type) {
    const uint8_t command[3] = { 29, 107, type };
    port->write(command, 3);
    port->write(data);
    const uint8_t z = 0;
    port->write(&z,1);
    usleep(50000);
}

// print Image, threshold defines grayscale to black&withe threshold level
void ofxThermalPrinter::printImage(ofImage &img, uint8_t threshold) {
    
    ofPixels pixels = img.getPixelsRef();

    int width = pixels.getWidth()/8;
    int height = pixels.getHeight();

    uint8_t data[width*height];
    memset(data, 0, width*height);
    
    for (int y=0; y< pixels.getHeight(); y++) {
        for (int x=0; x< pixels.getWidth(); x++) {
            uint8_t pixel = pixels.getColor(x, y).getBrightness() > threshold ? 0:1;
            data[y*width+x/8] += (pixel&0x01)<<(7-x%8);
        }
    }
    
    // split images with height > 255 into parts (from Adafruit)
    for (int rowStart=0; rowStart<height; rowStart+=256) {
        
        int chunkHeight = height - rowStart;
        
        if (chunkHeight > 255)
            chunkHeight = 255;
        
        const uint8_t command[4] = {18, 42, chunkHeight, width};
        port->write(command, 4);
        usleep(10000);
        
        for (int i=0; i<(width*chunkHeight); i++) {
            port->write(&data[rowStart*width + i],1);
            usleep(500);
        }
        
        usleep(50000);
    }
}


//
//  ofxThermalPrinter.h
//
//  Created by Patricio Gonzalez Vivo on 3/16/14.
//
//  References from:
//
//      -   Adafruit Arduino Driver https://github.com/adafruit/Adafruit-Thermal-Printer-Library
//      -   Python Implementation https://github.com/luopio/py-thermal-printer
//      -   QT C++ https://github.com/thefloe1/Raspi-Thermal-Printer
//      -   Christopher Baker's OpenFrameworks Addon https://github.com/bakercp/ofxESCPOS
//

#pragma once

#include "ofMain.h"
#include "serial.h"

// Though most of these printers are factory configured for 19200 baud
// operation, a few rare specimens instead work at 9600.  If so, change
// this constant.  This will NOT make printing slower!  The physical
// print and feed mechanisms are the limiting factor, not the port speed.
#define BAUDRATE  19200

// Number of microseconds to issue one byte to the printer.  11 bits
// (not 8) to accommodate idle, start and stop bits.  Idle time might
// be unnecessary, but erring on side of caution here.
#define BYTE_TIME (11L * 1000000L / BAUDRATE)

enum CharacterSet {
    USA              = 0,
    FRANCE           = 1,
    GERMANY          = 2,
    UNITED_KINGDOM   = 3,
    DENMARK_I        = 4,
    SWEDEN           = 5,
    ITALY            = 6,
    SPAIN_I          = 7,
    JAPAN            = 8,
    NORWAY           = 9,
    DENMARK_II       = 10,
    SPAIN_II         = 11,
    LATIN_AMERICA    = 12,
    KOREA            = 13, // Many printers stop here
    SLOVENIA_CROATIA = 14,
    CHINA            = 15,
    VIETNAM          = 16,
    ARABIA           = 17
};

enum CodeTable {
    PC437,
    PC850
};

enum AlignMode {
    LEFT,
    MIDDLE,
    RIGHT
};

enum PrintReadable {
    NONE,
    ABOVE,
    BELOW,
    BOTH
};

enum BarcodeType {
    UPCA,
    UPCE,
    EAN13,
    EAN8,
    CODE39,
    I25,
    CODEBAR,
    CODE93,
    CODE128,
    CODE11,
    MSI
};

typedef std::shared_ptr<serial::Serial> SharedSerial;

struct PixelsLine{
    bool *data;
    int size;
};

class ofxThermalPrinter : public ofThread {
public:
    ofxThermalPrinter();

    void    open(const std::string& portName);
    void    close();
    void    reset();
    
    void    setControlParameter(uint8_t heatingDots=20, uint8_t heatingTime=200, uint8_t heatingInterval=250);
    void    setSleepTime(uint8_t seconds = 0);
    void    setStatus(bool state=true);
    void    setPrintDensity(uint8_t printDensity=14, uint8_t printBreakTime=4);
    
    void    setDoubleWidth(bool state=false);
    void    setBold(bool state=false);
    void    setReverse(bool state=false);
    void    setUpDown(bool state=false);
    void    setUnderline(bool state=false);
    void    setKeyPanel(bool state=false);
    
    void    setCharacterSet(CharacterSet set=USA);
    void    setCodeTable(CodeTable table=PC437);
    void    feed();
    void    feed(uint8_t lines);
    void    setLineSpacing(uint8_t spacing=32);
    void    setAlign(AlignMode align=LEFT);
    void    setLeftBlankCharNums(uint8_t space=0);
    
    void    setBarcodePrintReadable(PrintReadable n=BELOW);
    void    setBarcodeHeight(uint8_t height=50);
    void    setBarCodeWidth(uint8_t width=3);
    
    void    print(const std::string &_line);
    void    println(const std::string &_line);
    void    printBarcode(const std::string &data, BarcodeType type=UPCA);
    void    printDitherImage(ofBaseHasPixels &_img, int threshold);
    void    printThresholdImage(ofBaseHasPixels &_img, int threshold);

private:
    void    write(const uint8_t &_a);
    void    write(const uint8_t &_a, const uint8_t &_b );
    void    write(const uint8_t &_a, const uint8_t &_b, const uint8_t &_c );
    void    write(const uint8_t &_a, const uint8_t &_b, const uint8_t &_c, const uint8_t &_d);
    void    write(const uint8_t *_array, int _size);
    
    void    writeBytesRow(const uint8_t *_array, int _width);
    void    writeBytesArray( vector<bool> _line );
    
    void    addToBuffer(vector<bool> _line);
    void    threadedFunction();
    
    vector< vector<bool> > buffer;
    
    SharedSerial    port;
};
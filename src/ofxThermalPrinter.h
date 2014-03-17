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


class ofxThermalPrinter {
public:
    ofxThermalPrinter();

    void    open(const std::string& portName);
    void    close();
    void    reset();
    
    void    setControlParameter(uint8_t heatingDots=20, uint8_t heatingTime=255, uint8_t heatingInterval=250);
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
    
    void    printImage(ofImage &img, uint8_t threshold=127);
    
private:
    typedef std::shared_ptr<serial::Serial> SharedSerial;

    SharedSerial    port;
};
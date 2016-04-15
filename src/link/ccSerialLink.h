//
//  ccSerialLink.h
//  emptyExample
//
//  Created by Sosolimited on 8/11/15.
//
//

#pragma once

#include "ClosedCaptionConversions.hpp"
#include <string>

#ifdef CINDER_CINDER
#include "asio/asio.hpp"
#include "cinder/Signals.h"
#else
// Using non-Boost ASIO
#define ASIO_STANDALONE 1
#include "asio.hpp"
#endif

namespace soso {

class ccSerialLink : public std::enable_shared_from_this<ccSerialLink> {
public:
    ccSerialLink(asio::io_service &iService, const std::string &iComPort, int iBaudRate = 9600);
    ~ccSerialLink() = default;

    enum Instruction {
        Header = 0x01,
        About = 0x3F, // Get CC box version, vendor information
        Clear = 0x40,
        // Recover roughly means decode closed captioning from.
        Recover_CC1 = 0x41,
        Recover_CC2 = 0x42,
        Recover_T1 = 0x43,
        Recover_T2 = 0x44,
        Recover_CC3 = 0x45,
        Recover_CC4 = 0x46,
        Recover_T3 = 0x47,
        Recover_T4 = 0x48,
        Recover_XDS = 0x49,
        Recover_Field_1 = 0x4A,
        Recover_Field_2 = 0x4B,
        Tokenize_Control_Codes = 0x4C,
        Disable_Control_Codes = 0x4D,
        Reset = 0x52,
        Test_Message_Reset = 0x54
        // Some instructions have been omitted
        // For full listing, see PDR-870 Manual
    };

    void sendInstruction(Instruction instruction);

    void addNewCharHandler(const std::function<void(std::string)> &iFn);
    void addNewCharPairHandler(const std::function<void(std::string, std::string)> &iFn);

    void addSetupHandler(const std::function<void()> &iFn);
    void addSerialIdleHandler(const std::function<void()> &iFn);
    void addSerialClosedHandler(const std::function<void()> &iFn);

    void setSerialIdleTimeout(float iSeconds) { serialTimeout = iSeconds; }
    bool                            getIsSerialConnected();
    float                           getSecondsSinceDataReceived() { return serialTimer; }
    /// Returns the signal emitted when a new word is found.
    auto &getSignalNewWord() { return _signal_new_word; }
    void  update();

    void enableSpecialChars(bool enable) { specialCharsEnabled = enable; }
private:
    bool isDoubleCharHeader(char iCode);
    bool isSpecialCharHeader(char iCode);

    void handleNewRawChar(char iNewChar);
    void fillCharBuffer(const std::string &to_send);
    void handleNewCleanChar(const std::string &str);

    void callSetupHandlers();
    void callNewCharHandlers(std::string iNewChar);
    void callNewCharPairHandlers(std::string iChar1, std::string iChar2);
    void callSerialIdleHandlers();
    void callSerialClosedHandlers();

    void listenForSerialData();
    void setupPDR();
    void testPDR();

    void updateMessageQueue(float iDt);

    std::vector<unsigned char>         messageQueue;
    std::string                        comPort = "/dev/tty.usbserial-AL00APKE"; // default. should change.
    std::shared_ptr<asio::serial_port> serial;

    bool isPDRSetup = false;

    asio::io_service &appIOService;

    std::vector<const std::function<void()>>            setupHandlers;
    std::vector<const std::function<void(std::string)>> newCharHandlers;
    std::vector<const std::function<void(std::string, std::string)>> newCharPairHandlers;
    std::vector<const std::function<void()>> serialIdleHandlers;
    std::vector<const std::function<void()>> serialClosedHandlers;

    ci::signals::Signal<void(const std::string &)> _signal_new_word;

    bool calledSetup = false;

    // Serial information
    int baudRate = 9600;

    // Timer information
    float currTime = 0;
    float serialTimer = 0;
    float serialTimeout = 30.0f; // In seconds

    // Information for sending commands to CC box
    float messagePeriod = 0.25;
    float messageTimer = 0;

    std::array<char, 512> _serial_data;
    std::string _leftover_string_data;

    std::chrono::high_resolution_clock::time_point previousUpdateTime;

    bool doubleCharFlag = false;
    bool specialCharFlag = false;

    bool secondCharFlag = false;
    bool specialCharsEnabled = false;

    std::deque<std::string> charBuffer;
};

} // namespace soso

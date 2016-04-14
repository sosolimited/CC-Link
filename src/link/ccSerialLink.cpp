//
//  ccSerialLink.cpp
//  emptyExample
//
//  Created by Sosolimited on 8/11/15.
//
//  Uses asio::serial to open a serial report and receive data
//  byte by byte from CC box (Model Link Electronics PDR-870)
//  Will notify subsribers of important serial events

// https://www.gpo.gov/fdsys/pkg/CFR-2007-title47-vol1/pdf/CFR-2007-title47-vol1-sec15-119.pdf

#include "StringManipulation.h"
#include "ccSerialLink.h"
#include "cinder/Utilities.h"
#include <iostream>

using namespace soso;
using namespace asio;
using namespace std;

ccSerialLink::ccSerialLink(io_service &iService, const string &iComPort, int iBaudRate)
    : appIOService(iService),
      comPort(iComPort),
      baudRate(iBaudRate)
{
    serial = make_shared<serial_port>(appIOService, comPort);
    serial->set_option(serial_port_base::baud_rate(baudRate));

    addNewCharHandler([this](const std::string &str) {
        handleNewCleanChar(str);
    });

    if (serial->is_open()) {
        // Flush serial port's input and output data
        // Gets rid of any data left over in serial buffer from previous sessions.
        ::tcflush(serial->lowest_layer().native_handle(), TCIOFLUSH);
    }
}

void ccSerialLink::sendInstruction(Instruction instruction)
{
    messageQueue.push_back(Instruction::Header);
    messageQueue.push_back(instruction);
}

void ccSerialLink::addNewCharHandler(const std::function<void(std::string)> &iFn)
{
    newCharHandlers.push_back(iFn);
}

void ccSerialLink::addNewCharPairHandler(const std::function<void(std::string, std::string)> &iFn)
{
    newCharPairHandlers.push_back(iFn);
}

void ccSerialLink::addSetupHandler(const std::function<void()> &iFn)
{
    setupHandlers.push_back(iFn);
}

void ccSerialLink::addSerialIdleHandler(const std::function<void()> &iFn)
{
    serialIdleHandlers.push_back(iFn);
}

void ccSerialLink::addSerialClosedHandler(const std::function<void()> &iFn)
{
    serialClosedHandlers.push_back(iFn);
}

void ccSerialLink::handleNewRawChar(char newChar)
{
    string to_send = "";

    // Did the previous byte indicate a double char?
    if (specialCharFlag) {
        // check if it's a special char.
        if (specialCharsEnabled) {
            to_send = special_closed_caption_to_string(newChar);
        }
        else {
            to_send = " ";
        }
    }
    else if (doubleCharFlag) {
        to_send = " ";
    }
    else if (isDoubleCharHeader(newChar)) {
        specialCharFlag = isSpecialCharHeader(newChar);
        doubleCharFlag = true;
        return;
    }
    else {
        to_send = closed_caption_to_string(newChar);
    }

    bool doubleSpace = false;

    if (to_send == " ") {
        if (charBuffer.size() > 1) {
            if (charBuffer.back() == " ") {
                doubleSpace = true;
            }
        }
    }

    if (!doubleSpace) {
        secondCharFlag = !secondCharFlag;
        charBuffer.push_back(to_send);
        callNewCharHandlers(to_send);

        if (charBuffer.size() > 2) {
            charBuffer.pop_front();
        }

        // Call char handler for 2 chars
        if (secondCharFlag) {
            callNewCharPairHandlers(charBuffer.front(), charBuffer.back());
        }
    }
    doubleCharFlag = false;
    specialCharFlag = false;
}

void ccSerialLink::handleNewCleanChar(const std::string &str)
{
    auto string_data = _leftover_string_data + str;
    auto separators = " \b\r\n\t\v\f\a.,;:…\0[](){}<>!?\"";
    auto tokens = ci::split(string_data, separators);

    for (auto i = 0; i < tokens.size() - 1; i += 1) {
        auto &w = tokens.at(i);
        if (!w.empty()) {
            _signal_new_word.emit(trim_unmatched_brackets(strip_punctuation(w)));
        }
    }
    _leftover_string_data = tokens.back();
}

void ccSerialLink::callSetupHandlers()
{
    for (auto f : setupHandlers) {
        appIOService.post(f);
    }
}

void ccSerialLink::callNewCharHandlers(std::string iNewChar)
{
    for (auto f : newCharHandlers) {
        auto boundFunction = [f, iNewChar]() {

            f(iNewChar);

        };

        appIOService.post(boundFunction);
    }
}

void ccSerialLink::callNewCharPairHandlers(std::string iChar1, std::string iChar2)
{
    for (auto f : newCharPairHandlers) {
        f(iChar1, iChar2);
    }
}

void ccSerialLink::callSerialIdleHandlers()
{
    for (auto f : serialIdleHandlers) {
        appIOService.post(f);
    }
}

void ccSerialLink::callSerialClosedHandlers()
{
    for (auto f : serialClosedHandlers) {
        appIOService.post(f);
    }
}

bool ccSerialLink::isSpecialCharHeader(char iCode)
{
    if ((iCode == 0x11) || (iCode == 0x19)) {
        return true;
    }
    return false;
}

// Wait for data to be receievd via serial port
void ccSerialLink::listenForSerialData()
{
    // if we read 1 byte at a time, seems too slow.  we miss data
    serial->async_read_some(asio::buffer(_serial_data), [this](std::error_code ec, std::size_t bytes_read) {

        if (!ec) {
            for (auto i = 0; i < bytes_read; i += 1) {
                //				callNewCharHandlers( _serial_data.at(i) );
                handleNewRawChar(_serial_data.at(i));
            }

            // Reset serial timer
            this->serialTimer = 0;

            // If we receive serial data, our connection is properly set up
            if (!isPDRSetup) {
                isPDRSetup = true;
            }
        }
        else {
            cout << "Error receiving serial data " << ec.message() << endl;
        }

        // Keep listening for serial data
        this->listenForSerialData();
    });
}

void ccSerialLink::testPDR()
{
    cout << "Testing PDR-870" << endl;
    sendInstruction(Instruction::About);
}

void ccSerialLink::setupPDR()
{
    cout << "Setup PDR-870" << endl;

    sendInstruction(Instruction::Clear);
    sendInstruction(Instruction::Recover_CC1);

    //	sendInstruction( Instruction::Disable_Control_Codes );
    //	sendInstruction( Instruction::Tokenize_Control_Codes );

    isPDRSetup = true;
}

void ccSerialLink::updateMessageQueue(float iDt)
{
    messageTimer += iDt;

    if (messageTimer > messagePeriod) {
        messageTimer = 0;
        if (messageQueue.size() > 0) {
            unsigned char c = messageQueue.at(0);

            // Write a message queue.
            std::vector<asio::const_buffer> buffers;
            buffers.push_back(asio::const_buffer(&c, 1));

            serial->write_some(buffers);

            messageQueue.erase(messageQueue.begin());
        }
    }
}

void ccSerialLink::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - previousUpdateTime).count();
    previousUpdateTime = now;

    auto dt = millis / 1000.0f;

    currTime += dt;
    serialTimer += dt;

    if (!serial->is_open()) {
    }

    if (serialTimer > serialTimeout) {
        cout << "Attempting to reconnect." << endl;

        // Attempt to reconnect
        if (messageQueue.empty()) {
            // Call serial timeout
            callSerialIdleHandlers();

            setupPDR();

            // Reset timer
            serialTimer = 0;
        }
    }

    // Don't call setupPDR if we're in the middle of
    // sending setup messages
    if (!isPDRSetup && messageQueue.empty()) {
        printf("\nInitial Setup of PDR-870 \n");
        setupPDR();
    }

    // Send messages in the queue
    updateMessageQueue(dt);

    if (isPDRSetup && messageQueue.empty()) {
        if (!calledSetup) {
            callSetupHandlers();
            listenForSerialData();
            calledSetup = true;
        }
    }
}

bool ccSerialLink::getIsSerialConnected()
{
    return serial->is_open();
}

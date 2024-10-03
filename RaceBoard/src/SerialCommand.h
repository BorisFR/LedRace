#ifndef SerialCommand_h
#define SerialCommand_h

#include <Arduino.h>

class SerialCommand
{

public:
    SerialCommand();
    SerialCommand(char *buf, int bufLen, char eoc, Stream *stream, Stream *stream2);
    void init(char *buf, int bufLen, char eoc, Stream *stream);
    int checkSerial(void);
    void sendCommand(char *);

    int overflow;

protected:
    Stream *_stream;
    Stream *_stream2;
    char *_buf;
    int _bufIdx;
    int _bufLen;
    char _eoc; // EndOfCommand char

    bool _initialized;
};

#endif

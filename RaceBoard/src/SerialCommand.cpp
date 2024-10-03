#include "SerialCommand.h"

/*
 *
 */
SerialCommand::SerialCommand()
{
    _initialized = false;
}
/*
 *
 */
SerialCommand::SerialCommand(char *buf, int blen, char eoc, Stream *stream, Stream *stream2)
{
    init(buf, blen, eoc, stream);
    _stream2 = stream2 == NULL ? &Serial1 : stream2;
}

/*
 *
 */
void SerialCommand::init(char *buf, int blen, char eoc, Stream *stream)
{
    if (_initialized)
        return;

    _stream = stream == NULL ? &Serial : stream;
    _buf = buf;
    _bufLen = blen;
    _bufIdx = 0;
    _eoc = eoc;
    _initialized = true;
}

/*
 *
 */
int SerialCommand::checkSerial()
{
    if (!_initialized)
        return (-1);

    while (_stream->available())
    {
        if (_bufIdx < _bufLen - 2)
        {
            char data = _stream->read();
            if (data == _eoc)
            {
                int cmsSize = _bufIdx;
                _buf[_bufIdx++] = '\0';
                _bufIdx = 0;
                return (cmsSize);
            }
            else
            {
                _buf[_bufIdx++] = data;
            }
        }
        else
        {
            // buffer full
            // reset and retunn error
            _buf[_bufIdx++] = '\0';
            _bufIdx = 0;
            return (-2);
        }
    }
    return (0);
}

void SerialCommand::sendCommand(char *str)
{
    // get command length
    int dlen = 0;
    for (; dlen < 80; dlen++)
    { // "dlen<80" to avoid infinite loop on malformed str without EOC
        if (*(str + dlen) == _eoc)
        {
            dlen++; // send EOC
            break;
        }
    }
    _stream->write(str, dlen);
    _stream2->write(str, dlen);
    return;
}

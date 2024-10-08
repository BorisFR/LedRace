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
SerialCommand::SerialCommand(char *buf, int blen, char eoc, Stream *stream)
{
    _stream = stream;
    init(buf, blen, eoc);
}

/*
 *
 */
void SerialCommand::init(char *buf, int blen, char eoc)
{
    if (_initialized)
        return;
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
    return;
}

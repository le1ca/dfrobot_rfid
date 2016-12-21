#ifndef RfidReader_h
#define RfidReader_h

#include "Arduino.h"

class RfidReader {
public:
    RfidReader(Stream& uart);
    bool readTagId(uint8_t *buf);

private:
    Stream& m_uart;
    uint8_t m_buf[17];
    uint8_t m_cnt;
    uint8_t m_idx;
    uint8_t m_cs;

};

#endif

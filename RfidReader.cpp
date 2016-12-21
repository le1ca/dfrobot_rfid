#include "RfidReader.h"

#define IDX(i) ((m_idx + i) % 17)

/* Constructor: Instantiates an RfidReader object on the given UART stream
 */
RfidReader::RfidReader(Stream& uart)
: m_uart(uart)
{  
}

/* readTagId(buf): places 12-byte tag ID in buf if
 * a tag is available. Returns true if a tag was
 * written; false otherwise.
 */
bool
RfidReader::readTagId(uint8_t *buf)
{
  
  // read all available input from uart
  while(m_uart.available()) {
    
    uint8_t x = m_uart.read();
    
    // if buffer is full, remove oldest byte from accumulated checksum
    if(m_cnt == 17) {
      m_cs = (m_cs - m_buf[m_idx]) & 0xff;
    }
    else {
      m_cnt++;
    }
    
    // write byte, increment index, and update checksum
    m_buf[m_idx] = x;
    m_idx = (m_idx + 1) % 17;
    m_cs = (m_cs + x) & 0xff;
    
  }

  // if the result is a valid tag, return and consume it
  // must have read 17 bytes, checksum must be valid, first two bytes must be 0x00, third-to-last byte must be 0x00, and last byte must be 0xff
  if(m_cnt == 17 && m_cs == 0xff && m_buf[IDX(0)] == 0x00 && m_buf[IDX(1)] == 0x00 && m_buf[IDX(14)] == 0x00 && m_buf[IDX(16)] == 0xff) {
    
     // write 12 bytes starting with the 3rd
     for(int i = 0; i < 12; i++) {
       buf[i] = m_buf[IDX(i + 2)];
     }
     
     // reset count to prevent same read from being returned again
     m_cnt = 0;
     m_cs  = 0;
     
     return true;
  }
  
  return false;
  
}

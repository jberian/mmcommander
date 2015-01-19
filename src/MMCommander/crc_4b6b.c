static short __xdata crc16Tab[256];

char crc8 (char *message, unsigned int nBytes)
{
  char remainder = 0;
  unsigned int byte;
  char bit;
  
  for (byte=0; byte < nBytes; ++byte)
  {
    remainder ^= message[byte];
    for (bit = 8; bit > 0; --bit)
    {
      if (remainder & 0x80)
        remainder = (remainder << 1) ^ 0x9B;
      else
        remainder = (remainder << 1);
    }
  }
  
  return(remainder);
}

void crc16Init (void)
{
  unsigned int i,j;
  unsigned short crc,c;
  
  for (i=0; i<256; i++) {
    crc = 0;
    c = ((unsigned short) i) << 8;
    for (j=0; j<8; j++) {
      if ( (crc^c) & 0x8000 ) crc = ( crc << 1 ) ^ 0x1021;
      else                    crc =   crc << 1;
      c = c << 1;
    }
    crc16Tab[i] = crc;
  }
}

short crc16 (char *message, unsigned int nBytes)
{
  unsigned int   i;
  unsigned short crc, short_c, tmp;
  
  crc = 0xffff;
  for (i=0; i<nBytes; i++) {
    short_c = 0x00ff & (unsigned short) message[i];
    tmp     = (crc >> 8) ^ short_c;
    crc     = (crc << 8) ^ crc16Tab[tmp];
  }
  
  return(crc);
}

char get4b6bsymbol (char inSymbol)
{
  switch (inSymbol) {
  case 0x00: return 0x15;
  case 0x01: return 0x31;
  case 0x02: return 0x32;
  case 0x03: return 0x23;
  case 0x04: return 0x34;
  case 0x05: return 0x25;
  case 0x06: return 0x26;
  case 0x07: return 0x16;
  case 0x08: return 0x1A;
  case 0x09: return 0x19;
  case 0x0A: return 0x2A;
  case 0x0B: return 0x0B;
  case 0x0C: return 0x2C;
  case 0x0D: return 0x0D;
  case 0x0E: return 0x0E;
  case 0x0F: return 0x1C;
  default:   return 0x00;
  }
}

char decode4b6bsymbol (char inSymbol, char *outSymbol)
{
  switch (inSymbol) {
  case 0x15: {*outSymbol = 0x00; return(0); }
  case 0x31: {*outSymbol = 0x01; return(0); }  
  case 0x32: {*outSymbol = 0x02; return(0); }
  case 0x23: {*outSymbol = 0x03; return(0); }
  case 0x34: {*outSymbol = 0x04; return(0); }
  case 0x25: {*outSymbol = 0x05; return(0); }
  case 0x26: {*outSymbol = 0x06; return(0); }
  case 0x16: {*outSymbol = 0x07; return(0); }
  case 0x1A: {*outSymbol = 0x08; return(0); }
  case 0x19: {*outSymbol = 0x09; return(0); }
  case 0x2A: {*outSymbol = 0x0A; return(0); }
  case 0x0B: {*outSymbol = 0x0B; return(0); }
  case 0x2C: {*outSymbol = 0x0C; return(0); }
  case 0x0D: {*outSymbol = 0x0D; return(0); }
  case 0x0E: {*outSymbol = 0x0E; return(0); }
  case 0x1C: {*outSymbol = 0x0F; return(0); }
  default:   {*outSymbol = 0x00; return(1); }
  }
}

void encode4b6b (char *messageIn, unsigned int bytesIn, char *messageOut, unsigned int *bytesOut)
{
  unsigned int i,j;
  int intBuffer;
  int mask;
  int intBitsAvailable;
  char symbol;
  
  *bytesOut = 0;
  intBitsAvailable = 0;
  intBuffer = 0;
  
  for (i=0; i<bytesIn*2; i++)
  {
    j = i>>1;
    if (j*2 == i) {
      symbol = (messageIn[j] >> 4 ) & 0x0F;
    } else {
      symbol = messageIn[j] & 0x0F;
    }
    symbol =  get4b6bsymbol(symbol);
    intBuffer = (intBuffer << 6) & 0xFFC0;
    intBuffer = intBuffer | symbol;
    intBitsAvailable += 6;
    if (intBitsAvailable >= 8) {
      symbol = intBuffer >> (intBitsAvailable - 8);
      messageOut[*bytesOut] = symbol;
      (*bytesOut)++;
      intBitsAvailable -= 8;
      mask = (1<<intBitsAvailable)-1;
      intBuffer = intBuffer & mask;
    }
  }
  
  if (intBitsAvailable != 0) {
    mask = (1<<intBitsAvailable)-1;
    intBuffer = intBuffer & mask;
    intBuffer = intBuffer << (8 - intBitsAvailable);
    symbol = intBuffer & 0x00FF;
    messageOut[*bytesOut] = symbol;
    (*bytesOut)++;
  }
  
}

void decode4b6b (char *messageIn, unsigned int bytesIn, char *messageOut, unsigned int *bytesOut)
{
  unsigned int i;
  int intBuffer;
  int intBitsAvailable;
  char symbol;
  char recByte, nibbleFlag;
  char outBytes;
  
  intBitsAvailable = 0;
  nibbleFlag = 0;
  outBytes = 0;
  for (i=0; i<bytesIn; i++) {
    intBuffer  = intBuffer << 8;
    intBuffer |= messageIn[i];
    intBitsAvailable += 8;
    while (intBitsAvailable >= 6) {
      symbol = intBuffer >> (intBitsAvailable-6);
      symbol &= 0x3F;
      decode4b6bsymbol (symbol, &symbol);
      recByte = recByte << 4;
      recByte &= 0xF0;
      recByte |= symbol;
      nibbleFlag ^= 1;
      if (nibbleFlag == 0) {
        messageOut[outBytes] = recByte;
        outBytes++;
      }
      intBitsAvailable -= 6;
    }
  }
  
  *bytesOut = outBytes;
}

void sleep (int time)
{
  int i,j;
  for (j=0; j<time*4; j++) for (i=0; i<0xFFFF; i++) asm("nop");
}
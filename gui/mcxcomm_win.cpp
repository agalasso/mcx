#include "wx/wxprec.h"
#ifndef WX_PRECOMP
# include "wx/wx.h"
#endif
#include "mcxcomm.h"

#define SERIALPORT_NAME_LEN 32
#define SERIALPORT_BUFSIZE 4096

enum Parity
{
    ParityNone,
    ParityOdd,
    ParityEven,
    ParityMark,
    ParitySpace
};

struct SerialPort_DCS
{
    int baud;
    Parity parity;
    unsigned char wordlen;
    unsigned char stopbits;
    bool rtscts;
    bool xonxoff;
    char buf[16];

    SerialPort_DCS() {
        baud = 38400;
        parity = ParityNone;
        wordlen = 8;
        stopbits = 1;
        rtscts = false;
        xonxoff = false;
    }

    const char *GetSettings() {
        const char ac[5] = {'N','O','E','M','S'};
        memset(buf,0,sizeof(buf));
        snprintf(buf,sizeof(buf)-1,"%i%c%i %i",
                 wordlen,
                 ac[parity],
                 stopbits,
                 baud);
        return buf;
    }
};

struct SerialPort
{
    SerialPort_DCS m_dcs;
    char m_devname[SERIALPORT_NAME_LEN];

    HANDLE fd;
    OVERLAPPED m_ov;

    int OpenDevice(const char *devname, const SerialPort_DCS& dcs);

public:

    enum FlowControl
    {
        NoFlowControl, /*!< No flow control at all */
        RtsCtsFlowControl, /*!< Enable RTS/CTS hardware flow control */
        XonXoffFlowControl /*!< Enable XON/XOFF protocol */
    };

    SerialPort();
    ~SerialPort();

    int Open( const char* portname, int baudrate,
              const char* protocol = "8N1",
              FlowControl flowControl = NoFlowControl );
//    int Open( int portnumber, int baudrate,
//              const char* protocol = "8N1",
//              FlowControl flowControl = NoFlowControl );

//    int Open(const char *devname, SerialPort_DCS *dcs = 0) {
//        return OpenDevice(devname, dcs);
//    };

    void Close();

    int Read(char* buf,size_t len);
    int Write(char* buf,size_t len);
};

SerialPort::SerialPort()
{
    m_devname[0] = '\0';
    memset( &m_ov, 0, sizeof( OVERLAPPED ) );
    fd = INVALID_HANDLE_VALUE;
}

SerialPort::~SerialPort()
{
    Close();
};

void
SerialPort::Close()
{
    if (fd != INVALID_HANDLE_VALUE) {
        CloseHandle(m_ov.hEvent);
        CloseHandle(fd);
        fd = INVALID_HANDLE_VALUE;
    }
}

int
SerialPort::Open(const char* portname, int baudrate,
                 const char* protocol,
                 FlowControl flowControl)
{
    SerialPort_DCS dcs;

    dcs.baud = baudrate;

    // default wordlen is 8
    if (( protocol[0] >= '5') && (protocol[0] <= '8'))
        dcs.wordlen = protocol[0] - '0';
    else
        return -1;

    // protocol is given as a string like "8N1", the first
    // character specifies the data bits (5...8), the second
    // the parity (None,Odd,Even,Mark,Space).
    // The third character defines the stopbit (1...2).
    switch( protocol[ 1 ] ) {
    case 'N': case 'n': dcs.parity = ParityNone; break;
    case 'O': case 'o': dcs.parity = ParityOdd; break;
    case 'E': case 'e': dcs.parity = ParityEven; break;
    case 'M': case 'm': dcs.parity = ParityMark; break;
    case 'S': case 's': dcs.parity = ParitySpace; break;
        // all other parameters cause an error!
    default: return -1;
    }
    // default stopbits is 1
    if ((protocol[2] >= '1') && (protocol[2] <= '2'))
        dcs.stopbits = protocol[2] - '0';
    else
        return -1;

    // default flow control is disabled
    dcs.rtscts = (flowControl == RtsCtsFlowControl);

    dcs.xonxoff = (flowControl == XonXoffFlowControl);

    return OpenDevice(portname, dcs);
}

int
SerialPort::OpenDevice(const char* devname, const SerialPort_DCS& dcs)
{
    m_dcs = dcs;

    fd = CreateFileA(devname,	// device name
                    GENERIC_READ | GENERIC_WRITE,	// O_RDWR
                    0,		// not shared
                    NULL,	// default value for object security ?!?
                    OPEN_EXISTING, // file (device) exists
                    FILE_FLAG_OVERLAPPED,	// asynchron handling
                    NULL); // no more handle flags

    if (fd == INVALID_HANDLE_VALUE) {
wxLogDebug("%s CreateFile(%s) returned invalid file handle",__FUNCTION__,devname);
        return -1;
    }

    // save the device name
    strncpy(m_devname,(char*)devname,sizeof(m_devname));
    // we write an eos to avoid a buffer overflow
    m_devname[sizeof(m_devname)-1] = '\0';

    // device control block
    DCB dcb;
    memset(&dcb,0,sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = m_dcs.baud;
    dcb.fBinary = 1;

    // Specifies whether the CTS (clear-to-send) signal is monitored 
    // for output flow control. If this member is TRUE and CTS is turned
    // off, output is suspended until CTS is sent again.
    dcb.fOutxCtsFlow = m_dcs.rtscts;

    // Specifies the DTR (data-terminal-ready) flow control. 
    // This member can be one of the following values:
    // DTR_CONTROL_DISABLE   Disables the DTR line when the device is 
    //                       opened and leaves it disabled. 
    // DTR_CONTROL_ENABLE    Enables the DTR line when the device is 
    //                       opened and leaves it on. 
    // DTR_CONTROL_HANDSHAKE Enables DTR handshaking. If handshaking is 
    //                       enabled, it is an error for the application
    //                       to adjust the line by using the 
    //                       EscapeCommFunction function.  
    dcb.fDtrControl = DTR_CONTROL_DISABLE;

    // Specifies the RTS flow control. If this value is zero, the
    // default is RTS_CONTROL_HANDSHAKE. This member can be one of
    // the following values:
    // RTS_CONTROL_DISABLE   Disables the RTS line when device is
    //                       opened and leaves it disabled.
    // RTS_CONTROL_ENABLED   Enables the RTS line when device is
    //                       opened and leaves it on.
    // RTS_CONTROL_HANDSHAKE Enables RTS handshaking. The driver
    //                       raises the RTS line when the
    //                       "type-ahead" (input)buffer is less than
    //                       one-half full and lowers the RTS line
    //                       when the buffer is more than three-quarters
    //                       full. If handshaking is enabled, it is an
    //                       error for the application to adjust the
    //                       line by using the EscapeCommFunction function.
    // RTS_CONTROL_TOGGLE    Specifies that the RTS line will be high if 
    //                       bytes are available for transmission. After
    //                       all buffered bytes have been send, the RTS
    //                       line will be low.
    if(m_dcs.rtscts) dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    else {
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
    }
    // Specifies the XON/XOFF flow control.
    // If fOutX is true (the default is false), transmission stops when the
    // XOFF character is received and starts again, when the XON character
    // is received.
    dcb.fOutX = m_dcs.xonxoff;
    // If fInX is true (default is false), the XOFF character is sent when
    // the input buffer comes within XoffLim bytes of being full, and the
    // XON character is sent, when the input buffer comes within XonLim
    // bytes of being empty.
    dcb.fInX = m_dcs.xonxoff;
    // default character for XOFF is 0x13 (hex 13)
    dcb.XoffChar = 0x13;
    // default character for XON is 0x11 (hex 11)
    dcb.XonChar = 0x11;
    // set the minimum number of bytes allowed in the input buffer before
    // the XON character is sent (3/4 of full size)
    dcb.XonLim = (SERIALPORT_BUFSIZE >> 2) * 3;
    // set the maximum number of free bytes in the input buffer, before the
    // XOFF character is sent (3/4 of full size)
    dcb.XoffLim = (SERIALPORT_BUFSIZE >> 2) * 3;

    // parity
    switch( m_dcs.parity ) {

    case ParityOdd: dcb.Parity = ODDPARITY; break;
    case ParityEven: dcb.Parity = EVENPARITY; break;
    case ParityMark: dcb.Parity = MARKPARITY; break;
    case ParitySpace: dcb.Parity = SPACEPARITY; break;
    default: dcb.Parity = NOPARITY;

    }

    // stopbits
    if(m_dcs.stopbits == 2) dcb.StopBits = TWOSTOPBITS;
    else dcb.StopBits = ONESTOPBIT;
    // wordlen, valid values are 5,6,7,8
    dcb.ByteSize = m_dcs.wordlen;

    if(!SetCommState(fd,&dcb))
        return -2;

    // create event for overlapped I/O
    // we need a event object, which inform us about the
    // end of an operation (here reading device)
    m_ov.hEvent = CreateEvent(NULL,// LPSECURITY_ATTRIBUTES lpsa
                              TRUE, // BOOL fManualReset 
                              TRUE, // BOOL fInitialState
                              NULL); // LPTSTR lpszEventName
    if(m_ov.hEvent == INVALID_HANDLE_VALUE) {
        return -3;
    }

    COMMTIMEOUTS cto = {MAXDWORD,0,0,0,0};
    if(!SetCommTimeouts(fd,&cto))
        return -5;

    // for a better performance with win95/98 I increased the internal
    // buffer to SERIALPORT_BUFSIZE (normal size is 1024, but this can 
    // be a little bit to small, if you use a higher baudrate like 115200)
    if(!SetupComm(fd,SERIALPORT_BUFSIZE,SERIALPORT_BUFSIZE))
        return -6;

    return 0;
}

int
SerialPort::Read(char* buf, size_t len)
{
    DWORD read;

    if (ReadFile(fd, buf, len, &read, &m_ov))
        return read;

    if (GetLastError() == ERROR_IO_PENDING)
        return 0;

    wxLogDebug("SerialPort read failed");
    return -1;
}

int
SerialPort::Write(char* buf,size_t len)
{
    DWORD write;
    if (!WriteFile(fd,buf,len,&write,&m_ov)) {
        if(GetLastError() != ERROR_IO_PENDING) {
            wxLogDebug("SerialPort write failed");
            return -1;
        }
        else {
            // VERY IMPORTANT to flush the data out of the internal
            // buffer
            FlushFileBuffers(fd);
            // first you must call GetOverlappedResult, then you
            // get the REALLY transmitted count of bytes
            if (!GetOverlappedResult(fd,&m_ov,&write,TRUE)) {
                // ooops... something is going wrong
                return write;
            }
        }
    }
    return write;
}

static SerialPort *s_dev;

bool
mcxcomm_init()
{
wxLogDebug("%s",__FUNCTION__);
    s_dev = new SerialPort();
    return true;
}

bool
mcxcomm_connect(const char *filename)
{
wxLogDebug("%s %p %s",__FUNCTION__,s_dev,filename);
    int ret = s_dev->Open(filename, 9600, "8N1", SerialPort::NoFlowControl);
    wxLogDebug("%s ret=%d",__FUNCTION__,ret);
    return ret == 0;
}

void
mcxcomm_disconnect()
{
wxLogDebug("%s",__FUNCTION__);
    s_dev->Close();
}

static bool
_send1(char val)
{
    wxLogDebug("%s 0x%x",__FUNCTION__,(unsigned int) val);
    int tries = 0;
    while (true) {
        int ret = s_dev->Write(&val, sizeof(val));
        if (ret == 1)
            return true;
        if (++tries >= 50)
            return false;
        wxMilliSleep(10);
    }
}

bool
mcxcomm_send_enq()
{
    return _send1(ENQ);
}

bool
mcxcomm_send_ack()
{
    return _send1(ACK);
}

bool
mcxcomm_send_msg(const msg& cmd)
{
    wxLogDebug("%s %02x %02x %02x %02x %02x %02x",__FUNCTION__,cmd.stx,cmd.cmdrsp,cmd.ctrl,cmd.data[0], cmd.data[1], cmd.data[2]);

    unsigned int tries = 0;
    const char *p = (const char *) &cmd;
    unsigned int rem = sizeof(cmd);
    while (rem > 0) {
        int n = s_dev->Write(const_cast<char *>(p), rem);
        if (n < 0) {
            wxLogDebug("SerialPort write failed");
            return false;
        }
        if (n == 0) {
            if (++tries >= 50) {
                wxLogDebug("SerialPort write timed-out");
                return false;
            }
            wxMilliSleep(10);
            continue;
        }
        p += n;
        rem -= n;
    }
    return true;
}

static bool
_read_n(void *buf, size_t n, const wxStopWatch& timer, unsigned int timeout_ms, bool *err)
{
    char *p = (char *) buf;
    unsigned int rem = (unsigned int) n;
    while (rem > 0) {
        int n = s_dev->Read(p, rem);
        if (n < 0) {
            *err = true;
            return false;
        }
        if (n == 0) {
            if (timer.Time() > timeout_ms) {
                *err = false;
                return false;
            }
            wxMilliSleep(10);
            continue;
        }
        p += n;
        rem -= n;
    }
    *err = false;
    return true;
}

bool
mcxcomm_recv(msg *msg, unsigned int timeout_ms, bool *err)
{
    wxStopWatch timer;

    if (!_read_n(&msg->stx, 1, timer, timeout_ms, err)) {
if (*err) wxLogDebug("%s fail reading 1 byte",__FUNCTION__);
        return false;
    }
//wxLogDebug("recv 0x%x",msg->stx);

    if (msg->stx == STX) {
        if (!_read_n(&msg->cmdrsp, sizeof(*msg) - 1, timer, timeout_ms, err))
            return false;
    }

    return true;
}

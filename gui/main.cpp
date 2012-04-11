// TODO: disable controls disable labels to make more obvious?
// TODO: top button functions
// TODO: svn
// TODO: camera comm for windows
// TODO: config window (comm port, anything else?)
// TODO: app icon
// TODO: resend mcx message if not acked?
// TODO: accept varios integration time formats  2m 2:30 2.5m
//
// TODO: fix toolbar background
// TODO: 3 minute agc change timer, + how to cancel it?
// TODO: integration fsm
// crash when integrating hit exit
// TODO: staus messages for reading camera state, maybe even a progress bar
//    general status
//         connecting, sending, integrating

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
# include "wx/wx.h"
#endif

#include "mcxgui.h"
#include "mcxcomm.h"

#include <map>

#define lengthof(a) (sizeof(a)/sizeof((a)[0]))

typedef unsigned char u8;
typedef unsigned int u32;

#define K1(a) ((u32)(a) << 24)
#define K2(a,b) (K1(a)|((u32)(b) << 16))
#define K3(a,b,c) (K2(a,b)|(u32)(c))

static void status(const wxString& str);

DECLARE_EVENT_TYPE(EVT_MCX_MSG, -1)
DEFINE_EVENT_TYPE(EVT_MCX_MSG)

class McxApp : public wxApp
{
  typedef wxApp inherited;
public:
  bool OnInit();
  int OnExit();
};
DECLARE_APP(McxApp)
IMPLEMENT_APP(McxApp)

enum {
  ID_DEFERRED_EVT_TIMER = 1,
  ID_FSM_TIMER,
  ID_INT_TIMER,
  ID_AGC_TIMER,
};

enum EnableType {
  EN_DISABLE,
  EN_DISABLE_FOR_INT,
  EN_ENABLE_ALL,
};

enum {
  DEFERRED_EVENT_INTERVAL = 1000, // milliseconds

  AGC_WAIT_MS = 3 * 60 * 1000, // how long to wait after agc change
};

enum CameraState {
  CAM_INIT,
  CAM_DISCONNECTED,
  CAM_CONNECTING,
  CAM_DISCOVER,
  CAM_DISCOVERING,
  CAM_READING1,
  CAM_READING2,
  CAM_SENDING1,
  CAM_SENDING2,
  CAM_UPTODATE,
  CAM_SHUTTING_DOWN,
};

enum IntState {
  INT_STOPPED,
  INT_INIT1,
  INT_INIT2,
  INT_INT1,
  INT_INT2,
  INT_CAPTURE1,
  INT_CAPTURE2,
  INT_STOP,
};

enum AgcState {
  AGC_STABLE,
  AGC_WAIT_INIT,
  AGC_WAIT1,
  AGC_WAIT2,
  AGC_CLEANUP,
};

typedef std::map<u32, msg> cmdmap_t;
static cmdmap_t s_cmdmap;
static msg s_active_cmd;
static CameraState s_camera_state = CAM_INIT;
static wxTimer *s_deferred_evt_timer;
static wxTimer *s_fsm_timer;
static bool s_fsm_timeout;
static wxThread *s_reader;
static int s_reader_connected = -1;
static int s_fsm_next_smry;
static bool s_fsm_got_ack;
static msg s_fsm_response;
static IntState s_int_state = INT_STOPPED;
static bool s_int_stop_clicked;
static wxStopWatch *s_int_stopwatch;
static wxTimer *s_int_timer;
static bool s_int_timer_expired;
static long s_int_time;
static AgcState s_agc_wait_state = AGC_STABLE;
static wxStopWatch *s_agc_stopwatch;
static wxTimer *s_agc_timer;
static bool s_agc_timer_expired;
static bool s_agc_wait_cancel_clicked;

static void _do_camera_fsm();
static void dnotify(int when);

enum UpdateWhen {
  UPD_IMMEDIATE,
  UPD_DEFER,
};

struct Config
{
  wxString cfg_serial_port;
};
static Config s_cfg;

// regs:
//   0x10,0: title on/off 0=off 1=on
//   0x10,1: title
//   0x10,3: title pos
//   0x11  : senseUp
//   0x12  : alc/elc
//   0x15  : alc
//   0x16  : elc
//   0x18,0: blc
//   0x18,1: blc preset
//   0x19  : blc area
//   0x22  : blc peak
//   0x1a,0: agc
//   0x1a,1: agc lvl
//   0x1a,2: agc man lvl
//   0x1b,0: wtb
//   0x1b,1: wtbMan
//   0x1b,2: wtbRed
//   0x1b,3: wtbBlue
//   0x1c,0: sync 0=int 1=line 2=vbs
//   0x1d,0: maskA
//   0x1d,1: maskB
//   0x1d,2: maskC
//   0x1d,3: maskD
//   0x1d,0x10: maskAArea
//   ...
//   0x1d,4: negative
//   0x1d,5: hrev
//   0x1d,7: vrev
//   0x1d,9: freeze fld=0 frame=1
//   0x1d,8: freeze 0=off 1=on
//   0x1d,6: priority
//   0x1d,0x14: gamma
//   0x1d,0x15,1: apch
//   0x1d,0x15,2: apcv
//   0x1d,0x16: coronagraph
//   0x1d,0x17: colorbar
//   0x47,0: tec on/off
//   0x47,2: tec lvl
//   0x47,3: dew removal
//   0x48: tecArea
//   0x1f,0: zoom on/off
//   0x1f,1: zoom level

struct Camera
{
  wxString title;
  u8 titlePos; // 0 = left-up 1=left-down 2=r-up 3=r-dn

  u8 senseUp;  // 0 = off, 1=2x, 4x, ... 0xc=128x

  u8 alcElc;  // 0=alc 1=elc
  u8 alc; // 0=off 1=1/100 .. 0xf=1/12000
  u8 elc; // 0=min .. 8=max

  u8 blc; // 0=off 1=on 2=peak
  u8 blcPreset; // 0=off 1=on [when blc=on]
  u8 blcArea[6];
  u8 blcPeak; // 0=min 8=max

  u8 agc; // 0=off 1=on 2=manual
  u8 agcLevel; // 0=min 8=max [when agc==on]
  u8 agcManual; // 0=min 8=max

  u8 wtb; // 0=atw 1=awc 2=man
  u8 wtbMan; // 0=3200 1=5600 2=user
  u8 wtbRed; // 0=min 8=max [when wtbMan == user]
  u8 wtbBlue; // 0=min 8=max [when wtbMan == user]

  u8 sync; // 0=int 1=line 2=vbs

  u8 mask; // 0=off 1=cross-hairs 2=cross-bars
  u8 neg; // 0=pos 1=neg
  u8 hRev; // 0=off 1=on
  u8 vRev; // 0=off 1=on

  u8 freezeMode; // 0=fld 1=frame
  u8 freeze; // 0=off 1=on
  u8 priority; // 0=agc 1=senseup
  u8 gamma; // 0=0.45 1=1.0

  u8 apcH; // 0..0x12
  u8 apcV; // 0..0x12

  u8 coronagraph; // 1..0x12   todo: 1-0x12 ???
  u8 colorBars; // 0=off 1=on

  u8 tec; // 0=off 1=on
  u8 tecLevel; // 0..8 [when tec==on]
  u8 dewRemoval; // 0=10s 1=30s 2=60s
  u8 tecArea[6]; // [for tec==on]

  u8 zoom; // 0=off 1=on
  u8 zoomLevel; // 0..8
};

static Camera s_cam0, s_cam1;

enum {
  SYNC_INT = 0,
  SYNC_LINE = 1,
  SYNC_VBS = 2,
};

static msg
cmda(u8 a, const u8 *b, size_t len)
{
  msg msg;
  mcxcmd_set(&msg, a, b, len);
wxPrintf("cmda a=%u b=[%u %u %u ...] l=%zu\n",(unsigned)a, (unsigned)b[0], (unsigned)b[1], (unsigned)b[2], len);
  return msg;
}

static msg
cmd(u8 a, u8 b, u8 c = 0, u8 d = 0)
{
  u8 data[] = { b, c, d };
  return cmda(a, data, sizeof(data));
}

static void
emit1(u8 a, u8 b)
{
  s_cmdmap[K1(a)] = cmd(a, b);
}

static void
emit1(u8 a, const u8 *b, size_t len)
{
  s_cmdmap[K1(a)] = cmda(a, b, len);
}

static void
emit2(u8 a, u8 b, u8 c)
{
  s_cmdmap[K2(a, b)] = cmd(a, b, c);
}

static void
emit2(u8 a, u8 b, const u8 *ary, size_t len)
{
  u8 buf[DATA_LEN];
  buf[0] = b;
  if (len > DATA_LEN - 1)
    len = DATA_LEN - 1;
  for (unsigned int i = 0; i < len; i++)
    buf[i + 1] = ary[i];
  s_cmdmap[K2(a, b)] = cmda(a, buf, len + 1);
}

static void
emit3(u8 a, u8 b, u8 c, u8 d)
{
  s_cmdmap[K3(a, b, c)] = cmd(a, b, c, d);
}

static void
_gen_title(u8 *buf, const char *s)
{
  u8 const INVAL = 0x3f;

  static u8 const map[] = {
    //       000   0     00    NUL                         
    INVAL,
    //       001   1     01    SOH                         
    INVAL,
    //       002   2     02    STX                         
    INVAL,
    //       003   3     03    ETX                         
    INVAL,
    //       004   4     04    EOT                         
    INVAL,
    //       005   5     05    ENQ                         
    INVAL,
    //       006   6     06    ACK                         
    INVAL,
    //       007   7     07    BEL                         
    INVAL,
    //       010   8     08    BS                          
    INVAL,
    //       011   9     09    HT
    INVAL,
    //       012   10    0A    LF  			     
    INVAL,
    //       013   11    0B    VT  			     
    INVAL,
    //       014   12    0C    FF  			     
    INVAL,
    //       015   13    0D    CR  			     
    INVAL,
    //       016   14    0E    SO  			     
    INVAL,
    //       017   15    0F    SI  			     
    INVAL,
    //       020   16    10    DLE 			     
    INVAL,
    //       021   17    11    DC1 			     
    INVAL,
    //       022   18    12    DC2 			     
    INVAL,
    //       023   19    13    DC3 			     
    INVAL,
    //       024   20    14    DC4 			     
    INVAL,
    //       025   21    15    NAK 			     
    INVAL,
    //       026   22    16    SYN 			     
    INVAL,
    //       027   23    17    ETB 			     
    INVAL,
    //       030   24    18    CAN 			     
    INVAL,
    //       031   25    19    EM  			     
    INVAL,
    //       032   26    1A    SUB 			     
    INVAL,
    //       033   27    1B    ESC 			     
    INVAL,
    //       034   28    1C    FS  		             
    INVAL,
    //       035   29    1D    GS  		             
    INVAL,
    //       036   30    1E    RS  		             
    INVAL,
    //       037   31    1F    US  		             
    INVAL,
    //       040   32    20    SPACE                       
    0x10,
    //       041   33    21    !                           
    INVAL,
    //       042   34    22    "                           
    INVAL,
    //       043   35    23    #                           
    INVAL,
    //       044   36    24    $                           
    INVAL,
    //       045   37    25    %                           
    INVAL,
    //       046   38    26    &                           
    INVAL,
    //       047   39    27    ´                           
    INVAL,
    //       050   40    28    (                           
    0x0b, // <
    //       051   41    29    )                           
    0x0c, // >
    //       052   42    2A    *                           
    INVAL,
    //       053   43    2B    +                           
    INVAL,
    //       054   44    2C    ,                           
    0x0f,
    //       055   45    2D    -                           
    0x0d,
    //       056   46    2E    .                           
    0x0e,
    //       057   47    2F    /                           
    0x6d,
    //       060   48    30    0                           
    0x00,
    //       061   49    31    1                           
    0x01,
    //       062   50    32    2                           
    0x02,
    //       063   51    33    3                           
    0x03,
    //       064   52    34    4                           
    0x04,
    //       065   53    35    5                           
    0x05,
    //       066   54    36    6                           
    0x06,
    //       067   55    37    7                           
    0x07,
    //       070   56    38    8                           
    0x08,
    //       071   57    39    9                           
    0x09,
    //       072   58    3A    :                           
    0x0a,
    //       073   59    3B    ;                           
    0x6b,
    //       074   60    3C    <                           
    0x0b,
    //       075   61    3D    =                           
    INVAL,
    //       076   62    3E    >                           
    0x0c,
    //       077   63    3F    ?                           
    0x50,
    //       100   64    40    @  
    INVAL,
    //       101   65    41    A  
    0x11,
    //       102   66    42    B  
    0x12,
    //       103   67    43    C  
    0x13,
    //       104   68    44    D  
    0x14,
    //       105   69    45    E  
    0x15,
    //       106   70    46    F  
    0x16,
    //       107   71    47    G  
    0x17,
    //       110   72    48    H  
    0x18,
    //       111   73    49    I  
    0x19,
    //       112   74    4A    J  
    0x1a,
    //       113   75    4B    K  
    0x1b,
    //       114   76    4C    L  
    0x1c,
    //       115   77    4D    M  
    0x1d,
    //       116   78    4E    N  
    0x1e,
    //       117   79    4F    O  
    0x00,
    //       120   80    50    P  
    0x20,
    //       121   81    51    Q  
    0x21,
    //       122   82    52    R  
    0x22,
    //       123   83    53    S  
    0x23,
    //       124   84    54    T  
    0x24,
    //       125   85    55    U  
    0x25,
    //       126   86    56    V  
    0x26,
    //       127   87    57    W  
    0x27,
    //       130   88    58    X  
    0x28,
    //       131   89    59    Y  
    0x29,
    //       132   90    5A    Z  
    0x2a,
    //       133   91    5B    [  
    INVAL,
    //       134   92    5C    \  
    INVAL,
    //       135   93    5D    ]  
    INVAL,
    //       136   94    5E    ^  
    INVAL,
    //       137   95    5F    _  
    INVAL,
    //       140   96    60    `  
    INVAL,
    //       141   97    61    a  
    0x51,
    //       142   98    62    b  
    0x52,
    //       143   99    63    c  
    0x53,
    //       144   100   64    d  
    0x54,
    //       145   101   65    e  
    0x55,
    //       146   102   66    f  
    0x56,
    //       147   103   67    g  
    0x57,
    //       150   104   68    h  
    0x58,
    //       151   105   69    i  
    0x59,
    //       152   106   6A    j  
    0x5a,
    //       153   107   6B    k  
    0x5b,
    //       154   108   6C    l  
    0x5c,
    //       155   109   6D    m  
    0x5d,
    //       156   110   6E    n  
    0x5e,
    //       157   111   6F    o  
    0x5f,
    //       160   112   70    p  
    0x60,
    //       161   113   71    q  
    0x61,
    //       162   114   72    r  
    0x62,
    //       163   115   73    s  
    0x63,
    //       164   116   74    t  
    0x64,
    //       165   117   75    u  
    0x65,
    //       166   118   76    v  
    0x66,
    //       167   119   77    w  
    0x67,
    //       170   120   78    x  
    0x68,
    //       171   121   79    y  
    0x69,
    //       172   122   7A    z  
    0x6a,
    //       173   123   7B    {  
    INVAL,
    //       174   124   7C    |  
    INVAL,
    //       175   125   7D    }  
    INVAL,
    //       176   126   7E    ~  
    INVAL,
    //       177   127   7F    DEL
    INVAL,
  };

  bool eos = false;
  for (unsigned int i = 0; i < DATA_LEN - 1; i++) {
    if (!eos && s[i] == 0)
      eos = true;
    char ch = eos ? ' ' : s[i];
    if (ch > 0x7f)
      ch = 0;
    buf[i] = map[ch];
  }
}

static wxString
_decode_title(const u8 *buf)
{
  static char const map[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', ':', '<', '>', '-', '.', ',',
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', ' ',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    '?', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'x', 'y', 'z', ';', ' ', '/', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  };

  char obuf[DATA_LEN];

  int i;
  for (i = 0; i < DATA_LEN - 1; i++) {
    unsigned int d = buf[i];
    if (d > 127) d = 127;
    obuf[i] = map[d];
  }
  obuf[i] = 0;
  --i;
  for (; i >= 0; --i)
    if (obuf[i] == ' ')
      obuf[i] = 0;
    else
      break;

  return obuf;
}

static void
gen_cmds(const Camera& a, const Camera& b)
{
  // title
  if (b.title != a.title) {
    bool const a_on = !a.title.empty();
    bool const b_on = !b.title.empty();
    if (!b.title.empty()) {
      u8 buf[DATA_LEN - 1];
      _gen_title(&buf[0], b.title.c_str());
      emit2(0x10, 1, &buf[0], sizeof(buf));
    }
    if (b_on != a_on)
      emit2(0x10, 0, b_on ? 1 : 0);
  }

  // title pos
  if (b.titlePos != a.titlePos) {
    // set title pos
    emit2(0x10, 3, b.titlePos);
  }

  // senseUp
  if (b.senseUp != a.senseUp) {
    // set senseUp
    emit1(0x11, b.senseUp);
  }

  // alc/elc
  if (b.alcElc != a.alcElc) {
    // set alcElc
    emit1(0x12, b.alcElc);
  }

  if (b.alc != a.alc) {
    // set alc
    emit1(0x15, b.alc);
  }
  if (b.elc != a.elc) {
    // set elc
    emit1(0x16, b.elc);
  }

  if (b.blc != a.blc) {
    // set blc
    emit2(0x18, 0, b.blc);
  }
  if (b.blcPreset != a.blcPreset) {
    // set blcPreset
    emit2(0x18, 1, b.blcPreset);
  }
  if (memcmp(&b.blcArea[0], &a.blcArea[0], sizeof(a.blcArea)) != 0) {
    // todo: set blcArea
    emit1(0x19, &b.blcArea[0], sizeof(b.blcArea));
  }
  if (b.blcPeak != a.blcPeak) {
    // set blcPeak
    emit1(0x22, b.blcPeak);
  }

  if (b.agc != a.agc) {
    // set agc
    emit2(0x1a, 0, b.agc);
  }
  if (b.agcLevel != a.agcLevel) {
    // set agcLevel
    emit2(0x1a, 1, b.agcLevel);
  }
  if (b.agcManual != a.agcManual) {
    // set agcManual
    emit2(0x1a, 2, b.agcManual);
  }

  if (b.wtb != a.wtb) {
    // set wtb
    emit2(0x1b, 0, b.wtb);
  }
  if (b.wtbMan != a.wtbMan) {
    // set wtbMan
    emit2(0x1b, 1, b.wtbMan);
  }
  if (b.wtbRed != a.wtbRed) {
    // set wtbRed
    emit2(0x1b, 2, b.wtbRed);
  }
  if (b.wtbBlue != a.wtbBlue) {
    // set wtbBlue
    emit2(0x1b, 3, b.wtbBlue);
  }

  if (b.sync != a.sync) {
    emit2(0x1c, 0, b.sync);
  }

  if (b.mask != a.mask) {
    // todo: set mask
  }

  if (b.neg != a.neg) {
    // set neg
    emit2(0x1d, 4, b.neg);
  }

  if (b.hRev != a.hRev) {
    // set hRev
    emit2(0x1d, 5, b.hRev);
  }
  if (b.vRev != a.vRev) {
    // set vRev
    emit2(0x1d, 7, b.vRev);
  }

  if (b.freezeMode != a.freezeMode) {
    // set freeze mode
    emit2(0x1d, 9, b.freezeMode);
  }
  if (b.freeze != a.freeze) {
    // set freeze
    emit2(0x1d, 8, b.freeze);
  }

  if (b.priority != a.priority) {
    // set priority
    emit2(0x1d, 6, b.priority);
  }

  if (b.gamma != a.gamma) {
    // set gamma
    emit2(0x1d, 0x14, b.gamma);
  }

  if (b.apcH != a.apcH) {
    // set apcH
    emit3(0x1d, 0x15, 1, b.apcH);
  }
  if (b.apcV != a.apcV) {
    // set apcV
    emit3(0x1d, 0x15, 2, b.apcV);
  }

  if (b.coronagraph != a.coronagraph) {
    // set coronagraph
    emit2(0x1d, 0x16, b.coronagraph);
  }

  if (b.colorBars != a.colorBars) {
    // set colorBars
    emit2(0x1d, 0x17, b.colorBars);
  }

  if (b.tec != a.tec) {
    // set tec
    emit2(0x47, 0, b.tec);
  }
  if (b.tecLevel != a.tecLevel) {
    // set tecLevel
    emit2(0x47, 2, b.tecLevel);
  }
  if (b.dewRemoval != a.dewRemoval) {
    // set dewRemoval
    emit2(0x47, 3, b.dewRemoval);
  }
  if (memcmp(&b.tecArea[0], &a.tecArea[0], sizeof(a.blcArea)) != 0) {
    // set tecArea
    emit1(0x48, &b.tecArea[0], sizeof(b.tecArea));
  }

  if (b.zoom != a.zoom) {
    // set zoom
    emit2(0x1f, 0, b.zoom);
  }
  if (b.zoomLevel != a.zoomLevel) {
    // set zoomLevel
    emit2(0x1f, 1, b.zoomLevel);
  }
}

enum {
  RDR_CONNECTED,
  RDR_CONNECT_FAILED,
  RDR_MSG,
};

struct McxMsgEvent : public wxNotifyEvent
{
  unsigned int evt_msgtype;
  msg evt_msg;

  McxMsgEvent()
    : wxNotifyEvent(EVT_MCX_MSG, 0 /*id*/) { }

  // required for sending with wxPostEvent()
  wxEvent *Clone(void) const { return new McxMsgEvent(*this); }
};

struct ReaderThread
  : public wxThread
{
  wxEvtHandler *m_evt_handler;

  ReaderThread(wxEvtHandler *evt_handler)
    : wxThread(wxTHREAD_DETACHED), m_evt_handler(evt_handler) { }
  ExitCode Entry();
};

wxThread::ExitCode
ReaderThread::Entry()
{
  wxPrintf("reader thread running\n");

  {
    bool connected = mcxcomm_connect(s_cfg.cfg_serial_port.c_str());
    McxMsgEvent evt;
    evt.evt_msgtype = connected ? RDR_CONNECTED : RDR_CONNECT_FAILED;
    m_evt_handler->AddPendingEvent(evt);
    if (!connected)
      return 0;
  }

  McxMsgEvent evt;
  evt.evt_msgtype = RDR_MSG;

  while (!TestDestroy()) {
    bool recvd = mcxcomm_recv(&evt.evt_msg, 1000);
    // todo: error vs timeout ?
    if (!recvd)
      continue;
    m_evt_handler->AddPendingEvent(evt);
  }
}

typedef void (wxEvtHandler::*McxMsgEventFunction)(McxMsgEvent&);

#define McxMsgEventHandler(func) \
  wxEVENT_HANDLER_CAST(McxMsgEventFunction, func)

class MainFrameD : public MainFrame
{
public:
  MainFrameD(wxWindow *parent);
  ~MainFrameD();

  void senseUpScroll(wxScrollEvent& event);
  void alcScroll(wxScrollEvent& event);
  void elcScroll(wxScrollEvent& event);
  void intTextEnter(wxCommandEvent& event);
  void intKillFocus(wxFocusEvent& event);
  void intBtnClicked(wxCommandEvent& event);
  void intCombobox(wxCommandEvent& event);
  void agcManScroll(wxScrollEvent& event);
  void agcAutoScroll(wxScrollEvent& event);
  void apcHScroll(wxScrollEvent& event);
  void apcVScroll(wxScrollEvent& event);
  void wtbRedScroll(wxScrollEvent& event);
  void wtbBlueScroll(wxScrollEvent& event);
  void gammaScroll(wxScrollEvent& event);
  void zoomScroll(wxScrollEvent& event);
  void tecLevelScroll(wxScrollEvent& event);
  void dewRemovalScroll(wxScrollEvent& event);
  void coronagraphScroll(wxScrollEvent& event);

  void atwSelected(wxCommandEvent& event);
  void awcSelected(wxCommandEvent& event);
  void wtbRBSelected(wxCommandEvent& event);
  void wtb3200Selected(wxCommandEvent& event);
  void wtb5600Selected(wxCommandEvent& event);
  void titleText(wxCommandEvent& event);

  void dsClicked(wxCommandEvent& event);
  void plClicked(wxCommandEvent& event);
  void luClicked(wxCommandEvent& event);
  void ldClicked(wxCommandEvent& event);
  void svClicked(wxCommandEvent& event);
  void xhClicked(wxCommandEvent& event);
  void xbClicked(wxCommandEvent& event);
  void cbClicked(wxCommandEvent& event);
  void rhClicked(wxCommandEvent& event);
  void rvClicked(wxCommandEvent& event);
  void ngClicked(wxCommandEvent& event);
  void fzClicked(wxCommandEvent& event);
  void ccClicked(wxCommandEvent& event);
  void sleepClicked(wxCommandEvent& event);
  void statusBarLeftUp(wxMouseEvent& event);

  void OnTimer(wxTimerEvent& event);
  void OnMcxMsg(McxMsgEvent& event);

  void EnableControls(EnableType enable);
  void doEnablesForSenseUp();
  void doEnablesForWtb();

  void senseUpUpdated();
  void alcUpdated();
  void elcUpdated();
  void agcManUpdated();
  void agcAutoUpdated();
  void wtbRedUpdated();
  void wtbBlueUpdated();
  void gammaUpdated();
  void apcHUpdated();
  void apcVUpdated();
  void coronagraphUpdated();
  void tecLevelUpdated();
  void dewRemovalUpdated();
  void zoomUpdated();

  void InitControls(Camera *cam);
};

static MainFrameD *
_win()
{
  return dynamic_cast<MainFrameD *>(wxGetApp().GetTopWindow());
}

static void
status(const wxString& str)
{
  _win()->m_statusBar->SetStatusText(str, 0);
}

MainFrameD::MainFrameD(wxWindow *parent)
  :
  MainFrame(parent)
{
  Connect(wxEVT_TIMER,
	  wxTimerEventHandler(MainFrameD::OnTimer),
	  NULL, this);
  Connect(EVT_MCX_MSG,
	  McxMsgEventHandler(MainFrameD::OnMcxMsg),
	  NULL, this);
  s_deferred_evt_timer = new wxTimer(this, ID_DEFERRED_EVT_TIMER);
  s_fsm_timer = new wxTimer(this, ID_FSM_TIMER);
  s_int_stopwatch = new wxStopWatch();
  s_int_timer = new wxTimer(this, ID_INT_TIMER);
  s_agc_timer = new wxTimer(this, ID_AGC_TIMER);
  s_agc_stopwatch = new wxStopWatch();
}

MainFrameD::~MainFrameD()
{
  delete s_deferred_evt_timer;
  delete s_fsm_timer;
  delete s_int_timer;
  delete s_agc_timer;
}

static wxString
_readable(const msg& cmd)
{
  char buf[128];
  mcxcmd_dump(buf, sizeof(buf), &cmd);
  return buf;
}

static void
_enable_controls(EnableType enable)
{
  _win()->EnableControls(enable);
}

static void
_cam_init(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  _enable_controls(EN_DISABLE);
  if (s_cfg.cfg_serial_port.IsEmpty())
    s_camera_state = CAM_DISCONNECTED;
  else
    s_camera_state = CAM_CONNECTING;
}

static void
_cam_disconnected(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  if (!s_cfg.cfg_serial_port.IsEmpty() && s_reader_connected == -1)
    s_camera_state = CAM_CONNECTING;
  else
    *done = true;
}

static void
_cam_connecting(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  if (!s_reader) {
    status("Connecting to camera on " + s_cfg.cfg_serial_port);
    wxPrintf("starting reader thread\n");
    wxWindow *topwin = wxGetApp().GetTopWindow();
    wxThread *const thread = new ReaderThread(topwin);
    thread->Create();
    thread->Run();
    s_reader_connected = -1;
    s_reader = thread;
    *done = true;
  }
  else if (s_reader_connected == 0) {
    // connect failed, reader will exit
    s_reader->Delete();
    s_reader = 0;
    s_camera_state = CAM_DISCONNECTED;
  }
  else if (s_reader_connected == 1) {
    s_camera_state = CAM_DISCOVER;
  }
  else
    *done = true;
}

static void
_cam_discover(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  s_fsm_got_ack = false;
  s_fsm_response.stx = 0;
  mcxcomm_send_enq();

  s_fsm_timeout = false;
  s_fsm_timer->Start(1000, wxTIMER_ONE_SHOT);

  s_camera_state = CAM_DISCOVERING;
  *done = true;
}

static void
_send_next_smry_cmd()
{
  msg req;
  mcxcmd_get(&req, 0x45);
  req.data[0] = s_fsm_next_smry;

  char buf[32];
  snprintf(buf, sizeof(buf), "Reading from camera (%u/7)", s_fsm_next_smry + 1);
  status(buf);

  s_fsm_got_ack = false;
  s_fsm_response.stx = 0;
  mcxcomm_send_msg(req);

  s_fsm_timeout = false;
  s_fsm_timer->Start(1000, wxTIMER_ONE_SHOT);
}

static void
_cam_discovering(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  if (s_fsm_got_ack) {
    s_fsm_timer->Stop();

    s_fsm_next_smry = 0;
    _send_next_smry_cmd();

    s_camera_state = CAM_READING1;
  }
  else if (s_fsm_timeout) {
    s_camera_state = CAM_DISCOVER;
    // todo: after N timeouts, kill reader, >disconnected
  }
  else
    *done = true;
}

static void
_cam_reading1(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  if (s_fsm_got_ack) {
    // restart timer for response
    s_fsm_timeout = false;
    s_fsm_timer->Start(1000, wxTIMER_ONE_SHOT);
    s_camera_state = CAM_READING2;
    *done = true;
  }
  else if (s_fsm_timeout) {
    s_camera_state = CAM_DISCOVER;
  }
  else
    *done = true;
}

static void
_handle_smry_0()
{
  s_cam0.title = _decode_title(&s_fsm_response.data[1]);
  wxPrintf("read camera: title = [%s]\n", s_cam0.title);
}

static void
_handle_smry_1()
{
  s_cam0.blcPeak   = s_fsm_response.data[1];
  s_cam0.titlePos  = s_fsm_response.data[2];
  s_cam0.wtb       = s_fsm_response.data[3];
  s_cam0.senseUp   = s_fsm_response.data[4];
  s_cam0.zoomLevel = s_fsm_response.data[5];
  s_cam0.alc       = s_fsm_response.data[6];
  s_cam0.agcManual = s_fsm_response.data[7];
  s_cam0.agcLevel  = s_fsm_response.data[8];
  //  syncVMaxHi   = s_fsm_response.data[9];
  //  syncVMaxLo   = s_fsm_response.data[10];
  //  syncVCurHi   = s_fsm_response.data[11];
  //  syncVCurLo   = s_fsm_response.data[12];
}

static void
_handle_smry_2()
{
  s_cam0.blcArea[0] = s_fsm_response.data[1];
  s_cam0.blcArea[1] = s_fsm_response.data[2];
  s_cam0.blcArea[2] = s_fsm_response.data[3];
  s_cam0.blcArea[3] = s_fsm_response.data[4];
  s_cam0.blcArea[4] = s_fsm_response.data[5];
  s_cam0.blcArea[5] = s_fsm_response.data[6];
  // unused = s_fsm_response.data[7];
  s_cam0.wtbMan = s_fsm_response.data[8];
  s_cam0.wtbBlue = s_fsm_response.data[9];
  s_cam0.wtbRed = s_fsm_response.data[10];
  s_cam0.zoom = s_fsm_response.data[11];
  s_cam0.zoomLevel = s_fsm_response.data[12];
}

static void
_handle_smry_3()
{
  s_cam0.alcElc = s_fsm_response.data[1];
  s_cam0.blc = s_fsm_response.data[2];
  s_cam0.blcPreset = s_fsm_response.data[3];
  s_cam0.agc = s_fsm_response.data[4];
  //  s_cam0.sync = s_fsm_response.data[5];
  s_cam0.sync = SYNC_INT;
  s_cam0.neg = s_fsm_response.data[6];
  s_cam0.hRev = s_fsm_response.data[7];
  s_cam0.priority = s_fsm_response.data[8];
  //  s_cam0.mask[0] = s_fsm_response.data[9];
  //  s_cam0.mask[1] = s_fsm_response.data[10];
  //  s_cam0.mask[2] = s_fsm_response.data[11];
  //  s_cam0.mask[3] = s_fsm_response.data[12];
}

static void
_handle_smry_4()
{
  s_cam0.elc        = s_fsm_response.data[1]; // ?? check this
  s_cam0.vRev       = s_fsm_response.data[2];
  s_cam0.freezeMode = s_fsm_response.data[3];
  s_cam0.freeze     = s_fsm_response.data[4];
  //  s_cam0.syncHMax = s_fsm_response.data[5];
  //  s_cam0.syncHCur = s_fsm_response.data[6];
  //  unused = s_fsm_response.data[7];
  //  unused = s_fsm_response.data[8];
  //  unused = s_fsm_response.data[9];
  //  unused = s_fsm_response.data[10];
  //  unused = s_fsm_response.data[11];
  //  unused = s_fsm_response.data[12];
}

static void
_handle_smry_5()
{
  s_cam0.tec = s_fsm_response.data[1];
  //  s_cam0.tecPreset = s_fsm_response.data[2]; ???
  s_cam0.tecLevel = s_fsm_response.data[3];
  s_cam0.tecArea[0] = s_fsm_response.data[4];
  s_cam0.tecArea[1] = s_fsm_response.data[5];
  s_cam0.tecArea[2] = s_fsm_response.data[6];
  s_cam0.tecArea[3] = s_fsm_response.data[7];
  s_cam0.tecArea[4] = s_fsm_response.data[8];
  s_cam0.tecArea[5] = s_fsm_response.data[9];
  s_cam0.dewRemoval = s_fsm_response.data[10];
  s_cam0.gamma = s_fsm_response.data[11];
  //  unused = s_fsm_response.data[12];
}

static void
_handle_smry_6()
{
  s_cam0.apcH = s_fsm_response.data[1];
  s_cam0.apcV = s_fsm_response.data[2];
  s_cam0.coronagraph = s_fsm_response.data[3]; // ??
  s_cam0.colorBars = s_fsm_response.data[4];
  //  unused = s_fsm_response.data[5..12];
}

static void
_handle_smry()
{
  wxPrintf("handle smry %u\n", (unsigned int) s_fsm_response.data[0]);

  switch (s_fsm_response.data[0]) {
  case 0: _handle_smry_0(); break;
  case 1: _handle_smry_1(); break;
  case 2: _handle_smry_2(); break;
  case 3: _handle_smry_3(); break;
  case 4: _handle_smry_4(); break;
  case 5: _handle_smry_5(); break;
  case 6: _handle_smry_6(); break;
  }
}

static void
_init_ctrl_vals()
{
  _win()->InitControls(&s_cam0);
  s_cam1 = s_cam0;
}

static void
_cam_reading2(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  if (s_fsm_response.stx == STX) {
    // got response
    s_fsm_timer->Stop();
    mcxcomm_send_ack();

    _handle_smry();

    if (++s_fsm_next_smry < 7) {
      _send_next_smry_cmd();
      s_camera_state = CAM_READING1;
      *done = true;
    }
    else {
      _init_ctrl_vals();
      _enable_controls(EN_ENABLE_ALL);
      status("");
      s_camera_state = CAM_UPTODATE;
    }
  }
  else if (s_fsm_timeout) {
    s_camera_state = CAM_DISCOVER;
  }
  else {
    *done = 1;
  }
}

static void
_cam_sending1(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  if (s_fsm_got_ack) {
    // restart timer for response
    s_fsm_timeout = false;
    s_fsm_timer->Start(1000, wxTIMER_ONE_SHOT);
    s_camera_state = CAM_SENDING2;
    *done = true;
  }
  else if (s_fsm_timeout) {
    s_camera_state = CAM_DISCOVER;
  }
  else
    *done = true;
}

static void
_cam_sending2(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  if (s_fsm_response.stx == STX) {
    // got response
    s_fsm_timer->Stop();
    mcxcomm_send_ack();
    s_camera_state = CAM_UPTODATE;
  }
  else if (s_fsm_timeout) {
    s_camera_state = CAM_DISCOVER;
  }
  else
    *done = true;
}

static void
_cam_uptodate(bool *done)
{
  //  wxPrintf("dostate %s\n", __FUNCTION__);

  cmdmap_t::iterator it = s_cmdmap.begin();

  if (it != s_cmdmap.end()) {
    s_active_cmd = it->second;
    s_cmdmap.erase(it);

    wxPrintf("send_cmd: [%s]\n", _readable(s_active_cmd));

    s_fsm_got_ack = false;
    s_fsm_response.stx = 0;
    mcxcomm_send_msg(s_active_cmd);

    s_fsm_timeout = false;
    s_fsm_timer->Start(1000, wxTIMER_ONE_SHOT);

    s_camera_state = CAM_SENDING1;
  }

  *done = true;
}

static void
_cam_shutting_down(bool *done)
{
  wxPrintf("dostate %s TODO\n", __FUNCTION__);
  *done = true;
  // todo
}

static void
__update_int_time()
{
  MainFrameD *win = _win();
  wxString s = win->m_int->GetValue();

  long l = ::atol(s.c_str());
  if (l < 3) {
    l = 3;
    win->m_int->SetValue("3");
  }

  s_int_time = l * 1000;
}

static void
_update_int_time()
{
  __update_int_time();
  _do_camera_fsm();
}

static void
_int_init1(bool *done)
{
  // setup for integration

  //  wxPrintf("INT_FSM %s\n", __FUNCTION__);

  _win()->m_intBtn->SetLabel("Stop");

  _enable_controls(EN_DISABLE_FOR_INT);

  __update_int_time();
  s_int_stop_clicked = false;
  s_int_state = INT_INIT2;
}

inline static bool
_camera_cmds_in_flight()
{
  return s_camera_state != CAM_UPTODATE || !s_cmdmap.empty();
}

static void
_int_init2(bool *done)
{
  // wait for queued commands to drain

  if (!_camera_cmds_in_flight())
    s_int_state = INT_INT1;
  else
    *done = true;
}

static void
_int_status(long elapsed)
{
  if (elapsed == -1)
    status("");
  else {
    unsigned int total = (unsigned int) (s_int_time / 1000);
    unsigned int e = elapsed / 1000;
    unsigned int rem = total - e;
    char buf[80];
    snprintf(buf, sizeof(buf), "INT %2u / %2u REM = %2u", e, total, rem);
    status(buf);
  }
}

static void
_int_int1(bool *done)
{
  // start integration timers

  //  wxPrintf("INT_FSM %s\n", __FUNCTION__);
  s_int_stopwatch->Start();
  s_int_timer->Start(500, wxTIMER_CONTINUOUS);
  //  wxPrintf("send sync=vbs\n");
  s_cam1.sync = SYNC_VBS;
  dnotify(UPD_IMMEDIATE);
  _int_status(0);
  s_int_state = INT_INT2;
  *done = 1;
}

static void
_int_int2(bool *done)
{
  // handle events during integration

  //  wxPrintf("INT_FSM %s\n", __FUNCTION__);

  if (s_int_stop_clicked) {
    s_int_timer->Stop();
    _int_status(-1);
    s_int_state = INT_CAPTURE1;
  }
  else {
    long elapsed = s_int_stopwatch->Time();
    _int_status(elapsed);

    if (elapsed >= s_int_time) {
      s_int_timer->Stop();
      s_int_state = INT_CAPTURE1;
    }
    else
      *done = true;
  }
}

static void
_int_capture1(bool *done)
{
  // wait for commands to drain; revert sync to int

  //  wxPrintf("INT_FSM %s\n", __FUNCTION__);

  if (!_camera_cmds_in_flight()) {
    //wxPrintf("todo: send sync=int\n");
    s_cam1.sync = SYNC_INT;
    dnotify(UPD_IMMEDIATE);
    if (s_int_stop_clicked)
      s_int_state = INT_STOP;
    else {
      s_int_timer_expired = false;
      s_int_timer->Start(175, wxTIMER_ONE_SHOT);
      s_int_state = INT_CAPTURE2;
      *done = true;
    }
  }
  else
    *done = true;
}

static void
_int_capture2(bool *done)
{
  // wait for trigger timer

  //  wxPrintf("INT_FSM %s\n", __FUNCTION__);

  if (s_int_timer_expired)
    s_int_state = INT_INT1;
  else
    *done = true;

  // todo: handle stop button clicked?
}

static void
_int_stop(bool *done)
{
  //  wxPrintf("INT_FSM %s\n", __FUNCTION__);
  _win()->m_intBtn->SetLabel("Start");
  _win()->EnableControls(EN_ENABLE_ALL);
  s_int_state = INT_STOPPED;
}

static void
_do_int_fsm()
{
  bool done = false;

  do {
    switch (s_int_state) {
    case INT_STOPPED:  done = true;          break;
    case INT_INIT1:    _int_init1(&done);    break;
    case INT_INIT2:    _int_init2(&done);    break;
    case INT_INT1:     _int_int1(&done);     break;
    case INT_INT2:     _int_int2(&done);     break;
    case INT_CAPTURE1: _int_capture1(&done); break;
    case INT_CAPTURE2: _int_capture2(&done); break;
    case INT_STOP:     _int_stop(&done);     break;
    default:
      wxASSERT(false);
      done = true;
      break;
    }
  } while (!done);
}

static bool
_send_agc()
{
  int const p = _win()->m_agcMan->GetValue();
  int const val[] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  if (p == 0) {
    s_cam1.agc = 0; // off
  }
  else {
    s_cam1.agc = 2; // manual
    s_cam1.agcManual = val[p];
  }
  bool changed =
    s_cam1.agc != s_cam0.agc ||
    s_cam1.agcManual != s_cam0.agcManual;

  // todo: does agc auto change require similar treatment?

  dnotify(UPD_IMMEDIATE);

  return changed;
}

static void
_update_agc_status(long elapsed)
{
  if (elapsed == -1)
    status("");
  else {
    unsigned int total = (unsigned int) (AGC_WAIT_MS / 1000);
    unsigned int e = elapsed / 1000;
    unsigned int rem = total - e;
    char buf[80];
    snprintf(buf, sizeof(buf), "AGC Stablizing %2u / %2u REM = %2u  Click here to Cancel", e, total, rem);
    status(buf);
  }
}

static void
_agc_wait_init(bool *done)
{
  s_agc_timer_expired = false;
  s_agc_timer->Start(4000, wxTIMER_ONE_SHOT);
  s_agc_wait_state = AGC_WAIT1;
  *done = true;
}

static void
_agc_wait1(bool *done)
{
  if (s_agc_timer_expired)
    if (_camera_cmds_in_flight())
      *done = true; // go back and wait
    else {
      bool changed = _send_agc();
      if (changed) {
	_enable_controls(EN_DISABLE);
	_update_agc_status(0);
	s_agc_wait_cancel_clicked = false;
	s_agc_stopwatch->Start();
	s_agc_timer->Start(500, wxTIMER_CONTINUOUS);
	s_agc_wait_state = AGC_WAIT2;
	*done = true;
      }
      else
	s_agc_wait_state = AGC_STABLE;
    }
  else
    *done = true;
}

static void
_agc_wait2(bool *done)
{
  if (s_agc_wait_cancel_clicked)
    s_agc_wait_state = AGC_CLEANUP;
  else {
    long elapsed = s_agc_stopwatch->Time();
    _update_agc_status(elapsed);
    if (elapsed >= AGC_WAIT_MS)
      s_agc_wait_state = AGC_CLEANUP;
    else
      *done = true;
  }
}

static void
_agc_cleanup(bool *done)
{
  s_agc_timer->Stop();
  _enable_controls(EN_ENABLE_ALL);
  _update_agc_status(-1);
  s_agc_wait_state = AGC_STABLE;
}

static void
_do_agc_wait_fsm()
{
  bool done = false;

  do {
    switch (s_agc_wait_state) {
    case AGC_STABLE:    done = true;            break;
    case AGC_WAIT_INIT: _agc_wait_init(&done);  break;
    case AGC_WAIT1:     _agc_wait1(&done);      break;
    case AGC_WAIT2:     _agc_wait2(&done);      break;
    case AGC_CLEANUP:   _agc_cleanup(&done);    break;
    default:
      wxASSERT(false);
      done = true;
      break;
    }
  } while (!done);
}

static void
___do_camera_fsm()
{
  bool done = false;

  do {
    switch (s_camera_state) {
    case CAM_INIT:          _cam_init(&done);          break;
    case CAM_DISCONNECTED:  _cam_disconnected(&done);  break;
    case CAM_CONNECTING:    _cam_connecting(&done);    break;
    case CAM_DISCOVER:      _cam_discover(&done);      break;
    case CAM_DISCOVERING:   _cam_discovering(&done);   break;
    case CAM_READING1:      _cam_reading1(&done);      break;
    case CAM_READING2:      _cam_reading2(&done);      break;
    case CAM_SENDING1:      _cam_sending1(&done);      break;
    case CAM_SENDING2:      _cam_sending2(&done);      break;
    case CAM_UPTODATE:      _cam_uptodate(&done);      break;
    case CAM_SHUTTING_DOWN: _cam_shutting_down(&done); break;
    default:
      wxASSERT(false);
      done = true;
      break;
    }
  } while (!done);
}

static void
__do_camera_fsm()
{
  //  wxPrintf("_DO_FSM enter\n");

  ___do_camera_fsm();
  _do_int_fsm();
  _do_agc_wait_fsm();

  //  wxPrintf("_DO_FSM exit\n");
}

static void
_do_camera_fsm()
{
  static bool s_fsm_active;
  static bool s_fsm_resched;

  if (s_fsm_active) {
    s_fsm_resched = true;
    return;
  }

  s_fsm_active = true;

  do {
    s_fsm_resched = false;
    __do_camera_fsm();
  } while (s_fsm_resched);

  s_fsm_active = false;
}

static void
_dnotify()
{
  // fill in s_cmdmap
  gen_cmds(s_cam0, s_cam1);
  s_cam0 = s_cam1;

  // send buffered commands to camera
  _do_camera_fsm();
}

static void
dnotify(int when)
{
  switch (when) {
  case UPD_IMMEDIATE:
    s_deferred_evt_timer->Stop();
    _dnotify();
    break;
  case UPD_DEFER:
    // reset timer
    s_deferred_evt_timer->Start(DEFERRED_EVENT_INTERVAL, wxTIMER_ONE_SHOT);
    break;
  }
}

void
MainFrameD::doEnablesForSenseUp()
{
  int const p = m_senseUp->GetValue();

  if (p == 12) { // 128x
    // enable integration controls
    m_int->Enable(true);
    m_intBtn->Enable(true);
    m_intBtn->SetLabel("Start");
  }
  else {
    // stop integration and disable integration controls
    m_int->Enable(false);
    m_intBtn->Enable(false);
    m_intBtn->SetLabel("Start");
  }
}

void
MainFrameD::senseUpUpdated()
{
  const char *sval[] = { "Off", "2x", "4x", "6x", "8x", "12x", "16x", "24x", "32x", "48x", "64x", "96x", "128x" };
  int const p = m_senseUp->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_senseUpVal->SetLabel(sval[p]);

  doEnablesForSenseUp();
}

void
MainFrameD::senseUpScroll(wxScrollEvent&)
{
  senseUpUpdated();
  int const val[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  int const p = m_senseUp->GetValue();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.senseUp = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::alcUpdated()
{
  const char *sval[] = { "Off", "1/100", "1/120", "1/180", "1/250", "1/350", "1/500", "1/750", "1/1000",
			 "1/1500", "1/2000", "1/3000", "1/4000", "1/6000", "1/8000", "1/12000" };
  int const p = m_alc->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_alcVal->SetLabel(sval[p]);
}

void
MainFrameD::alcScroll(wxScrollEvent& event)
{
  alcUpdated();

  int const p = event.GetPosition();

  if (p > 0) {
    // force sense-up off
    m_senseUp->SetValue(0);
    wxScrollEvent ev(wxEVT_SCROLL_TOP);
    m_senseUp->GetEventHandler()->ProcessEvent(ev);
    // set ELC off
    m_elc->SetValue(0);
    m_elc->GetEventHandler()->ProcessEvent(ev);
  }

  int const val[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  s_cam1.alcElc = 0; // alc
  s_cam1.alc = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::intTextEnter(wxCommandEvent& event)
{
  _update_int_time();
}

void
MainFrameD::intKillFocus(wxFocusEvent& event)
{
  _update_int_time();
}

void
MainFrameD::intCombobox(wxCommandEvent& event)
{
  _update_int_time();
}

void
MainFrameD::elcUpdated()
{
  int const p = m_elc->GetValue();

  const char *sval[] = { "Off", "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_elcVal->SetLabel(sval[p]);
}

void
MainFrameD::elcScroll(wxScrollEvent& event)
{
  elcUpdated();

  int const p = event.GetPosition();

  if (p > 0) {
    // force sense-up off
    m_senseUp->SetValue(0);
    wxScrollEvent ev(wxEVT_SCROLL_TOP);
    m_senseUp->GetEventHandler()->ProcessEvent(ev);
    // set ALC off
    m_alc->SetValue(0);
    m_alc->GetEventHandler()->ProcessEvent(ev);
  }

  int const val[] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  if (p == 0) { // off
    s_cam1.alcElc = 0; // alc
    s_cam1.alc = 0; // alc off
  }
  else {
    s_cam1.alcElc = 1; // elc
    s_cam1.elc = val[p];
  }
  dnotify(UPD_DEFER);
}

static void
_start_integration()
{
  s_int_state = INT_INIT1;
  _do_camera_fsm();
}

static void
_stop_integration()
{
  s_int_stop_clicked = true;
  _do_camera_fsm();
}

void
MainFrameD::intBtnClicked(wxCommandEvent& event)
{
  if (s_int_state != INT_STOPPED)
    _stop_integration();
  else
    _start_integration();
}

void
MainFrameD::agcManUpdated()
{
  const char *sval[] = { "Off", "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_agcMan->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_agcManVal->SetLabel(sval[p]);
}

void
MainFrameD::agcManScroll(wxScrollEvent& event)
{
  agcManUpdated();

  int const p = event.GetPosition();
  if (p > 0) {
    // set AGC Auto off
    m_agcAuto->SetValue(0);
    wxScrollEvent ev(wxEVT_SCROLL_TOP);
    m_agcAuto->GetEventHandler()->ProcessEvent(ev);
  }

  s_agc_wait_state = AGC_WAIT_INIT;
  _do_camera_fsm();
}

void
MainFrameD::agcAutoUpdated()
{
  const char *sval[] = { "Off", "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_agcAuto->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_agcAutoVal->SetLabel(sval[p]);
}

void
MainFrameD::agcAutoScroll(wxScrollEvent& event)
{
  agcAutoUpdated();

  int const p = event.GetPosition();

  if (p > 0) {
    // set AGC Manual off
    m_agcMan->SetValue(0);
    wxScrollEvent ev(wxEVT_SCROLL_TOP);
    m_agcMan->GetEventHandler()->ProcessEvent(ev);
  }

  int const val[] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  if (p == 0) {
    s_cam1.agc = 0; // off
  }
  else {
    s_cam1.agc = 1; // on (auto)
    s_cam1.agcLevel = val[p];
  }
  dnotify(UPD_DEFER);
}

void
MainFrameD::apcHUpdated()
{
  const char *sval[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_apcH->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_apcHVal->SetLabel(sval[p]);
}

void
MainFrameD::apcHScroll(wxScrollEvent& event)
{
  apcHUpdated();

  int const val[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.apcH = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::apcVUpdated()
{
  const char *sval[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_apcV->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_apcVVal->SetLabel(sval[p]);
}

void
MainFrameD::apcVScroll(wxScrollEvent& event)
{
  apcVUpdated();

  int const val[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.apcV = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::wtbRedUpdated()
{
  const char *sval[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_wtbRed->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_wtbRedVal->SetLabel(sval[p]);
}

void
MainFrameD::wtbRedScroll(wxScrollEvent& event)
{
  wtbRedUpdated();
  int const val[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.wtbRed = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::wtbBlueUpdated()
{
  const char *sval[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_wtbBlue->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_wtbBlueVal->SetLabel(sval[p]);
}

void
MainFrameD::wtbBlueScroll(wxScrollEvent& event)
{
  wtbBlueUpdated();
  int const val[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.wtbBlue = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::gammaUpdated()
{
  const char *sval[] = { "0.75", "1.0" };
  int const p = m_gamma->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_gammaVal->SetLabel(sval[p]);
}

void
MainFrameD::gammaScroll(wxScrollEvent& event)
{
  gammaUpdated();
  int const val[] = { 0, 1 };
  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.gamma = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::zoomUpdated()
{
  const char *sval[] = { "Off", "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_zoom->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_zoomVal->SetLabel(sval[p]);
}

void
MainFrameD::zoomScroll(wxScrollEvent& event)
{
  zoomUpdated();

  int const val[] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));

  if (p == 0) {
    s_cam1.zoom = 0; // off
  }
  else {
    s_cam1.zoom = 1; // on
    s_cam1.zoomLevel = val[p];
  }
  dnotify(UPD_DEFER);
}

void
MainFrameD::tecLevelUpdated()
{
  const char *sval[] = { "Off", "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_tecLevel->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_tecLevelVal->SetLabel(sval[p]);
}

void
MainFrameD::tecLevelScroll(wxScrollEvent& event)
{
  tecLevelUpdated();

  int const val[] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));

  if (p == 0) {
    s_cam1.tec = 0; // off
    s_cam1.tecLevel = 0;
  }
  else {
    s_cam1.tec = 1; // on
    s_cam1.tecLevel = val[p];
  }
  dnotify(UPD_DEFER);
}

void
MainFrameD::dewRemovalUpdated()
{
  const char *sval[] = { "10", "30", "60" };
  int const p = m_dewRemoval->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_dewRemovalVal->SetLabel(sval[p]);
}

void
MainFrameD::dewRemovalScroll(wxScrollEvent& event)
{
  dewRemovalUpdated();
  int const val[] = { 0, 1, 2 };
  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.dewRemoval = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::coronagraphUpdated()
{
  const char *sval[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", };
  int const p = m_coronagraph->GetValue();
  wxASSERT(p >= 0 && p < lengthof(sval));
  m_coronagraphVal->SetLabel(sval[p]);
}

void
MainFrameD::coronagraphScroll(wxScrollEvent& event)
{
  coronagraphUpdated();
  int const val[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  int const p = event.GetPosition();
  wxASSERT(p >= 0 && p < lengthof(val));
  s_cam1.coronagraph = val[p];
  dnotify(UPD_DEFER);
}

void
MainFrameD::doEnablesForWtb()
{
  if (m_atwBtn->GetValue()) {
    m_wtbRed->Enable(false);
    m_wtbBlue->Enable(false);
    m_awcSet->Enable(false);
  }
  else if (m_awcBtn->GetValue()) {
    m_wtbRed->Enable(false);
    m_wtbBlue->Enable(false);
    m_awcSet->Enable(true);
  }
  else if (m_wtbRbBtn->GetValue()) {
    m_wtbRed->Enable(true);
    m_wtbBlue->Enable(true);
    m_awcSet->Enable(false);
  }
  else if (m_wtb3200Btn->GetValue()) {
    m_wtbRed->Enable(false);
    m_wtbBlue->Enable(false);
    m_awcSet->Enable(false);
  }
  else if (m_wtb5600Btn->GetValue()) {
    m_wtbRed->Enable(false);
    m_wtbBlue->Enable(false);
    m_awcSet->Enable(false);
  }
}

void
MainFrameD::InitControls(Camera *cam)
{
  m_title->SetValue(cam->title);

  switch (cam->titlePos) {
  case 0: m_titleTL->SetValue(true); break;
  case 1: m_titleBL->SetValue(true); break;
  case 2: m_titleTR->SetValue(true); break;
  case 3: m_titleBR->SetValue(true); break;
  }

  m_senseUp->SetValue(cam->senseUp);
  senseUpUpdated();

  if (cam->alcElc == 0) { // alc
    m_alc->SetValue(cam->alc);
    alcUpdated();
    cam->elc = 0;
    m_elc->SetValue(0);
    elcUpdated();
  }
  else { // elc
    m_elc->SetValue(cam->elc);
    elcUpdated();
    cam->alc = 0;
    m_alc->SetValue(0);
    alcUpdated();
  }

  // todo

  //u8 blc; // 0=off 1=on 2=peak
  //u8 blcPreset; // 0=off 1=on [when blc=on]
  //u8 blcArea[6];
  //u8 blcPeak; // 0=min 8=max

  if (cam->agc == 0) { // agc off
    cam->agcManual = 0;
    m_agcMan->SetValue(0); // "Off"
    cam->agcLevel = 0;
    m_agcAuto->SetValue(0);
  }
  else if (cam->agc == 1) { // agc on
    cam->agcManual = 0;
    m_agcMan->SetValue(0); // "Off"
    m_agcAuto->SetValue(cam->agcLevel + 1); // 0=Off
  }
  else { // agc manual
    m_agcMan->SetValue(cam->agcManual + 1); // 0=Off
    cam->agcLevel = 0;
    m_agcAuto->SetValue(0);
  }
  agcManUpdated();
  agcAutoUpdated();

  if (cam->wtb != 2) { // atw or awc
    m_atwBtn->SetValue(cam->wtb == 0);
    m_awcBtn->SetValue(cam->wtb == 1);
    m_wtbRbBtn->SetValue(false);
    m_wtb3200Btn->SetValue(false);
    m_wtb5600Btn->SetValue(false);
    cam->wtbMan = 2;
  }
  else { // man
    m_atwBtn->SetValue(false);
    m_awcBtn->SetValue(false);
    m_wtbRbBtn->SetValue(cam->wtbMan == 2);
    m_wtb3200Btn->SetValue(cam->wtbMan == 0);
    m_wtb5600Btn->SetValue(cam->wtbMan == 1);
  }

  m_wtbRed->SetValue(cam->wtbRed);
  wtbRedUpdated();
  m_wtbBlue->SetValue(cam->wtbBlue);
  wtbBlueUpdated();

  doEnablesForWtb();

  cam->mask = 0; // off - don't persist this
  m_toolBar->ToggleTool(ID_CROSS_BOX, false);
  m_toolBar->ToggleTool(ID_CROSS_HAIRS, false);

  m_toolBar->ToggleTool(ID_NEGATIVE, cam->neg == 1);
  m_toolBar->ToggleTool(ID_H_REV, cam->hRev == 1);
  m_toolBar->ToggleTool(ID_V_REV, cam->vRev == 1);
  m_toolBar->ToggleTool(ID_FREEZE, cam->freeze == 1);

  m_priority->SetSelection(cam->priority);  // 0=agc 1=senseup

  m_gamma->SetValue(cam->gamma);
  gammaUpdated();

  m_apcH->SetValue(cam->apcH);
  apcHUpdated();

  m_apcV->SetValue(cam->apcV);
  apcVUpdated();

  m_coronagraph->SetValue(cam->coronagraph);
  coronagraphUpdated();

  m_toolBar->ToggleTool(ID_COLOR_BARS, cam->colorBars == 1);

  if (cam->tec) {
    m_tecLevel->SetValue(cam->tecLevel + 1); // 0 = Off
  }
  else {
    cam->tecLevel = 0;
    m_tecLevel->SetValue(0);
  }
  tecLevelUpdated();

  m_dewRemoval->SetValue(cam->dewRemoval);
  dewRemovalUpdated();

  if (cam->zoom) {
    m_zoom->SetValue(cam->zoomLevel + 1); // 0 = Off
  }
  else {
    cam->zoomLevel = 0;
    m_zoom->SetValue(0);
  }
  zoomUpdated();
}

void
MainFrameD::atwSelected(wxCommandEvent&)
{
  doEnablesForWtb();
  s_cam1.wtb = 0; // atw
  dnotify(UPD_IMMEDIATE);
}

void
MainFrameD::awcSelected(wxCommandEvent& event)
{
  doEnablesForWtb();
  s_cam1.wtb = 1; // awc
  dnotify(UPD_IMMEDIATE);
}

void
MainFrameD::wtbRBSelected(wxCommandEvent& event)
{
  doEnablesForWtb();
  s_cam1.wtb = 2; // manual
  s_cam1.wtbMan = 2; // user
  dnotify(UPD_IMMEDIATE);
}

void
MainFrameD::wtb3200Selected(wxCommandEvent& event)
{
  doEnablesForWtb();
  s_cam1.wtb = 2; // manual
  s_cam1.wtbMan = 0; // 3200
  dnotify(UPD_IMMEDIATE);
}

void
MainFrameD::wtb5600Selected(wxCommandEvent& event)
{
  doEnablesForWtb();
  s_cam1.wtb = 2; // manual
  s_cam1.wtbMan = 1; // 5600
  dnotify(UPD_IMMEDIATE);
}

void
MainFrameD::titleText(wxCommandEvent& event)
{
  s_cam1.title = m_title->GetValue();
  dnotify(UPD_DEFER);
}

void
MainFrameD::dsClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
  // todo: load ds presets
}

void
MainFrameD::plClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
  // todo: load ds presets
}

void
MainFrameD::luClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::ldClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::svClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::xhClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
  bool on = m_toolBar->GetToolState(ID_CROSS_HAIRS);
  if (on) {
    m_toolBar->ToggleTool(ID_CROSS_BOX, false);
  }
  // todo send cmd
}

void
MainFrameD::xbClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
  bool on = m_toolBar->GetToolState(ID_CROSS_BOX);
  if (on) {
    m_toolBar->ToggleTool(ID_CROSS_HAIRS, false);
  }
  // todo send cmd
}

void
MainFrameD::cbClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::rhClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::rvClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::ngClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::fzClicked(wxCommandEvent& event)
{
  wxPrintf("%s\n", __FUNCTION__);
}

void
MainFrameD::ccClicked(wxCommandEvent& event)
{
  wxPrintf("%s id=%d\n", __FUNCTION__, event.GetId());
}

void
MainFrameD::sleepClicked(wxCommandEvent& event)
{
  wxPrintf("%s id=%d\n", __FUNCTION__, event.GetId());
}

void
MainFrameD::statusBarLeftUp(wxMouseEvent& event)
{
  s_agc_wait_cancel_clicked = true;
  _do_camera_fsm();
}

void
MainFrameD::OnTimer(wxTimerEvent& event)
{
  switch (event.GetId()) {
  case ID_DEFERRED_EVT_TIMER:
    _dnotify();
    break;
  case ID_FSM_TIMER:
    wxPrintf("fsm timeout\n");
    s_fsm_timeout = true;
    _do_camera_fsm();
    break;
  case ID_INT_TIMER:
    s_int_timer_expired = true;
    _do_camera_fsm();
    break;
  case ID_AGC_TIMER:
    s_agc_timer_expired = true;
    _do_camera_fsm();
    break;
  }
}

void
MainFrameD::OnMcxMsg(McxMsgEvent& event)
{
  //  wxPrintf("got mcx msg %u\n", event.evt_msgtype);

  switch (event.evt_msgtype) {
  case RDR_CONNECTED:
    s_reader_connected = 1;
    break;
  case RDR_CONNECT_FAILED:
    s_reader_connected = 0;
    break;
  case RDR_MSG:
wxPrintf("got %s\n", event.evt_msg.stx == ACK ? "ACK" : "response");
// todo: handle NAK and EOT
    if (event.evt_msg.stx == ACK)
      s_fsm_got_ack = true;
    else {
      wxPrintf("recvd: [%s]\n", _readable(event.evt_msg));
      s_fsm_response = event.evt_msg;
    }
    break;
  }

  _do_camera_fsm();
}

void
MainFrameD::EnableControls(EnableType how)
{
  wxPrintf("enable controls %d\n", how);

  bool enable;
  bool for_int;

  switch (how) {
  case EN_DISABLE:
    enable = false;
    for_int = false;
    break;
  case EN_DISABLE_FOR_INT:
    enable = false;
    for_int = true;
    break;
  case EN_ENABLE_ALL:
    enable = true;
    for_int = false;
    break;
  }

  m_senseUp->Enable(enable);
  m_alc->Enable(enable);
  m_elc->Enable(enable);
  m_agcMan->Enable(enable);
  m_agcAuto->Enable(enable);
  m_priority->Enable(enable);
  m_tecLevel->Enable(enable);
  m_dewRemoval->Enable(enable);
  m_coronagraph->Enable(enable);
  m_titleTL->Enable(enable);
  m_titleTR->Enable(enable);
  m_titleBL->Enable(enable);
  m_titleBR->Enable(enable);
  m_title->Enable(enable);
  m_zoom->Enable(enable);

  m_toolBar->EnableTool(ID_DSO, enable);
  m_toolBar->EnableTool(ID_PLANET, enable);
  m_toolBar->EnableTool(ID_LUNAR, enable);
  m_toolBar->EnableTool(ID_LOAD, enable);
  m_toolBar->EnableTool(ID_SAVE, enable);
  m_toolBar->EnableTool(ID_CROSS_HAIRS, enable);
  m_toolBar->EnableTool(ID_CROSS_BOX, enable);
  m_toolBar->EnableTool(ID_COLOR_BARS, enable);
  m_toolBar->EnableTool(ID_H_REV, enable);
  m_toolBar->EnableTool(ID_V_REV, enable);
  m_toolBar->EnableTool(ID_NEGATIVE, enable);
  m_toolBar->EnableTool(ID_CCD_MODE, enable);
  m_toolBar->EnableTool(ID_SLEEP, enable);

  if (!for_int) {
    m_int->Enable(enable);
    m_intBtn->Enable(enable);
    m_gamma->Enable(enable);
    m_apcH->Enable(enable);
    m_apcV->Enable(enable);
    m_toolBar->EnableTool(ID_FREEZE, enable);
    m_atwBtn->Enable(enable);
    m_awcBtn->Enable(enable);
    m_awcSet->Enable(enable);
    m_wtbRbBtn->Enable(enable);
    m_wtb3200Btn->Enable(enable);
    m_wtb5600Btn->Enable(enable);
    m_wtbRed->Enable(enable);
    m_wtbBlue->Enable(enable);
    m_wtbBlueVal->Enable(enable);
  }

  if (enable) {
    doEnablesForSenseUp();
    doEnablesForWtb();
  }

  //  m_toolBar->Enable(enable);
}

bool
McxApp::OnInit()
{
    wxFrame *frame = new MainFrameD(0);
    frame->Show(true);
    SetTopWindow(frame);
//fixme:testing
s_cfg.cfg_serial_port = "[simulated]";
    _do_camera_fsm();
    return true;
}

int
McxApp::OnExit()
{
  if (s_reader) {
    wxPrintf("APP EXIT DELETE RDR\n");
    s_reader->Delete();
    s_reader = 0;
  }
  return inherited::OnExit();
}

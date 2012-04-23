#ifndef MCX_INCLUDED
#define MCX_INCLUDED

#define WARN(format, ...) wxLogWarning(format, ## __VA_ARGS__)
#define MSG(format, ...) wxLogMessage(format, ## __VA_ARGS__)
#define VERBOSE(format, ...) wxLogVerbose(format, ## __VA_ARGS__)

#endif

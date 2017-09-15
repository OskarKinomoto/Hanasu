#pragma once

#include <sstream>
#include <syslog.h>

extern int _log_level;

#define CERR_LOGGER
#define LOG_ONLY_ERR

#ifdef SYSTEMD_LOGGER
#define _LOGGER(LEVEL, msg) ?_?(LEVEL, msg)
#elif defined(CERR_LOGGER)
#include <iostream>
#define _LOGGER(LEVEL, msg) std::cerr << msg << std::endl
#else
#define _LOGGER(LEVEL, msg) syslog(LEVEL, msg)
#endif

#ifndef NO_LOGS
#define KI_LOG(log_level, msg) \
    do { \
        if (log_level <= _log_level) { \
            std::stringstream stream; \
            stream << msg; \
            _LOGGER(log_level, stream.str().c_str()); \
        } \
    } while (false)

#else
#define KI_LOG(...)
#endif

#define LOGA(msg) KI_LOG(LOG_ALERT, msg)
#define LOGC(msg) KI_LOG(LOG_CRIT, msg)
#define LOGE(msg) KI_LOG(LOG_ERR, msg)

#ifndef LOG_ONLY_ERR

#define LOGW(msg) KI_LOG(LOG_WARNING, msg)
#define LOGN(msg) KI_LOG(LOG_NOTICE, msg)
#define LOGI(msg) KI_LOG(LOG_INFO, msg)
#define LOGD(msg) KI_LOG(LOG_DEBUG, msg)

#else

#define LOGW(msg)
#define LOGN(msg)
#define LOGI(msg)
#define LOGD(msg)

#endif

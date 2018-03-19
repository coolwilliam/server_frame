/*
 * customloglevel.h
 *
 *  Created on: 2015年10月20日
 *      Author: jumper
 */

#ifndef INCLUDE_CUSTOMLOGLEVEL_H_
#define INCLUDE_CUSTOMLOGLEVEL_H_

#include <log4cplus/logger.h>

#include <log4cplus/helpers/loglog.h>



using namespace log4cplus;

using namespace log4cplus::helpers;


//级别说明
//NOT_SET_LOG_LEVEL ( -1)
//接受缺省的LogLevel，如果有父logger则继承它的LogLevel
//
//ALL_LOG_LEVEL (0)
//开放所有log信息输出
//
//TRACE_LOG_LEVEL (0)
//开放trace信息输出(即ALL_LOG_LEVEL)
//
//DEBUG_LOG_LEVEL(10000)
//开放debug信息输出
//
//INFO_LOG_LEVEL (20000)
//开放info信息输出
//
//WARN_LOG_LEVEL (30000)
//开放warning信息输出
//
//ERROR_LOG_LEVEL(40000)
//开放error信息输出
//
//FATAL_LOG_LEVEL (50000)
//开放fatal信息输出
//
//OFF_LOG_LEVEL (60000)
//关闭所有log信息输出


const LogLevel CRITICAL_LOG_LEVEL =  45000;

#if !defined(LOG4CPLUS_DISABLE_CRITICAL)
#define LOG4CPLUS_CRITICAL(logger, logEvent)  \
    if(logger.isEnabledFor(CRITICAL_LOG_LEVEL)) { \
    log4cplus::tostringstream _log4cplus_buf;  \
    _log4cplus_buf << logEvent; \
    logger.forcedLog(CRITICAL_LOG_LEVEL,  _log4cplus_buf.str(), __FILE__, __LINE__); \
    }
#else
#define LOG4CPLUS_CRITICAL(logger, logEvent) LOG4CPLUS_DOWHILE_NOTHING()
#endif



#endif /* INCLUDE_CUSTOMLOGLEVEL_H_ */

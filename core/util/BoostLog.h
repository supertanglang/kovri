#ifndef BOOST_LOG_H__
#define BOOST_LOG_H__
#include "Log.h"
#include <boost/log/core/core.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <memory>
namespace kovri
{
namespace log
{
    typedef boost::log::core_ptr core_ptr;
    
    typedef boost::log::sinks::text_ostream_backend backend_t;
    typedef boost::shared_ptr<backend_t> backend_ptr;
    
    typedef boost::log::sinks::asynchronous_sink< backend_t > sink_t;
    typedef boost::shared_ptr<sink_t> sink_ptr;
    
    typedef boost::log::sources::severity_channel_logger_mt<LogLevel, std::string> log_t;
    
    class LogStreamImpl
    {
    public:
        LogStreamImpl(log_t & l, LogLevel levelno);
        ~LogStreamImpl() {}
        void MetaImpl(const std::string & key, std::string value);
        void Flush();
        bool IsEnabled() { return m_Enable; };
        void Disable() { m_Enable = false; };
        void Enable() { m_Enable = true; };
        std::streambuf * Stream() { return &m_str; }
    private:
        log_t & m_Log;
        LogLevel m_Level;
        bool m_Enable;
        std::stringbuf m_str;
    };

    class BoostEventStream : public EventStream
    {
    public:
        virtual EventStream & Flush() const {};
        virtual EventStream & operator << (const std::vector<std::string> & strs ) const {};
    };
    
    class LoggerImpl
    {
    public:
        /**
           Construct default Logger
         */
        LoggerImpl();
        /**
           Construct logger with a name that belongs in 1 log channel
         */
        LoggerImpl(const std::string & name, const std::string & channel);
        LogStream & Debug() { return m_Debug.Flush(); } 
        LogStream & Info() { return m_Info.Flush(); }
        LogStream & Warning() { return m_Warn.Flush(); }
        LogStream & Error() { return m_Error.Flush(); }
        EventStream & UI() { return m_Events; }
        log_t log;
    private:
        LogStream m_Debug, m_Info, m_Warn, m_Error;
        BoostEventStream m_Events;
    };

    class LogImpl
    {
    public:
        LogImpl(LogLevel minLevel, std::ostream * out);
        LogImpl() : LogImpl(eLogWarning, &std::clog) {}
        void Flush();
    private:
        backend_ptr m_LogBackend;
        core_ptr m_LogCore;
        static void Format(boost::log::record_view const & rec, boost::log::formatting_ostream &s);
    };
}
}

#endif

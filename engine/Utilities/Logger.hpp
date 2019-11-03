//
// Created by Marcin Praski on 31.10.19.
//

#ifndef TYPEWRITER_GAEM_LOGGER_HPP
#define TYPEWRITER_GAEM_LOGGER_HPP


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/core/null_deleter.hpp>
#include "General.hpp"

namespace engine {
namespace logging = boost::log;
namespace sinks = logging::sinks;
namespace keywords = logging::keywords;
namespace expr = logging::expressions;
namespace src = logging::sources;

class Logger {
public:
    template<typename ... Ts>
    void debug(Ts &&... ts) {
      log<logging::trivial::debug>(std::forward<Ts>(ts)...);
    }

    template<typename ... Ts>
    void info(Ts &&... ts) {
      log<logging::trivial::info>(std::forward<Ts>(ts)...);
    }

    template<typename ... Ts>
    void error(Ts &&... ts) {
      log<logging::trivial::error>(std::forward<Ts>(ts)...);
    }

private:
    friend class System;

    Logger();

    static void addStreamSink();

    static void addFileSink();

    template<logging::trivial::severity_level L, typename ... Ts>
    void log(Ts &&... ts) {
      if (logging::record rec = mLogger.open_record(keywords::severity = L); rec) {
        logging::record_ostream ros(rec);
        gen::to_stream(ros, std::forward<Ts>(ts)...);

        ros.flush();

        logging::core::get()->push_record(boost::move(rec));
      }
    }

private:
    std::once_flag mSetupFlag;
    src::severity_logger<logging::trivial::severity_level> mLogger;
};
}

#endif //TYPEWRITER_GAEM_LOGGER_HPP

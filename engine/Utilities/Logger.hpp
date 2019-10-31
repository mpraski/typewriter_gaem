//
// Created by Marcin Praski on 31.10.19.
//

#ifndef TYPEWRITER_GAEM_LOGGER_HPP
#define TYPEWRITER_GAEM_LOGGER_HPP


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/core/null_deleter.hpp>
#include "General.hpp"

namespace engine {
namespace logging = boost::log;
namespace sinks = logging::sinks;
namespace keywords = logging::keywords;

class Logger {
public:
    template<typename ... Ts>
    void log(Ts &&... ts) {
      logging::record rec = core->open_record(logging::attribute_set{});
      if (rec) {
        logging::record_ostream ros(rec);
        gen::to_stream(ros, std::forward<Ts>(ts)...);

        ros.flush();

        core->push_record(boost::move(rec));
      }
    }

private:
    friend class System;

    Logger();

    void addStreamSink();

    void addFileSink();

private:
    boost::shared_ptr<logging::core> core;
};
}

#endif //TYPEWRITER_GAEM_LOGGER_HPP

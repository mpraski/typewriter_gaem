//
// Created by Marcin Praski on 31.10.19.
//

#include "Logger.hpp"

engine::Logger::Logger() : core{logging::core::get()} {
  addStreamSink();
#ifdef DEBUG
  addFileSink();
#endif
  core->add_global_attribute("LineID", logging::attributes::counter<unsigned int>(1));
  core->add_global_attribute("TimeStamp", logging::attributes::local_clock());
}

void engine::Logger::addStreamSink() {
// Create a backend and attach a couple of streams to it
  auto backend{boost::make_shared<sinks::text_ostream_backend>()};
  backend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

  // Enable auto-flushing after each log record written
  backend->auto_flush(true);

  // Wrap it into the frontend and register in the core.
  // The backend requires synchronization in the frontend.
  typedef sinks::synchronous_sink<sinks::text_ostream_backend> sink_t;
  boost::shared_ptr<sink_t> sink(new sink_t(backend));
  core->add_sink(sink);
}

void engine::Logger::addFileSink() {
  auto backend{
      boost::make_shared<sinks::text_file_backend>(
          keywords::file_name = "engine_%5N.log",
          keywords::rotation_size = 5 * 1024 * 1024,
          keywords::time_based_rotation = sinks::file::rotation_at_time_point(12, 0, 0)
      )
  };

  // Wrap it into the frontend and register in the core.
  // The backend requires synchronization in the frontend.
  typedef sinks::synchronous_sink<sinks::text_file_backend> sink_t;
  boost::shared_ptr<sink_t> sink(new sink_t(backend));

  core->add_sink(sink);
}
/*
 * Copyright (c) 2015, Formax Inc. All rights reserved.
 */
#ifndef COMMON_TIME_COST_TIME_COST_H
#define COMMON_TIME_COST_TIME_COST_H

#include <time.h>
#include <chrono>


#include <string>

#include "boost/date_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/date_time/time_duration.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "thirdparty/boost/date_time/posix_time/ptime.hpp"

namespace formax { namespace common {
// timestamp
int64_t CurUTCTimeStamp();
int64_t CurUTCTimeStampSeconds();
int64_t TimestampAfterDays(int32_t days_after);
int64_t TimestampAfterMonths(int32_t months_after);
int64_t NowUTCTick();
void TimestampToTime(std::string& str, const int64_t&);
void TimestampToTime(std::string& str, const int64_t& tm, const char *pFormat);
std::string CurUTCTimeStr();
int64_t TimeToTimestamp(const std::string& str);
int64_t ZoneToUTCSeconds(const int64_t& zone_seconds,
  const int32_t& zone_diff_hour);
int64_t UTCToZoneSeconds(const int64_t& utc_seconds,
  const int32_t& zone_diff_hour);


// convert date string to utc time stamp, which is expressed in milliseconds.
// [Parameters]:
//  :param str:
//    the date time in string
//    [Notes] you need to make sure the date string is in the following format:
//       2015-10-25 12:34:00.222
//    or 2015-10-25 12:34:00
//    or 2015-10-25 12:34
//    or 2015-10-25
//    or 20151025 12:34:00.222
//    or 20151025 12:34:00
//    or 20151025 12:34
//    or 20151025
//    or 20151025-12:34:00.222
//    or 20151025-12:34
//
// [Return value]:
//  return a utc timestamp in milliseconds
int64_t DateStringToUTCTimeStamp(const std::string& str);


/*{
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  boost::posix_time::time_duration t = now - epoch;
  return t.total_milliseconds();
}
*/
struct TimeCost {
  explicit TimeCost(const std::string& tip) : m_tip(tip) {
    time_start = CurUTCTimeStamp();
    VLOG(20) << m_tip << " start at: " << time_start;
  }
  ~TimeCost() {
    int64_t time_end = CurUTCTimeStamp();
    VLOG(20) << m_tip << " cost: " << time_end - time_start << " start: " << time_start;
  }
 private:
  int64_t time_start;
  std::string m_tip;
};

struct TimeCostLevel {
  explicit TimeCostLevel(const std::string& tip, const int level) : m_tip(tip), m_level(level) {
    time_start = CurUTCTimeStamp();
    VLOG(m_level) << m_tip << " start at: " << time_start;
  }
  ~TimeCostLevel() {
    int64_t time_end = CurUTCTimeStamp();
    VLOG(m_level) << m_tip << " cost: " << time_end - time_start << " start: " << time_start;
  }
 private:
  int64_t time_start;
  std::string m_tip;
  int32_t m_level;
};

template <int LEVEL>
class TimeCostWithLevel {
 public:
  explicit TimeCostWithLevel(const std::string& tip) : m_tip(tip) {
    time_start = CurUTCTimeStamp();
    VLOG(LEVEL) << m_tip << "  start at: " << time_start;
  }

  ~TimeCostWithLevel() {
    int64_t time_end = CurUTCTimeStamp();
    VLOG(LEVEL) << m_tip << " cost: " << time_end - time_start << " start: "
      << time_start;
  }

  int64_t CurrentCost() {
    return CurUTCTimeStamp() - time_start;
  }

 private:
  int64_t time_start;
  std::string m_tip;
};

class TimeCostLogIf {
 public:
  TimeCostLogIf(const std::string& severity, int32_t limitation_ms,
    const std::string& tip = "")
    : m_severity(severity), m_limitation_ms(limitation_ms), m_tip(tip) {
    m_begin = std::chrono::steady_clock::now();
  }

  ~TimeCostLogIf() {
    int64_t timecost = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::steady_clock::now() - m_begin).count();
    if (m_severity == "INFO") {
      LOG_IF(INFO, timecost >= m_limitation_ms) << "timecost:"
        << timecost << "ms > limitation:" << m_limitation_ms << "ms. " << m_tip;
    } else if (m_severity == "WARNING") {
      LOG_IF(WARNING, timecost >= m_limitation_ms) << "timecost:"
        << timecost << "ms > limitation:" << m_limitation_ms << "ms. " << m_tip;
    } else if (m_severity == "ERROR") {
      LOG_IF(ERROR, timecost >= m_limitation_ms) << "timecost:"
        << timecost << "ms > limitation:" << m_limitation_ms << "ms. " << m_tip;
    } else if (m_severity == "FATAL") {
      LOG_IF(FATAL, timecost >= m_limitation_ms) << "timecost:"
        << timecost << "ms > limitation:" << m_limitation_ms << "ms. " << m_tip;
    } else {
      LOG(ERROR) << "unknown m_severity[" << m_severity << "]";
      LOG_IF(ERROR, timecost >= m_limitation_ms) << "timecost:"
        << timecost << "ms > limitation:" << m_limitation_ms << "ms. " << m_tip;
    }
  }

 private:
  const std::string m_severity;
  const int32_t m_limitation_ms;
  const std::string m_tip;
  std::chrono::steady_clock::time_point m_begin;
};
#define TIMECOST_LOGIF(severity, limitation_ms, tip) \
  TimeCostLogIf tc9823((#severity), (limitation_ms), (tip));

}  // namespace common
}  // namespace formax
#endif  // COMMON_TIME_COST_TIME_COST_H

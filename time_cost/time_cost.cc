//
#include "time_cost.h"

#include <algorithm>

namespace formax { namespace common {

using boost::gregorian::date;
using namespace boost::posix_time;
using namespace boost::gregorian;

int64_t CurUTCTimeStamp()
{
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  boost::posix_time::time_duration t = now - epoch;
  return t.total_milliseconds();
}

int64_t TimestampAfterDays(int32_t days_after) {
  date today(day_clock::local_day());
  ptime end_time(today + days(days_after));

  ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  time_duration time_period = end_time - epoch;
  return time_period.total_seconds();
}

int64_t TimestampAfterMonths(int32_t months_after) {
  date today(day_clock::local_day());
  ptime end_time(today + months(months_after));

  ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  time_duration time_period = end_time - epoch;
  return time_period.total_seconds();
}

int64_t CurUTCTimeStampSeconds() {
  return CurUTCTimeStamp() / 1000;
}

int64_t NowUTCTick() {
  time_t tick = time(static_cast<time_t*>(0));
  return tick;
}

void TimestampToTime(std::string& str, const int64_t& timestamp, const char *pformat) {
  struct tm utc_time;
  memset(&utc_time, 0, sizeof(struct tm));
  char str_buf[100] = {'\0'};
  utc_time = *gmtime(&timestamp);
  strftime(str_buf, sizeof(str_buf), pformat, &utc_time);
  str = str_buf;
}

void TimestampToTime(std::string& str, const int64_t& timestamp) {
  const char *pformat = "%Y-%m-%d %H:%M:%S";
  TimestampToTime(str, timestamp, pformat);
}

std::string CurUTCTimeStr() {
  std::string time_str;
  int64_t cur_timestamp = CurUTCTimeStampSeconds();
  TimestampToTime(time_str, cur_timestamp);
  return time_str;
}

/*
   int64_t UTCTimeToTimestamp(const std::string& str) {
   ptime utc_tick(time_from_string(str));
   ptime epoch(time_from_string("1970-1-1 00:00:00.000"));
   time_duration t = utc_tick - epoch;
   return t.total_seconds();
   }
   */

int64_t TimeToTimestamp(const std::string& str) {
  struct tm utc_time;
  memset(&utc_time, 0, sizeof(struct tm));
  const char *pFormat = "%Y-%m-%d %H:%M:%S";
  struct timezone tz;
  memset(&tz, 0, sizeof(struct timezone));
  struct timeval tv;
  memset(&tv, 0, sizeof(struct timeval));
  gettimeofday(&tv, &tz);
  strptime(str.c_str(), pFormat, &utc_time);
  return mktime(&utc_time) - (tz.tz_minuteswest * 60);
}

int64_t ZoneToUTCSeconds(const int64_t& zone_seconds,
  const int32_t& zone_diff_hour) {
  return zone_seconds - (zone_diff_hour * 60 * 60);
}

int64_t UTCToZoneSeconds(const int64_t& utc_seconds,
  const int32_t& zone_diff_hour) {
  return utc_seconds + (zone_diff_hour * 60 * 60);
}


int64_t DateStringToUTCTimeStamp(const std::string& str) {
  if (str.empty()) {
    LOG(ERROR) << "invalid date string for conversion.";
    return -1;
  }

  // format string into "%Y-%m-%d %H:%M:%S"
  char buf[20] = { 0 };
  if (str.find(' ') == 8 || str.find('-') == 8 || str.length() == 8) {
    // transfer "20151025 12:34:00.222" into "2015-10-25 12:34:00.222"
    // transfer "20151025-12:34:00.222" into "2015-10-25 12:34:00.222"
    //
    // add two dash line after year and month
    size_t j = 0;
    for (size_t i = 0; i < 19; ++i) {
      if (i == 4 || i == 7) {
        buf[i] = '-';
      } else {
        buf[i] = str[j];
        ++j;
      }
    }
  } else {
    std::copy(str.begin(), str.end(), buf);
  }

  // transfer "2015-10-25-12:34:00.222" into "2015-10-25 12:34:00.222"
  // transfer "2015-10-25.12:34:00.222" into "2015-10-25 12:34:00.222"
  //
  // 10 is the length of string "2015-10-25"
  if (strlen(buf) > 10) {
    // if ('-' == buf[10] || '.' == buf[10]) {
      buf[10] = ' ';
    // }
  }

  // convert string to struct tm
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));
  strptime(buf, "%Y-%m-%d %H:%M:%S", &tm);

  // convert struct tm to time_t, which represents senconds since
  // 00:00 1 Jan, 1970 UTC
  time_t tt = mktime(&tm);

  boost::posix_time::ptime pt = boost::posix_time::from_time_t(tt);
  boost::posix_time::ptime
    epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  boost::posix_time::time_duration t = pt - epoch;
  return t.total_milliseconds();
}
}  // namespace common
}  // namespace formax

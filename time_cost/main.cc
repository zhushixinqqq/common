#include <iostream>

#include "common/time_cost/time_cost.h"


int main() {
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  boost::posix_time::time_duration t = now - epoch;
  int64_t ut = t.total_milliseconds() / 1000;
  std::cout << "universal time stamp is: " << ut << std::endl;

  int64_t nt = formax::common::NowUTCTick();
  std::cout << "NowUTCTick is:" << nt << std::endl;
  std::cout << "local tick is:" << formax::common::CurUTCTimeStampSeconds() << std::endl;
  std::string str1;

  formax::common::TimestampToTime(str1, nt);
  std::cout << "utc time is:" << str1 << std::endl;

  int64_t bt = formax::common::UTCToZoneSeconds(nt, 8);
  std::cout << "utc seconds " << nt << "--" << "beijing seconds:" << bt << std::endl;
  std::string str2;
  formax::common::TimestampToTime(str2, bt);
  std::cout << "beijing time is:" << str2 << std::endl;

  int64_t ct = formax::common::TimeToTimestamp(str2);
  int64_t uut = formax::common::ZoneToUTCSeconds(ct, 8);
  std::cout << "beijing seconds:" << ct << "--" << "utc seconds:" << uut << std::endl;
  std::string str3;
  formax::common::TimestampToTime(str3, ct);
  std::cout << "beijing time::" << str3 << std::endl;
  std::string str4;
  formax::common::TimestampToTime(str4, uut);
  std::cout << "utc time:" << str4 << std::endl;

  std::string date_str = "20151023 13:24:23.001";
  int64_t millisecs = formax::common::DateStringToUTCTimeStamp(date_str);
  std::cout << "utc timestamp of date[" << date_str << "] is [" << millisecs
    << "]." << std::endl;


  int64_t ts_01 = formax::common::TimeToTimestamp(date_str);
  std::cout << "utc timestamp of date[" << date_str << "] is [" << ts_01
    << "]." << std::endl;



  return 0;
}


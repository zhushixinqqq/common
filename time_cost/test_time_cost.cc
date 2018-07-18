/*
 * Copyright [2013-2015] <zhushixinq@qq.com>"  [legal/copyright]
 *
 * file: order_book_maintainer_test.cc
 * author:  zhushixinq@formaxmarket.com
 *
 */
#include <iostream>
#include <cassert>

#include "common/time_cost/time_cost.h"
#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gtest/gtest.h"

#define NDEBUG

TEST(TestTimeCost, DateStringToTimeStamp) {
  std::string date_str = "20151023 13:24:23.001";
  int64_t millisecs = formax::common::DateStringToUTCTimeStamp(date_str);
  std::cout << "utc timestamp of date[" << date_str << "] is [" << millisecs
    << "]." << std::endl;
  CHECK_EQ(millisecs, 1445606663000);

  std::string date_str_01 = "2015-10-23 13:24:23.001";
  int64_t millisecs_01 = formax::common::DateStringToUTCTimeStamp(date_str_01);
  std::cout << "utc timestamp of date[" << date_str_01 << "] is ["
    << millisecs_01 << "]." << std::endl;
  CHECK_EQ(millisecs, 1445606663000);


  date_str_01 = "2015-10-23 13:24";
  millisecs_01 = formax::common::DateStringToUTCTimeStamp(date_str_01);
  std::cout << "utc timestamp of date[" << date_str_01 << "] is ["
    << millisecs_01 << "]." << std::endl;
  CHECK_EQ(millisecs_01, 1445606640000);

  date_str_01 = "2015-10-23";
  millisecs_01 = formax::common::DateStringToUTCTimeStamp(date_str_01);
  std::cout << "utc timestamp of date[" << date_str_01 << "] is ["
    << millisecs_01 << "]." << std::endl;
  CHECK_EQ(millisecs_01, 1445558400000);

  date_str_01 = "20151023 13:24";
  millisecs_01 = formax::common::DateStringToUTCTimeStamp(date_str_01);
  std::cout << "utc timestamp of date[" << date_str_01 << "] is ["
    << millisecs_01 << "]." << std::endl;

  CHECK_EQ(millisecs_01, 1445606640000);

  date_str_01 = "20151023";
  millisecs_01 = formax::common::DateStringToUTCTimeStamp(date_str_01);
  std::cout << "utc timestamp of date[" << date_str_01 << "] is ["
    << millisecs_01 << "]." << std::endl;
  CHECK_EQ(millisecs_01, 1445558400000);


  date_str_01 = "20151029-10:01:04.669";
  millisecs_01 = formax::common::DateStringToUTCTimeStamp(date_str_01);
  std::cout << "utc timestamp of date[" << date_str_01 << "] is ["
    << millisecs_01 << "]." << std::endl;
}


int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

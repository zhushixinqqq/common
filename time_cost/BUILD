cc_library (
  name = "time_cost",
  srcs = [
  "time_cost.cc",
  ],
  deps = [
    "//thirdparty/glog:glog",
    "//thirdparty/gflags:gflags",
  ],
)

cc_binary (
  name = "test",
  srcs = [
  "main.cc",
  "time_cost.cc",
  ],
  deps = [
    "//thirdparty/glog:glog",
    "//thirdparty/gflags:gflags",
    "#pthread",
  ],
)

cc_binary (
  name = "test_time_cost",
  srcs = [
  "test_time_cost.cc",
  "time_cost.cc",
  ],
  deps = [
    "//thirdparty/glog:glog",
    "//thirdparty/gflags:gflags",
    '//thirdparty/gtest:gtest',
    "#pthread",
  ],
)

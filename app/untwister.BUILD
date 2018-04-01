cc_library(
    name = "php-mt_rand",
    srcs = [
        "ConsoleColors.h",
        "prngs/PHP_mt19937.cpp",
    ],
    hdrs = [
        "prngs/PHP_mt19937.h",
    ],
    visibility = ["//visibility:public"],
    deps = [":prng"],
)

cc_library(
    name = "mt_rand",
    srcs = [
        "ConsoleColors.h",
        "prngs/Mt19937.cpp",
    ],
    hdrs = [
        "prngs/Mt19937.h",
    ],
    visibility = ["//visibility:public"],
    deps = [":prng"],
)

cc_library(
    name = "prng",
    srcs = [
        "prngs/PRNG.h",
    ],
)

cc_library(
    name = "php-mt_rand",
    srcs = [
        "ConsoleColors.h",
        "prngs/PHP_mt19937.cpp",
        "prngs/PHP_mt19937.h",
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

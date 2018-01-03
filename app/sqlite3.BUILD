cc_binary(
    name = "lemon",
    srcs = ["tool/lemon.c"],
)

COMPILER_OPTS = [
    "-DNDEBUG=1",
    "-DHAVE_FDATASYNC=1",
]

cc_library(
    name = "sqlite3",
    srcs = [
        "shell.c",
        "sqlite3.c",
    ],
    hdrs = [
        "sqlite3.h",
        "sqlite3ext.h",
    ],
    copts = ["-DSQLITE_CORE"] + COMPILER_OPTS,
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "makeseed",
    srcs = ["makeseed.cc"],
    deps = [
        ":arraylength",
        ":items",
        ":locations",
        ":mt_rand",
        ":world",
        "@sqlite3//:sqlite3",
    ],
)

cc_library(
    name = "mt_rand",
    srcs = [
        "mt_rand.cc",
        "mt_rand.h",
    ],
    deps = ["@untwister//:php-mt_rand"],
)

cc_library(
    name = "arraylength",
    srcs = ["arraylength.h"],
)

cc_library(
    name = "locations",
    srcs = [
        "locations.h",
    ],
    deps = [":arraylength"],
)

cc_library(
    name = "items",
    srcs = [
        "items.h",
    ],
    deps = [
        ":arraylength",
    ],
)

cc_library(
    name = "world",
    srcs = [
        "world.cc",
        "world.gen.cc",
        "world.h",
    ],
    deps = [
        ":items",
        ":locations",
        "@sqlite3//:sqlite3",
    ],
)

py_binary(
    name = "build_index",
    srcs = ["build_index.py"],
    data = [
        "world.cc",
    ],
    deps = [":php_grammar"],
)

py_library(
    name = "php_grammar",
    srcs = ["php_grammar.py"],
)

genrule(
    name = "world-generated",
    srcs = glob(["Region/**/*.php"]) + ["Support/ItemCollection.php"],
    outs = ["world.gen.cc"],
    cmd = "$(location :build_index) | clang-format > $(OUTS)",
    tools = [
        "world_template.cc",
        ":build_index",
    ],
)

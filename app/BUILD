cc_binary(
    name = "makeseed",
    srcs = ["makeseed.cc"],
    deps = [
        ":item_groups",
        ":items",
        ":locations",
        ":mt_rand",
        ":world",
    ],
)

cc_library(
    name = "mt_rand",
    srcs = [
        "mt_rand.cc",
        "mt_rand.h",
    ],
)

cc_library(
    name = "locations",
    srcs = [
        "locations.h",
    ],
)

cc_library(
    name = "items",
    srcs = [
        "items.h",
    ],
)

cc_library(
    name = "item_groups",
    srcs = [
        "item_groups.cc",
    ],
    deps = [":items"],
)

cc_library(
    name = "world",
    srcs = [
        "world.gen.cc",
        "world.h",
    ],
    deps = [
        ":items",
        ":locations",
    ],
)

py_binary(
    name = "build_index",
    srcs = ["build_index.py"],
    data = [
        "items.txt",
        "locations.txt",
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
    srcs = glob(["**/*.php"]),
    outs = ["world.gen.cc"],
    cmd = "$(location :build_index) > $(OUTS)",
    tools = [
        "items.txt",
        "locations.txt",
        "world.cc",
        ":build_index",
    ],
)

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
        "world.cc",
        "world.h",
    ],
    deps = [
        ":items",
        ":locations",
    ],
)

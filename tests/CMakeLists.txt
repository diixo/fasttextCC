
include_directories(
    "../src"
)

add_executable(test-dictionary dictionary-main.cc)

target_link_libraries(test-dictionary fasttext-static)

# Applies the project's warning/hardening policy to a target.
function(nova_configure_target target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    if(NOVA_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE /WX)
    endif()
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Woverloaded-virtual
      -Wconversion -Wsign-conversion -Wold-style-cast -Wnull-dereference)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      # Qt Test's QTEST_* macros trigger this under -Wpedantic with Clang/AppleClang.
      target_compile_options(${target} PRIVATE -Wno-gnu-zero-variadic-macro-arguments)
    endif()
    if(NOVA_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE -Werror)
    endif()
    if(NOVA_SANITIZE)
      target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
      target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
  endif()
endfunction()

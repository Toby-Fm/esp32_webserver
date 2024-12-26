# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/toby/Downloads/repos/esp-idf/components/bootloader/subproject"
  "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader"
  "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader-prefix"
  "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader-prefix/tmp"
  "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader-prefix/src/bootloader-stamp"
  "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader-prefix/src"
  "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/toby/Dokumente/programming/cpp/esp32/test/pio_test/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()

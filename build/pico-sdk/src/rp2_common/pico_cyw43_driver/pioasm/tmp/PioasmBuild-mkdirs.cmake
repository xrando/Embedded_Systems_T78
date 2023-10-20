# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/pico/pico-sdk/tools/pioasm"
  "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pioasm"
  "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/seankzw/Desktop/SIT/year_2/y2t1/inf2004-esp/Embedded_Systems_T78/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()

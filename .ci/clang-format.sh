#!/bin/sh

find QDash -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=file:QDash/.clang-format

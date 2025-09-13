# SPDX-FileCopyrightText: 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

include(GetGitRevisionDescription)

function(trim var)
  string(REGEX REPLACE "\n" "" new "${${var}}")
  set(${var} ${new} PARENT_SCOPE)
endfunction()

set(TAG_FILE ${CMAKE_SOURCE_DIR}/GIT-TAG)
set(REF_FILE ${CMAKE_SOURCE_DIR}/GIT-REFSPEC)
set(COMMIT_FILE ${CMAKE_SOURCE_DIR}/GIT-COMMIT)

if (EXISTS ${REF_FILE} AND EXISTS ${COMMIT_FILE})
  file(READ ${CMAKE_SOURCE_DIR}/GIT-REFSPEC QDASH_REFSPEC)
  file(READ ${CMAKE_SOURCE_DIR}/GIT-COMMIT QDASH_COMMIT)
else()
  get_git_head_revision(QDASH_REFSPEC QDASH_COMMIT)
  git_branch_name(QDASH_REFSPEC)
  if (QDASH_REFSPEC MATCHES "NOTFOUND")
    set(QDASH_REFSPEC 1.0.0)
    set(QDASH_COMMIT stable)
  endif()
endif()

if (EXISTS ${TAG_FILE})
  file(READ ${TAG_FILE} QDASH_TAG)
else()
  git_describe(QDASH_TAG --tags --abbrev=0)
  if (QDASH_TAG MATCHES "NOTFOUND")
    set(QDASH_TAG "${QDASH_REFSPEC}")
  endif()
endif()

trim(QDASH_REFSPEC)
trim(QDASH_COMMIT)
trim(QDASH_TAG)

message(STATUS "Git commit: ${QDASH_COMMIT}")
message(STATUS "Git tag: ${QDASH_TAG}")
message(STATUS "Git refspec: ${QDASH_REFSPEC}")

string(REPLACE "-beta" "." QDASH_NUMERIC_TAG ${QDASH_TAG})
string(REPLACE "-rc" "." QDASH_NUMERIC_TAG ${QDASH_NUMERIC_TAG})

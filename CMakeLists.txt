# **********************************************************
# Copyright (c) 2012 Google, Inc.    All rights reserved.
# **********************************************************

cmake_minimum_required(VERSION 2.6)

project(countingexample)

# Boiler plate to find DynamoRIO.  Invoke cmake like this to configure:
# cmake . -DDynamoRIO_DIR=DynamoRIO-Windows-3.2.0-3/cmake
if (NOT DEFINED DynamoRIO_DIR)
  set(DynamoRIO_DIR "${PROJECT_SOURCE_DIR}/../cmake" CACHE PATH
    "DynamoRIO installation's cmake directory")
endif (NOT DEFINED DynamoRIO_DIR)
find_package(DynamoRIO 3.1)
if (NOT DynamoRIO_FOUND)
  message(FATAL_ERROR "DynamoRIO package required to build")
endif(NOT DynamoRIO_FOUND)

# Make some shared library CMake targets and configure them to be DR clients.
add_library(bbcount SHARED bbcount.c)
configure_DynamoRIO_client(bbcount)

add_library(tracecount SHARED tracecount.c)
configure_DynamoRIO_client(tracecount)

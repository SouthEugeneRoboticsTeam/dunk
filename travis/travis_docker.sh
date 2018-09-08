#!/bin/bash
set -e   # Make sure any error makes the script to return an error code

export STAGE_INPUT_IMAGE=sert/dunk:latest

docker run -v $HOME:$HOME $CODECOV_VARS -e CC -e BUILD_TARGET -e STAGE -e BUILD_TYPE -e MAKEFLAGS -e CCACHE_SLOPPINESS -e TASK -e DEPS -e CI_SOURCE_PATH -e REPOSITORY_NAME -e HOME -e DISTRO -e CI_ROS_DISTRO -e CODECOV_TOKEN --name mrptbuild $STAGE_INPUT_IMAGE bash -c 'cd $CI_SOURCE_PATH; apt-get update -qq; source travis/travis_main.sh'

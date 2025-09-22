# Copyright 1996-2023 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Webots Makefile system 
#
# You may add some variable definitions hereafter to customize the build process
# See documentation in $(WEBOTS_HOME_PATH)/resources/Makefile.include

include config.include

# Do not modify the following: this includes Webots global Makefile.include
INCLUDE = -IJavinoInC/include
# LIBRARIES =

JavinoInC/src/javino.c:
	git clone https://github.com/chon-group/JavinoInC.git

# C_SOURCES = my_controller.c my_second_file.c my_third_file.c
C_SOURCES = JavinoInC/src/javino.c four_wheels_collision_avoidance.c

CFLAGS = $(USE_BATTERY) $(USE_GPS)

null :=
space := $(null) $(null)
WEBOTS_HOME_PATH?=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
include $(WEBOTS_HOME_PATH)/resources/Makefile.include

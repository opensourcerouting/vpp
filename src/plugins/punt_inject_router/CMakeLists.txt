
# Copyright (c) <current-year> <your-organization>
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

add_vpp_plugin(punt_inject_router
  SOURCES
  punt_inject_router.c
  node.c
  punt_inject_router_periodic.c
  tap_inject.c
  tap_inject_node.c
  tap_inject_tap.c
  punt_inject_router.h
  tap_inject.h
  

  MULTIARCH_SOURCES
  node.c

  API_FILES
  punt_inject_router.api

  API_TEST_SOURCES
  punt_inject_router_test.c
)

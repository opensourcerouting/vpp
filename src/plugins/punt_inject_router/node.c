/*
 * node.c - skeleton vpp engine plug-in dual-loop node skeleton
 *
 * Copyright (c) <current-year> <your-organization>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <vlib/vlib.h>
#include <vnet/vnet.h>
#include <vnet/pg/pg.h>
#include <vppinfra/error.h>
#include <punt_inject_router/punt_inject_router.h>


/* *INDENT-OFF* */
#ifndef CLIB_MARCH_VARIANT


VLIB_REGISTER_NODE (tap_inject_tx_node) = {
  .function = tap_inject_tx,
  .name = "tap-inject-tx",
  .vector_size = sizeof (u32),
  .type = VLIB_NODE_TYPE_INTERNAL,
};

VLIB_REGISTER_NODE (tap_inject_neighbor_node) = {
  .function = tap_inject_neighbor,
  .name = "tap-inject-neighbor",
  .vector_size = sizeof (u32),
  .type = VLIB_NODE_TYPE_INTERNAL,
  .n_next_nodes = 2,
  .next_nodes = {
    [NEXT_NEIGHBOR_ARP] = "arp-input",
    [NEXT_NEIGHBOR_ICMP6] = "icmp6-neighbor-solicitation",
  },
};

VLIB_REGISTER_NODE (tap_inject_rx_node) = {
  .function = tap_inject_rx,
  .name = "tap-inject-rx",
  .type = VLIB_NODE_TYPE_INPUT,
  .state = VLIB_NODE_STATE_INTERRUPT,
  .vector_size = sizeof (u32),
};


#endif /* CLIB_MARCH_VARIANT */
/* *INDENT-ON* */
/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */

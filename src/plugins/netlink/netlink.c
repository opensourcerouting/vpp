/*
 * netlink.c - skeleton vpp engine plug-in
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

#include <vnet/vnet.h>
#include <vnet/plugin/plugin.h>
#include <netlink/netlink.h>

#include <vlibapi/api.h>
#include <vlibmemory/api.h>
#include <vpp/app/version.h>
#include <stdbool.h>

#include <netlink/netlink.api_enum.h>
#include <netlink/netlink.api_types.h>

#define REPLY_MSG_ID_BASE nmp->msg_id_base
#include <vlibapi/api_helper_macros.h>

netlink_main_t netlink_main;

/* Action function shared between message handler and debug CLI */

int netlink_enable_disable (netlink_main_t * nmp, u32 sw_if_index,
                                   int enable_disable)
{
  vnet_sw_interface_t * sw;
  int rv = 0;

  /* Utterly wrong? */
  if (pool_is_free_index (nmp->vnet_main->interface_main.sw_interfaces,
                          sw_if_index))
    return VNET_API_ERROR_INVALID_SW_IF_INDEX;

  /* Not a physical port? */
  sw = vnet_get_sw_interface (nmp->vnet_main, sw_if_index);
  if (sw->type != VNET_SW_INTERFACE_TYPE_HARDWARE)
    return VNET_API_ERROR_INVALID_SW_IF_INDEX;

  netlink_create_periodic_process (nmp);

  vnet_feature_enable_disable ("device-input", "netlink",
                               sw_if_index, enable_disable, 0, 0);

  /* Send an event to enable/disable the periodic scanner process */
  vlib_process_signal_event (nmp->vlib_main,
                             nmp->periodic_node_index,
                             NETLINK_EVENT_PERIODIC_ENABLE_DISABLE,
                            (uword)enable_disable);
  return rv;
}

static clib_error_t *
netlink_enable_disable_command_fn (vlib_main_t * vm,
                                   unformat_input_t * input,
                                   vlib_cli_command_t * cmd)
{
  netlink_main_t * nmp = &netlink_main;
  u32 sw_if_index = ~0;
  int enable_disable = 1;

  int rv;

  while (unformat_check_input (input) != UNFORMAT_END_OF_INPUT)
    {
      if (unformat (input, "disable"))
        enable_disable = 0;
      else if (unformat (input, "%U", unformat_vnet_sw_interface,
                         nmp->vnet_main, &sw_if_index))
        ;
      else
        break;
  }

  if (sw_if_index == ~0)
    return clib_error_return (0, "Please specify an interface...");

  rv = netlink_enable_disable (nmp, sw_if_index, enable_disable);

  switch(rv)
    {
  case 0:
    break;

  case VNET_API_ERROR_INVALID_SW_IF_INDEX:
    return clib_error_return
      (0, "Invalid interface, only works on physical ports");
    break;

  case VNET_API_ERROR_UNIMPLEMENTED:
    return clib_error_return (0, "Device driver doesn't support redirection");
    break;

  default:
    return clib_error_return (0, "netlink_enable_disable returned %d",
                              rv);
    }
  return 0;
}

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (netlink_enable_disable_command, static) =
{
  .path = "netlink enable-disable",
  .short_help =
  "netlink enable-disable <interface-name> [disable]",
  .function = netlink_enable_disable_command_fn,
};
/* *INDENT-ON* */

/* API message handler */
static void vl_api_netlink_enable_disable_t_handler
(vl_api_netlink_enable_disable_t * mp)
{
  vl_api_netlink_enable_disable_reply_t * rmp;
  netlink_main_t * nmp = &netlink_main;
  int rv;

  rv = netlink_enable_disable (nmp, ntohl(mp->sw_if_index),
                                      (int) (mp->enable_disable));

  REPLY_MACRO(VL_API_NETLINK_ENABLE_DISABLE_REPLY);
}

/* API definitions */
#include <netlink/netlink.api.c>

static clib_error_t * netlink_init (vlib_main_t * vm)
{
  netlink_main_t * nmp = &netlink_main;
  clib_error_t * error = 0;

  nmp->vlib_main = vm;
  nmp->vnet_main = vnet_get_main();

  /* Add our API messages to the global name_crc hash table */
  nmp->msg_id_base = setup_message_id_table ();

  return error;
}

VLIB_INIT_FUNCTION (netlink_init);

/* *INDENT-OFF* */
VNET_FEATURE_INIT (netlink, static) =
{
  .arc_name = "device-input",
  .node_name = "netlink",
  .runs_before = VNET_FEATURES ("ethernet-input"),
};
/* *INDENT-ON */

/* *INDENT-OFF* */
VLIB_PLUGIN_REGISTER () =
{
  .version = VPP_BUILD_VER,
  .description = "netlink plugin description goes here",
};
/* *INDENT-ON* */

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */

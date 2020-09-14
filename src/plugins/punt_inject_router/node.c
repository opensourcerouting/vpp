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

typedef struct
{
  u32 sw_if_index;
  u32 next_index;
} punt_inject_router_trace_t;

#ifndef CLIB_MARCH_VARIANT

/* packet trace format function */
static u8 * format_punt_inject_router_trace (u8 * s, va_list * args)
{
  CLIB_UNUSED (vlib_main_t * vm) = va_arg (*args, vlib_main_t *);
  CLIB_UNUSED (vlib_node_t * node) = va_arg (*args, vlib_node_t *);
  punt_inject_router_trace_t * t = va_arg (*args, punt_inject_router_trace_t *);
  
  s = format (s, "PUNT_INJECT_ROUTER: sw_if_index %d, next index %d\n",
              t->sw_if_index, t->next_index);
  return s;
}

vlib_node_registration_t punt_inject_router_node;

#endif /* CLIB_MARCH_VARIANT */

#define foreach_punt_inject_router_error \
_(SWAPPED, "Mac swap packets processed")

typedef enum 
{
#define _(sym,str) PUNT_INJECT_ROUTER_ERROR_##sym,
  foreach_punt_inject_router_error
#undef _
  PUNT_INJECT_ROUTER_N_ERROR,
} punt_inject_router_error_t;

#ifndef CLIB_MARCH_VARIANT
static char * punt_inject_router_error_strings[] = 
{
#define _(sym,string) string,
  foreach_punt_inject_router_error
#undef _
};
#endif /* CLIB_MARCH_VARIANT */

typedef enum 
{
  PUNT_INJECT_ROUTER_NEXT_DROP,
  PUNT_INJECT_ROUTER_N_NEXT,
} punt_inject_router_next_t;

always_inline uword
punt_inject_router_inline (vlib_main_t * vm,
     		 vlib_node_runtime_t * node, vlib_frame_t * frame,
		 int is_ip4, int is_trace)
{
  u32 n_left_from, *from;
  vlib_buffer_t *bufs[VLIB_FRAME_SIZE], **b;
  u16 nexts[VLIB_FRAME_SIZE], *next;

  from = vlib_frame_vector_args (frame);
  n_left_from = frame->n_vectors;

  vlib_get_buffers (vm, from, bufs, n_left_from);
  b = bufs;
  next = nexts;

  while (n_left_from >= 4)
    {
      /* Prefetch next iteration. */
      if (PREDICT_TRUE (n_left_from >= 8))
	{
	  vlib_prefetch_buffer_header (b[4], STORE);
	  vlib_prefetch_buffer_header (b[5], STORE);
	  vlib_prefetch_buffer_header (b[6], STORE);
	  vlib_prefetch_buffer_header (b[7], STORE);
          CLIB_PREFETCH (b[4]->data, CLIB_CACHE_LINE_BYTES, STORE);
          CLIB_PREFETCH (b[5]->data, CLIB_CACHE_LINE_BYTES, STORE);
          CLIB_PREFETCH (b[6]->data, CLIB_CACHE_LINE_BYTES, STORE);
          CLIB_PREFETCH (b[7]->data, CLIB_CACHE_LINE_BYTES, STORE);
	}

     /* $$$$ process 4x pkts right here */
      next[0] = 0;
      next[1] = 0;
      next[2] = 0;
      next[3] = 0;

      if (is_trace)
	{
	  if (b[0]->flags & VLIB_BUFFER_IS_TRACED)
	    {
	      punt_inject_router_trace_t *t = 
                   vlib_add_trace (vm, node, b[0], sizeof (*t));
	      t->next_index = next[0];
              t->sw_if_index = vnet_buffer(b[0])->sw_if_index[VLIB_RX];
	    }
	  if (b[1]->flags & VLIB_BUFFER_IS_TRACED)
	    {
	      punt_inject_router_trace_t *t = 
                    vlib_add_trace (vm, node, b[1], sizeof (*t));
	      t->next_index = next[1];
              t->sw_if_index = vnet_buffer(b[1])->sw_if_index[VLIB_RX];
	    }
	  if (b[2]->flags & VLIB_BUFFER_IS_TRACED)
	    {
	      punt_inject_router_trace_t *t = 
                    vlib_add_trace (vm, node, b[2], sizeof (*t));
	      t->next_index = next[2];
              t->sw_if_index = vnet_buffer(b[2])->sw_if_index[VLIB_RX];
	    }
	  if (b[3]->flags & VLIB_BUFFER_IS_TRACED)
	    {
	      punt_inject_router_trace_t *t = 
                    vlib_add_trace (vm, node, b[3], sizeof (*t));
	      t->next_index = next[3];
              t->sw_if_index = vnet_buffer(b[3])->sw_if_index[VLIB_RX];
	    }
	}

      b += 4;
      next += 4;
      n_left_from -= 4;
    }

  while (n_left_from > 0)
    {

     /* $$$$ process 1 pkt right here */
      next[0] = 0;

      if (is_trace)
	{
	  if (b[0]->flags & VLIB_BUFFER_IS_TRACED)
	    {
	      punt_inject_router_trace_t *t = 
                    vlib_add_trace (vm, node, b[0], sizeof (*t));
	      t->next_index = next[0];
              t->sw_if_index = vnet_buffer(b[0])->sw_if_index[VLIB_RX];
	    }
	}

      b += 1;
      next += 1;
      n_left_from -= 1;
    }

  vlib_buffer_enqueue_to_next (vm, node, from, nexts, frame->n_vectors);

  return frame->n_vectors;
}

VLIB_NODE_FN (punt_inject_router_node) (vlib_main_t * vm, vlib_node_runtime_t * node,
                             vlib_frame_t * frame)
{
  if (PREDICT_FALSE (node->flags & VLIB_NODE_FLAG_TRACE))
    return punt_inject_router_inline (vm, node, frame, 1 /* is_ip4 */ ,
			    1 /* is_trace */ );
  else
    return punt_inject_router_inline (vm, node, frame, 1 /* is_ip4 */ ,
			    0 /* is_trace */ );
}

/* *INDENT-OFF* */
#ifndef CLIB_MARCH_VARIANT
VLIB_REGISTER_NODE (punt_inject_router_node) = 
{
  .name = "punt_inject_router",
  .vector_size = sizeof (u32),
  .format_trace = format_punt_inject_router_trace,
  .type = VLIB_NODE_TYPE_INTERNAL,
  
  .n_errors = ARRAY_LEN(punt_inject_router_error_strings),
  .error_strings = punt_inject_router_error_strings,

  .n_next_nodes = PUNT_INJECT_ROUTER_N_NEXT,

  /* edit / add dispositions here */
  .next_nodes = {
        [PUNT_INJECT_ROUTER_NEXT_DROP] = "error-drop",
  },
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

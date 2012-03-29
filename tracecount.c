/* **********************************************************
 * Copyright (c) 2012 Google, Inc.    All rights reserved.
 * **********************************************************/

/* Trace and basic block count.  */

#include "dr_api.h"

/* Forward decls. */
static void event_exit(void);

static dr_emit_flags_t
event_basic_block(void *drcontext, void *tag,
                  instrlist_t *bb, bool for_trace,
                  bool translating);

static dr_emit_flags_t
event_trace(void *drcontext, void *tag, instrlist_t *trace,
            bool translating);

/* Exported entry point. */
DR_EXPORT void
dr_init(client_id_t id)
{
    /* Can't use libc printf: conflict with app! */
    dr_printf("Client 'bbcount' initializing\n");
    /* Register events. */
    dr_register_bb_event(event_basic_block);
    dr_register_trace_event(event_trace);
    dr_register_exit_event(event_exit);
}

/************************************************************
 * Basic block counting.
 */

static uint bbs_instrumented;
static uint bbs_executed;

/* Basic block execution callback, aka Pin's "analysis" callback. */
static void bb_count(void) { bbs_executed++; }

/* Basic block instrumentation callback. */
static dr_emit_flags_t
event_basic_block(void *drcontext, void *tag, instrlist_t *bb,
                  bool for_trace, bool translating)
{
    /* bb event called again for tracing and translating, don't count those. */
    if (!for_trace && !translating)
        bbs_instrumented++;
    if (!for_trace)
        dr_insert_clean_call(drcontext, bb, instrlist_first(bb),
                             (void *)bb_count, false/*fpstate*/, 0/*num args*/);
    return DR_EMIT_DEFAULT;
}

/************************************************************
 * Trace counting.
 */

static uint traces_instrumented;
static uint traces_executed;

/* Trace execution callback, aka Pin's "analysis" callback. */
static void trace_count(void) { traces_executed++; }

/* Trace instrumentation callback. */
static dr_emit_flags_t
event_trace(void *drcontext, void *tag, instrlist_t *trace, bool translating)
{
    /* trace event called again for translating, don't count those. */
    if (!translating)
        traces_instrumented++;
    dr_insert_clean_call(drcontext, trace, instrlist_first(trace),
                         (void *)trace_count, false/*fpstate*/, 0/*num args*/);
    return DR_EMIT_DEFAULT;
}

static void
event_exit(void)
{
    dr_messagebox("Instrumentation results:\n"
                  "%10u bbs instrumented\n"
                  "%10u bbs executed\n"
                  "%10u traces instrumented\n"
                  "%10u traces executed\n",
                  bbs_instrumented,
                  bbs_executed,
                  traces_instrumented,
                  traces_executed);
}

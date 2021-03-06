/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#pragma once

/* the NVIDIA Timers (TMR) in the manual, there are 10 of them for TK1
 * each timer has a 29-bit programmable timer counter and a 32-bit
 * timestamp counter
 * */

#include <platsupport/timer.h>
#include <stdint.h>

typedef enum {
    TMR0 = 0,
    TMR1,
    TMR2,
    TMR3,
    TMR4,
    TMR5,
    TMR6,
    TMR7,
    TMR8,
    TMR9,
    TMR_LAST = TMR9,
    PS_DEFAULT_TIMER = TMR0
} nv_tmr_id_t;


typedef struct {
    /* vaddr of the base timer device registers */
    uintptr_t vaddr_base;
    /* second vaddr mapping for platforms that have timers on separate pages (TX2) */
    uintptr_t vaddr_tmr;
    nv_tmr_id_t id;
} nv_tmr_config_t;

struct tmr_map {
    uint32_t pvt;    /* present trigger value */
    uint32_t pcr;    /* present count value */
};


typedef struct nv_tmr {
    volatile struct tmr_map         *tmr_map;
    volatile struct tmrus_map       *tmrus_map;
    volatile struct tmr_shared_map  *tmr_shared_map;
    uint64_t                        counter_start;
} nv_tmr_t;

static UNUSED timer_properties_t tmr_properties = {
    .upcounter = true,
    .timeouts = true,
    .irqs = 1,
    .relative_timeouts = true,
    .periodic_timeouts = true,
    .absolute_timeouts = false,
};

int nv_tmr_start(nv_tmr_t *tmr);
int nv_tmr_stop(nv_tmr_t *tmr);
int nv_tmr_set_timeout(nv_tmr_t *tmr, bool periodic, uint64_t ns);
void nv_tmr_handle_irq(nv_tmr_t *tmr);
uint64_t nv_tmr_get_time(nv_tmr_t *tmr);
long nv_tmr_get_irq(nv_tmr_id_t n);
int nv_tmr_init(nv_tmr_t *tmr, nv_tmr_config_t config);
uint32_t nv_tmr_get_usec_upcounter_val(nv_tmr_t *tmr);

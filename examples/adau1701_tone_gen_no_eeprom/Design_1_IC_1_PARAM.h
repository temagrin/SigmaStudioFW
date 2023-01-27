/*
 * File:           C:\Users\TMno\Documents\Arduino\adau1701_tone_gen_no_eeprom\Design_1_IC_1_PARAM.h
 *
 * Created:        Saturday, January 28, 2023 3:59:50 AM
 * Description:    :IC 1 parameter RAM definitions.
 *
 * This software is distributed in the hope that it will be useful,
 * but is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This software may only be used to program products purchased from
 * Analog Devices for incorporation by you into audio products that
 * are intended for resale to audio product end users. This software
 * may not be distributed whole or in any part to third parties.
 *
 * Copyright ©2023 Analog Devices, Inc. All rights reserved.
 */
#ifndef __DESIGN_1_IC_1_PARAM_H__
#define __DESIGN_1_IC_1_PARAM_H__


/* Module Tone1 - Sine Tone*/
#define MOD_TONE1_COUNT                                3
#define MOD_TONE1_DEVICE                               "IC1"
#define MOD_STATIC_TONE1_ALG0_MASK_ADDR                0
#define MOD_STATIC_TONE1_ALG0_MASK_FIXPT               0x000000FF
#define MOD_STATIC_TONE1_ALG0_MASK_VALUE               SIGMASTUDIOTYPE_INTEGER_CONVERT(255)
#define MOD_STATIC_TONE1_ALG0_MASK_TYPE                SIGMASTUDIOTYPE_INTEGER
#define MOD_TONE1_ALG0_INCREMENT_ADDR                  1
#define MOD_TONE1_ALG0_INCREMENT_FIXPT                 0x0002AAAE
#define MOD_TONE1_ALG0_INCREMENT_VALUE                 SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.02083375)
#define MOD_TONE1_ALG0_INCREMENT_TYPE                  SIGMASTUDIOTYPE_FIXPOINT
#define MOD_TONE1_ALG0_ON_ADDR                         2
#define MOD_TONE1_ALG0_ON_FIXPT                        0x00800000
#define MOD_TONE1_ALG0_ON_VALUE                        SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_TONE1_ALG0_ON_TYPE                         SIGMASTUDIOTYPE_FIXPOINT

/* Module Square1 - Square wave*/
#define MOD_SQUARE1_COUNT                              3
#define MOD_SQUARE1_DEVICE                             "IC1"
#define MOD_STATIC_SQUARE1_ALG0_MASK_ADDR              3
#define MOD_STATIC_SQUARE1_ALG0_MASK_FIXPT             0x000000FF
#define MOD_STATIC_SQUARE1_ALG0_MASK_VALUE             SIGMASTUDIOTYPE_INTEGER_CONVERT(255)
#define MOD_STATIC_SQUARE1_ALG0_MASK_TYPE              SIGMASTUDIOTYPE_INTEGER
#define MOD_SQUARE1_ALG0_FREQ_ADDR                     4
#define MOD_SQUARE1_ALG0_FREQ_FIXPT                    0x0002AAAE
#define MOD_SQUARE1_ALG0_FREQ_VALUE                    SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.02083375)
#define MOD_SQUARE1_ALG0_FREQ_TYPE                     SIGMASTUDIOTYPE_FIXPOINT
#define MOD_SQUARE1_ALG0_ON_ADDR                       5
#define MOD_SQUARE1_ALG0_ON_FIXPT                      0x00000000
#define MOD_SQUARE1_ALG0_ON_VALUE                      SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0)
#define MOD_SQUARE1_ALG0_ON_TYPE                       SIGMASTUDIOTYPE_FIXPOINT

#endif

/*! \file bcm56960_a0_meter_sc_drv.h
 *
 * This file contains chip specific macro definitions
 * for Storm Control Meter.
 */
/*
 * Copyright: (c) 2018 Broadcom. All Rights Reserved. "Broadcom" refers to 
 * Broadcom Limited and/or its subsidiaries.
 * 
 * Broadcom Switch Software License
 * 
 * This license governs the use of the accompanying Broadcom software. Your 
 * use of the software indicates your acceptance of the terms and conditions 
 * of this license. If you do not agree to the terms and conditions of this 
 * license, do not use the software.
 * 1. Definitions
 *    "Licensor" means any person or entity that distributes its Work.
 *    "Software" means the original work of authorship made available under 
 *    this license.
 *    "Work" means the Software and any additions to or derivative works of 
 *    the Software that are made available under this license.
 *    The terms "reproduce," "reproduction," "derivative works," and 
 *    "distribution" have the meaning as provided under U.S. copyright law.
 *    Works, including the Software, are "made available" under this license 
 *    by including in or with the Work either (a) a copyright notice 
 *    referencing the applicability of this license to the Work, or (b) a copy 
 *    of this license.
 * 2. Grant of Copyright License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    copyright license to reproduce, prepare derivative works of, publicly 
 *    display, publicly perform, sublicense and distribute its Work and any 
 *    resulting derivative works in any form.
 * 3. Grant of Patent License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    patent license to make, have made, use, offer to sell, sell, import, and 
 *    otherwise transfer its Work, in whole or in part. This patent license 
 *    applies only to the patent claims licensable by Licensor that would be 
 *    infringed by Licensor's Work (or portion thereof) individually and 
 *    excluding any combinations with any other materials or technology.
 *    If you institute patent litigation against any Licensor (including a 
 *    cross-claim or counterclaim in a lawsuit) to enforce any patents that 
 *    you allege are infringed by any Work, then your patent license from such 
 *    Licensor to the Work shall terminate as of the date such litigation is 
 *    filed.
 * 4. Redistribution
 *    You may reproduce or distribute the Work only if (a) you do so under 
 *    this License, (b) you include a complete copy of this License with your 
 *    distribution, and (c) you retain without modification any copyright, 
 *    patent, trademark, or attribution notices that are present in the Work.
 * 5. Derivative Works
 *    You may specify that additional or different terms apply to the use, 
 *    reproduction, and distribution of your derivative works of the Work 
 *    ("Your Terms") only if (a) Your Terms provide that the limitations of 
 *    Section 7 apply to your derivative works, and (b) you identify the 
 *    specific derivative works that are subject to Your Terms. 
 *    Notwithstanding Your Terms, this license (including the redistribution 
 *    requirements in Section 4) will continue to apply to the Work itself.
 * 6. Trademarks
 *    This license does not grant any rights to use any Licensor's or its 
 *    affiliates' names, logos, or trademarks, except as necessary to 
 *    reproduce the notices described in this license.
 * 7. Limitations
 *    Platform. The Work and any derivative works thereof may only be used, or 
 *    intended for use, with a Broadcom switch integrated circuit.
 *    No Reverse Engineering. You will not use the Work to disassemble, 
 *    reverse engineer, decompile, or attempt to ascertain the underlying 
 *    technology of a Broadcom switch integrated circuit.
 * 8. Termination
 *    If you violate any term of this license, then your rights under this 
 *    license (including the license grants of Sections 2 and 3) will 
 *    terminate immediately.
 * 9. Disclaimer of Warranty
 *    THE WORK IS PROVIDED "AS IS" WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 *    KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WARRANTIES OR CONDITIONS OF 
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE OR 
 *    NON-INFRINGEMENT. YOU BEAR THE RISK OF UNDERTAKING ANY ACTIVITIES UNDER 
 *    THIS LICENSE. SOME STATES' CONSUMER LAWS DO NOT ALLOW EXCLUSION OF AN 
 *    IMPLIED WARRANTY, SO THIS DISCLAIMER MAY NOT APPLY TO YOU.
 * 10. Limitation of Liability
 *    EXCEPT AS PROHIBITED BY APPLICABLE LAW, IN NO EVENT AND UNDER NO LEGAL 
 *    THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE 
 *    SHALL ANY LICENSOR BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY DIRECT, 
 *    INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
 *    OR RELATED TO THIS LICENSE, THE USE OR INABILITY TO USE THE WORK 
 *    (INCLUDING BUT NOT LIMITED TO LOSS OF GOODWILL, BUSINESS INTERRUPTION, 
 *    LOST PROFITS OR DATA, COMPUTER FAILURE OR MALFUNCTION, OR ANY OTHER 
 *    COMMERCIAL DAMAGES OR LOSSES), EVEN IF THE LICENSOR HAS BEEN ADVISED OF 
 *    THE POSSIBILITY OF SUCH DAMAGES.
 */

#ifndef BCM56960_A0_METER_SC_DRV_H
#define BCM56960_A0_METER_SC_DRV_H

#include <bcmdrd/chip/bcm56960_a0_enum.h>
#include <bcmmeter/bcmmeter_sc.h>

/*! Storm Control Meters per port for BCM56960_A0. */
#define BCM56960_A0_METER_SC_METERS_PER_PORT           4
/*! Max ports per device for BCM56960_A0. */
#define BCM56960_A0_METER_SC_MAX_PORTS                 136
/*! Number of quantum levels for BCM56960_A0. */
#define BCM56960_A0_METER_SC_PKT_QUANTUM_LEVELS        18
/*! Minimum meter rate in byte mode for BCM56960_A0. */
#define BCM56960_A0_METER_SC_METER_RATE_KBIT_MIN       64
/*! Max bucket size for BCM56960_A0. */
#define BCM56960_A0_METER_SC_MAX_BUCKET_SZ             8
/*! Max refresh count for BCM56960_A0. */
#define BCM56960_A0_METER_SC_MAX_REFRESH_COUNT         0x1fffff

/*! Convert microseconds to seconds. */
#define BCM56960_A0_METER_MICROSEC2SEC                 (1000 * 1000)
/*! Refresh interval in microseconds for BCM56960_A0. */
#define BCM56960_A0_METER_REFRESH_INTERVAL             15.625

/*! Number of meter refreshes per sec for BCM56960_A0. */
#define BCM56960_A0_METER_SC_NUM_REFRESH_PER_SEC       \
    (BCM56960_A0_METER_MICROSEC2SEC /                  \
     BCM56960_A0_METER_REFRESH_INTERVAL)

/*! Number of tokens added every refresh cycle when refresh count is 1 for BCM56960_A0. */
#define BCM56960_A0_METER_SC_MIN_TOKENS_PER_REFRESH    2
/*! Storm Control Meter Configuration register name for BCM56960_A0. */
#define BCM56960_A0_METER_SC_CONFIG_SID                STORM_CONTROL_METER_CONFIGr
/*! Storm Control Meter H/w table name for BCM56960_A0. */
#define BCM56960_A0_METER_SC_METER_SID                 IFP_STORM_CONTROL_METERSm
/*! Storm Control Configuration byte mode field name for BCM56960_A0. */
#define BCM56960_A0_METER_SC_BYTEMODE_FID              BYTE_MODEf
/*! Storm Control Configuration pakcet quantum field name for BCM56960_A0. */
#define BCM56960_A0_METER_SC_PKTQUANTUM_FID            PACKET_QUANTUMf
/*! Storm Control Meter refresh count field name for BCM56960_A0. */
#define BCM56960_A0_METER_SC_REFRESHCOUNT_FID          REFRESHCOUNTf
/*! Storm Control bucket size field name for BCM56960_A0. */
#define BCM56960_A0_METER_SC_BUCKETSIZE_FID            BUCKETSIZEf
/*! Storm Control bucket count field name for BCM56960_A0. */
#define BCM56960_A0_METER_SC_BUCKETCOUNT_FID           BUCKETCOUNTf

extern int bcm56960_a0_meter_sc_init(int unit, bool cold);
extern int bcm56960_a0_meter_sc_cleanup(int unit);
extern int bcm56960_a0_meter_sc_insert (int unit,
                                        uint32_t trans_id,
                                        bcmmeter_sc_entry_t *sc_entry);
extern int bcm56960_a0_meter_sc_lookup (int unit,
                                        uint32_t trans_id,
                                        uint32_t port_id,
                                        uint32_t meter_idx,
                                        bcmmeter_sc_entry_t **ptr);

extern int bcm56960_a0_meter_sc_delete (int unit,
                                        uint32_t trans_id,
                                        bcmmeter_sc_entry_t *sc_entry);

extern int bcm56960_a0_meter_sc_bytemode_get (int unit,
                                              uint32_t trans_id,
                                              uint32_t port_id,
                                              uint32_t *byte_mode);

#endif /* BCM56960_A0_METER_SC_DRV_H */
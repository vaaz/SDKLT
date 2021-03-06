/*! \file stg.c
 *
 * STG Transform Handler
 *
 * This file contains field transform information for STG.
 *
 * Element 'N' of the state array corresponds to the field
 * arg->rfield[N](forward transform), arg->field[N](reverse transform).
 * State index should be between 0 and arg->rfields - 1 (forward transform)
 * between 0 and arg->fields -1 (reverse transform).
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

#include <shr/shr_debug.h>
#include <bsl/bsl.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmlrd/bcmlrd_table.h>
#include <bcmvlan/bcmvlan_stg.h>

#define BSL_LOG_MODULE BSL_LS_BCMLTX_VLAN

/*
 * \brief STG state array forward transform
 *
 * \param [in]  unit            Unit number.
 * \param [in]  in              STATE field array.
 * \param [out] out             STG_VLAN/EGR_STG_VLAN fields.
 * \param [in]  arg             Handler arguments.
 *
 * Transform direction is logical to physical.
 *
 * There is a single input STG state field array. The array idx of the
 * field is effectively a port number. There are N output fields,
 * where N is the number of ports, and each output field corresponds
 * to the port STG state. This transform simply copies the input
 * fields from the field array to the physical table field
 * corresponding to a port.
 *
 * \retval SHR_E_NONE  OK
 * \retval !SHR_E_NONE ERROR
 */

int
bcmvlan_stg_transform(int unit,
                      const bcmltd_fields_t *in,
                      bcmltd_fields_t *out,
                      const bcmltd_transform_arg_t *arg)
{
    size_t i; /* logical in field index */
    size_t port; /* physical out field index, which is essentially port */
    int rv = SHR_E_NONE;
    bcmltd_sid_t sid;
    uint32_t idx_count;

    SHR_FUNC_ENTER(unit);

    LOG_VERBOSE(BSL_LOG_MODULE,
                (BSL_META_U(unit,
                            "\t bcmvlan_stg_transform\n")));

    sid = arg->comp_data->sid;
    idx_count = bcmlrd_field_idx_count_get(unit, sid, arg->field[0]);

    for (i = out->count = 0; i < in->count; i++) {
        port = in->field[i]->idx;
        /* The number of rfields is equal to the number of ports */
        if (port < arg->rfields) {
            out->field[out->count]->id = arg->rfield[port];
            out->field[out->count]->data = in->field[i]->data;
            out->count++;
        } else {
            /* Port limit exceeded */
            if (port >= idx_count) {
                rv = SHR_E_FAIL;
                break;
            }
        }
    }

    SHR_RETURN_VAL_EXIT(rv);
 exit:
    SHR_FUNC_EXIT();
}

/*
 * \brief STG state array reverse transform
 *
 * \param [in]  unit            Unit number.
 * \param [in]  in              STG_VLAN/EGR_STG_VLAN fields.
 * \param [out] out             STATE field array.
 * \param [in]  arg             Handler arguments.
 *
 * Transform direction is physical to logical.
 *
 * There are N input fields, where N is the number of ports, and each
 * input field corresponds to the STG state of a port. There is a
 * single output STG state field array. The array idx of the field is
 * effectively a port number.  This transform simply copies the input
 * physical table field corresponding to a port to the field array.
 *
 * \retval SHR_E_NONE  OK
 * \retval !SHR_E_NONE ERROR
 */

int
bcmvlan_stg_rev_transform(int unit,
                          const bcmltd_fields_t *in,
                          bcmltd_fields_t *out,
                          const bcmltd_transform_arg_t *arg)
{
    size_t port; /* physical out field index */
    int rv = SHR_E_NONE;

    SHR_FUNC_ENTER(unit);

    LOG_VERBOSE(BSL_LOG_MODULE,
                (BSL_META_U(unit,
                            "\t bcmvlan_rev_stg_transform\n")));

    for (port = out->count = 0; port < in->count; port++) {
        /* The number of fields is equal to the number of ports */
        if (port < arg->fields) {
            out->field[out->count]->id = arg->rfield[0];
            out->field[out->count]->idx = port;
            out->field[out->count]->data = in->field[port]->data;
            out->count++;
        } else {
            /* port limit exceeded */
            rv = SHR_E_FAIL;
            break;
        }
    }

    SHR_RETURN_VAL_EXIT(rv);
 exit:
    SHR_FUNC_EXIT();
}



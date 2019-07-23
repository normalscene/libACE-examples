#!/bin/bash
sed \
	-e 's/\<CHAR\>/char/g' \
	-e 's/\<BOOL\>/bool/g' \
	-e 's/\<TRUE\>/true/g' \
	-e 's/\<FALSE\>/false/g' \
	-e 's/\<INT8\>/int8_t/g' \
	-e 's/\<INT16\>/int16_t/g' \
	-e 's/\<INT32\>/int32_t/g' \
	-e 's/\<UINT8\>/uint8_t/g' \
	-e 's/\<UINT16\>/uint16_t/g' \
	-e 's/\<UINT32\>/uint32_t/g' \
	-e 's/\<FLOAT\>/float/g' \
	-e 's/\<DOUBLE\>/double/g' \
	-e 's/\<emtUnPack\>/emt_unpack/g' \
	-e 's/\<emtInit\>/emt_init/g' \
	-e 's/\<emtBasePack\>/emt_base_pack/g' \
	-e 's/\<emtBaseUnpack\>/emt_base_unpack/g' \
	-e 's/\<emtBaseUnpack\>/emt_base_unpack/g' \
	-e 's/\<eMtGetCmdInfor\>/emt_get_cmd_info/g' \
	-e 's/\<emtFindValidPack\>/emt_find_valid_pack/g' \
	-e 's/\<emtPackBase\>/emt_pack_base/g' \
	-e 's/\<emtPackLite\>/emt_pack_lite/g' \
	-e 's/\<emtLitePack\>/emt_lite_pack/g' \
	-e 's/\<emtLiteUnPack\>/emt_lite_unpack/g' \
	-e 's/\<emtPack\>/emt_pack/g' \
	-e 's/\<emtIsValidPack\>/emt_is_valid_pack/g' \
	-e 's/\<emtGetGroupNum\>/emt_get_group_number/g' \
	-e 's/\<emtBaseUnPack\>/emt_base_unpack/g' \
	-e 's/\<emtUnPackBase\>/emt_unpack_base/g' \
	-e 's/\<emtUnPackLite\>/emt_unpack_lite/g' \
	-e 's/\<emtWhoAmI\>/emt_who_am_i/g' \
	-e 's/\<eMtInit\>/emt_init/g' \
	-e 's/\<emtGetPrm\>/emt_get_prm/g' \
	-e 's/\<emtGetCmdAfn\>/emt_get_cmd_afn/g' \
	-e 's/\<emtGetFrzFreq\>/emt_get_freez_freq/g' \
	-e 's/\<nMtPow\>/nmt_pow/g' \
	-e 's/\<pMtGetRecTransFunc\>/pmt_get_record_trans_func/g' \
	-e 's/\<ucGetCmdFn\>/uc_get_cmd_fn/g' \
	-e 's/\<vMtSetFloatSign\>/vmt_set_float_sign/g' \
	-e 's/\<vMtSetDoubleSign\>/vmt_set_double_sign/g' \
	-e 's/\<vMtSetPw\>/vmt_set_pw/g' \
	-e 's/\<vMtSetPwBuf\>/vmt_set_pw_buf/g' \
	-e 's/\<vMtSetEC\>/vmt_set_ec/g' \
	-e 's/\<smtGetErr\>/smt_get_err/g' \
	-e 's/\<bMtGetFloatSign\>/bmt_get_float_sign/g' \
	-e 's/\<bMtGetDoubleSign\>/bmt_get_double_sign/g' \
	-e 's/emtTrans_/emt_trans_/g' \
	$@

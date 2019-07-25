sed \
	-e 's/\<CHAR\>/char/g'\
	-e 's/\<BOOL\>/bool/g'\
	-e 's/\<TRUE\>/true/g'\
	-e 's/\<FALSE\>/false/g'\
	\
	-e 's/\<INT8\>/int8_t/g'\
	-e 's/\<INT16\>/int16_t/g'\
	-e 's/\<INT32\>/int32_t/g'\
	-e 's/\<INT64\>/int64_t/g'\
	\
	-e 's/\<UINT8\>/uint8_t/g'\
	-e 's/\<UINT16\>/uint16_t/g'\
	-e 's/\<UINT32\>/uint32_t/g'\
	-e 's/\<UINT64\>/uint64_t/g'\
	\
	-e 's/\<FLOAT\>/float/g'\
	-e 's/\<DOUBLE\>/double/g'\
	\
	-e 's/\<eMtErr\>/emt_err_t/g'\
	-e 's/\<eMtAFN\>/emt_afn_t/g'\
	-e 's/\<eMtTrans\>/emt_trans_t/g'\
	\
	-e 's/\<smtPack\>/smt_pack_t/g'\
	-e 's/\<smtLitePack\>/smt_litepack_t/g'\
	-e 's/\<smtBasePack\>/smt_basepack_t/g'\
	-e 's/\<sMtUnpackCommon\>/smt_unpack_common_t/g'\
	\
	-e 's/\<emtLitePack\>/emt_lite_pack/g'\
	-e 's/\<emtLiteUnPack\>/emt_lite_unpack/g'\
	\
	-e 's/\<emtPackLite\>/emt_pack_lite/g'\
	-e 's/\<emtUnPackLite\>/emt_unpack_lite/g'\
	\
	-e 's/\<emtBasePack\>/emt_base_pack/g'\
	-e 's/\<emtBaseUnPack\>/emt_base_unpack/g'\
	\
	-e 's/\<emtPackBase\>/emt_pack_base/g'\
	-e 's/\<emtUnPackBase\>/emt_unpack_base/g'\
	\
	-e 's/\<emtPack\>/emt_pack/g'\
	-e 's/\<emtUnPack\>/emt_unpack/g'\
	-e 's/\<ecmGetCmdAfn\>/ecm_get_cmd_afn/g'\
	-e 's/\<eGetMtCmd\>/eget_mt_cmd/g'\
	-e 's/\<eGetCmCmd\>/eget_cm_cmd/g'\
	-e 's/\<ecmGetPrm\>/ecm_get_prm/g'\
	-e 's/\<emtTransAppData_cm_2_mt\>/emt_trans_appdata_cm2mt/g'\
	-e 's/\<emtTransAppData_mt_2_cm\>/emt_trans_appdata_mt2cm/g'\
	-e 's/\<ncm_3761_checkAndGetPacket\>/ncm_3761_check_and_get_pack/g'\
	-e 's/\<vCmSetEC\>/vcm_set_ec/g'\
	-e 's/\<bCmSetPw\>/bcm_set_pw/g'\
	\
	-i $@


--Beginning of script 2--   DatabaseDB2LUOW (SEG6_LLO) [WARNING***Please do not alter this line]--
-- CONNECT TO SEG6_LLO USER XXXX using XXXX;
INSERT INTO ASN.IBMQREP_RECVQUEUES
 (repqmapname, recvq, sendq, adminq, capture_alias, capture_schema,
 num_apply_agents, memory_limit, state, description, capture_server,
 source_type, maxagents_correlid)
 VALUES
 ('SEG6_CIT_ASN_TO_SEG6_LLO_ASN', 'ASN.QM3_TO_QM2.DATAQ',
 'ASN.QM3_TO_QM2.DATAQ', 'ASN.QM3.ADMINQ', 'SEG6_CIT', 'ASN', 16, 2,
 'A', '', 'SEG6_CIT', 'D', 0);
-- COMMIT;
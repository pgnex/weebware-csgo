#pragma once
struct recvprop;


class d_variant
{
public:
	union
	{
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
		float m_Vector[3];
		__int64  m_Int64;
	};
};

class c_rec_proxy_data
{
public:
	const recvprop* m_pRecvProp;
	d_variant m_Value;
	int m_iElement;
	int m_ObjectID;
};

typedef void(*RecvVarProxyFn)(const c_rec_proxy_data* pData, void* pStruct, void* pOut);

struct recvtable
{
	recvprop* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

struct recvprop
{
	char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	recvprop* m_pArrayProp;
	void* m_ArrayLengthProxy;
	void* m_ProxyFn;
	void* m_DataTableProxyFn;
	recvtable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
} SendPropType;
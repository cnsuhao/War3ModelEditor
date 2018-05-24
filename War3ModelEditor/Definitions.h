//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_DEFINITIONS_H
#define MAGOS_DEFINITIONS_H


//+-----------------------------------------------------------------------------
//| Memory macros
//+-----------------------------------------------------------------------------
#define SAFE_DELETE(P)       if(P) { delete (P);         (P) = NULL; }
#define SAFE_ARRAY_DELETE(P) if(P) { delete[] (P);       (P) = NULL; }
#define SAFE_RELEASE(P)      if(P) { (P)->Release();     (P) = NULL; }
#define SAFE_DESTROY(P)      if(P) { ::DestroyWindow(P); (P) = NULL; }


//+-----------------------------------------------------------------------------
//| Debug printing macro
//+-----------------------------------------------------------------------------
#define DEBUG(M) \
{ \
	std::stringstream _s; \
	_s << M; \
	MessageBox(NULL, _s.str().c_str(), "Debug", MB_OK); \
}


//+-----------------------------------------------------------------------------
//| Container clearing macro
//+-----------------------------------------------------------------------------
#define SAFE_CLEAR(C) \
for(INT _i = 0; _i < (C).GetTotalSize(); _i++) \
{ \
	if((C).ValidIndex(_i)) \
	{ \
		delete (C)[_i]; \
	} \
} \
(C).Clear();


//+-----------------------------------------------------------------------------
//| Container ID-generating macro
//+-----------------------------------------------------------------------------
#define CONTAINER_GENERATE_ID(C) \
for(INT _i = 0; _i < (C).GetTotalSize(); _i++) \
{ \
	if((C).ValidIndex(_i)) \
	{ \
		(C)[_i]->BaseData()->ObjectId = ObjectIdManager.GetNewObjectId(); \
	} \
}


//+-----------------------------------------------------------------------------
//| Container ID-assigning macro
//+-----------------------------------------------------------------------------
#define CONTAINER_ASSIGN_ID(C, I) \
{ \
	INT _j = 0; \
	for(INT _i = 0; _i < (C).GetTotalSize(); _i++) \
	{ \
		if((C).ValidIndex(_i)) \
		{ \
			(C)[_i]->Data().I = _j; \
			_j++; \
		} \
	} \
}


//+-----------------------------------------------------------------------------
//| Container ID-retrieving macro
//+-----------------------------------------------------------------------------
#define CONTAINER_RETRIEVE_ID(C, N, I1, I2) \
for(INT _i = 0; _i < (C).GetTotalSize(); _i++) \
{ \
	if((C).ValidIndex(_i)) \
	{ \
		if((C)[_i]->N.IsAttached()) \
		{ \
			(C)[_i]->Data().I1 = (C)[_i]->N.GetObjectData()->Data().I2; \
		} \
		else \
		{ \
			(C)[_i]->Data().I1 = INVALID_INDEX; \
		} \
	} \
}


//+-----------------------------------------------------------------------------
//| Container node connecting macro
//+-----------------------------------------------------------------------------
#define CONTAINER_CONNECT_NODES(C1, C2, N1, N2, I) \
for(INT _i = 0; _i < (C1).GetTotalSize(); _i++) \
{ \
	if((C1).ValidIndex(_i)) \
	{ \
		INT _j =  (C1)[_i]->Data().I; \
		if((C2).ValidIndex(_j)) \
		{ \
			(C1)[_i]->N1.Attach((C2)[_j]->N2); \
		} \
	} \
}


//+-----------------------------------------------------------------------------
//| Class macros
//+-----------------------------------------------------------------------------
#define CONSTRUCTOR
#define DESTRUCTOR  virtual


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

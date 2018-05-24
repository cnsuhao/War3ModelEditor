//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Interpolator.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
REFERENCE_OBJECT<INTERPOLATOR*, INTERPOLATOR*> INTERPOLATOR::InterpolatorNodes;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
INTERPOLATOR::INTERPOLATOR()
{
	Name = "";
	GlobalSequenceId = INVALID_INDEX;
	Type = INTERPOLATOR_TYPE_NONE;
	InterpolationType = INTERPOLATION_TYPE_NONE;

	Static = TRUE;
	StaticVector = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	DefaultVector = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	GlobalSequenceNode.SetData(this);
	InterpolatorNode.SetData(this);

	InterpolatorNode.Attach(InterpolatorNodes);
}


//+-----------------------------------------------------------------------------
//| Copy constructor
//+-----------------------------------------------------------------------------
INTERPOLATOR::INTERPOLATOR(CONST INTERPOLATOR& CopyObject)
{
	NodeList = CopyObject.NodeList;

	GlobalSequenceId = CopyObject.GlobalSequenceId;
	Type = CopyObject.Type;
	InterpolationType = CopyObject.InterpolationType;

	Static = CopyObject.Static;
	StaticVector = CopyObject.StaticVector;
	DefaultVector = CopyObject.DefaultVector;

	GlobalSequenceNode = CopyObject.GlobalSequenceNode;
	InterpolatorNode = CopyObject.InterpolatorNode;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
INTERPOLATOR::~INTERPOLATOR()
{
	Clear();

	InterpolatorNode.Detach();
}


//+-----------------------------------------------------------------------------
//| Assignment operator
//+-----------------------------------------------------------------------------
CONST INTERPOLATOR& INTERPOLATOR::operator =(CONST INTERPOLATOR& CopyObject)
{
	NodeList = CopyObject.NodeList;

	GlobalSequenceId = CopyObject.GlobalSequenceId;
	Type = CopyObject.Type;
	InterpolationType = CopyObject.InterpolationType;

	Static = CopyObject.Static;
	StaticVector = CopyObject.StaticVector;
	DefaultVector = CopyObject.DefaultVector;

	GlobalSequenceNode = CopyObject.GlobalSequenceNode;
	InterpolatorNode = CopyObject.InterpolatorNode;

	return (*this);
}


//+-----------------------------------------------------------------------------
//| Clears the interpolator
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::Clear()
{
	NodeList.clear();

	Type = INTERPOLATOR_TYPE_NONE;
	InterpolationType = INTERPOLATION_TYPE_NONE;

	Static = TRUE;
	StaticVector = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the interpolator
//+-----------------------------------------------------------------------------
INT INTERPOLATOR::GetSize()
{
	INT NodeSize = 0;
	INT Factor = 1;

	if(GetNrOfNodes() <= 0) return 0;

	switch(InterpolationType)
	{
		case INTERPOLATION_TYPE_HERMITE:
		case INTERPOLATION_TYPE_BEZIER:
		{
			Factor = 3;
			break;
		}
	}

	switch(Type)
	{
		case INTERPOLATOR_TYPE_SCALAR:
		case INTERPOLATOR_TYPE_SCALAR_INT:
		{
			NodeSize = (Factor * 4) + 4;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR2:
		{
			NodeSize = (Factor * 8) + 4;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR3:
		{
			NodeSize = (Factor * 12) + 4;
			break;
		}

		case INTERPOLATOR_TYPE_VECTOR4:
		{
			NodeSize = (Factor * 16) + 4;
			break;
		}
	}

	NodeSize *= GetNrOfNodes();

	return NodeSize + 16;
}


//+-----------------------------------------------------------------------------
//| Checks if the interpolator is static
//+-----------------------------------------------------------------------------
BOOL INTERPOLATOR::IsStatic()
{
	return Static;
}


//+-----------------------------------------------------------------------------
//| Returns the nr of nodes
//+-----------------------------------------------------------------------------
INT INTERPOLATOR::GetNrOfNodes()
{
	return static_cast<INT>(NodeList.size());
}


//+-----------------------------------------------------------------------------
//| Adds a new node to the interpolator
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::AddNode(CONST INTERPOLATOR_NODE& Node)
{
	NodeList.push_back(Node);
	Static = FALSE;
}


//+-----------------------------------------------------------------------------
//| Sets a new type
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetType(INTERPOLATOR_TYPE NewType)
{
	Type = NewType;
}


//+-----------------------------------------------------------------------------
//| Returns the type
//+-----------------------------------------------------------------------------
INTERPOLATOR_TYPE INTERPOLATOR::GetType()
{
	return Type;
}


//+-----------------------------------------------------------------------------
//| Sets a new interpoaltion type
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetInterpolationType(INTERPOLATION_TYPE NewInterpolationType)
{
	InterpolationType = NewInterpolationType;
}


//+-----------------------------------------------------------------------------
//| Returns the interpolation type
//+-----------------------------------------------------------------------------
INTERPOLATION_TYPE INTERPOLATOR::GetInterpolationType()
{
	return InterpolationType;
}


//+-----------------------------------------------------------------------------
//| Sets a new global sequence ID
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetGlobalSequenceId(INT NewGlobalSequenceId)
{
	GlobalSequenceId = NewGlobalSequenceId;
}


//+-----------------------------------------------------------------------------
//| Returns the global sequence ID
//+-----------------------------------------------------------------------------
INT INTERPOLATOR::GetGlobalSequenceId()
{
	return GlobalSequenceId;
}


//+-----------------------------------------------------------------------------
//| Saves the interpolator
//+-----------------------------------------------------------------------------
BOOL INTERPOLATOR::Save(DATA_OUT_STREAM& DataStream, BOOL ReverseXZ)
{
	std::list<INTERPOLATOR_NODE>::iterator i;

	DataStream.WriteDWord(static_cast<DWORD>(NodeList.size()));
	switch(InterpolationType)
	{
		case INTERPOLATION_TYPE_NONE:
		{
			DataStream.WriteDWord(0);
			break;
		}

		case INTERPOLATION_TYPE_LINEAR:
		{
			DataStream.WriteDWord(1);
			break;
		}

		case INTERPOLATION_TYPE_HERMITE:
		{
			DataStream.WriteDWord(2);
			break;
		}

		case INTERPOLATION_TYPE_BEZIER:
		{
			DataStream.WriteDWord(3);
			break;
		}
	}

	DataStream.WriteDWord((GlobalSequenceId == INVALID_INDEX) ? 0xFFFFFFFF : GlobalSequenceId);

	i = NodeList.begin();
	while(i != NodeList.end())
	{
		DataStream.WriteDWord(i->Time);

		switch(Type)
		{
			case INTERPOLATOR_TYPE_SCALAR:
			{
				DataStream.WriteFloat(i->Vector.x);
				break;
			}

			case INTERPOLATOR_TYPE_SCALAR_INT:
			{
				DataStream.WriteDWord(static_cast<DWORD>(i->Vector.x));
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR2:
			{
				DataStream.WriteFloat(i->Vector.x);
				DataStream.WriteFloat(i->Vector.y);
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR3:
			{
				if(ReverseXZ)
				{
					DataStream.WriteFloat(i->Vector.z);
					DataStream.WriteFloat(i->Vector.y);
					DataStream.WriteFloat(i->Vector.x);
				}
				else
				{
					DataStream.WriteFloat(i->Vector.x);
					DataStream.WriteFloat(i->Vector.y);
					DataStream.WriteFloat(i->Vector.z);
				}
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR4:
			{
				DataStream.WriteFloat(i->Vector.x);
				DataStream.WriteFloat(i->Vector.y);
				DataStream.WriteFloat(i->Vector.z);
				DataStream.WriteFloat(i->Vector.w);
				break;
			}
		}

		switch(InterpolationType)
		{
			case INTERPOLATION_TYPE_HERMITE:
			case INTERPOLATION_TYPE_BEZIER:
			{
				switch(Type)
				{
					case INTERPOLATOR_TYPE_SCALAR:
					{
						DataStream.WriteFloat(i->InTan.x);
						DataStream.WriteFloat(i->OutTan.x);
						break;
					}

					case INTERPOLATOR_TYPE_SCALAR_INT:
					{
						DataStream.WriteDWord(static_cast<DWORD>(i->InTan.x));
						DataStream.WriteDWord(static_cast<DWORD>(i->OutTan.x));
						break;
					}

					case INTERPOLATOR_TYPE_VECTOR2:
					{
						DataStream.WriteFloat(i->InTan.x);
						DataStream.WriteFloat(i->InTan.y);
						DataStream.WriteFloat(i->OutTan.x);
						DataStream.WriteFloat(i->OutTan.y);
						break;
					}

					case INTERPOLATOR_TYPE_VECTOR3:
					{
						if(ReverseXZ)
						{
							DataStream.WriteFloat(i->InTan.z);
							DataStream.WriteFloat(i->InTan.y);
							DataStream.WriteFloat(i->InTan.x);
							DataStream.WriteFloat(i->OutTan.z);
							DataStream.WriteFloat(i->OutTan.y);
							DataStream.WriteFloat(i->OutTan.x);
						}
						else
						{
							DataStream.WriteFloat(i->InTan.x);
							DataStream.WriteFloat(i->InTan.y);
							DataStream.WriteFloat(i->InTan.z);
							DataStream.WriteFloat(i->OutTan.x);
							DataStream.WriteFloat(i->OutTan.y);
							DataStream.WriteFloat(i->OutTan.z);
						}
						break;
					}

					case INTERPOLATOR_TYPE_VECTOR4:
					{
						DataStream.WriteFloat(i->InTan.x);
						DataStream.WriteFloat(i->InTan.y);
						DataStream.WriteFloat(i->InTan.z);
						DataStream.WriteFloat(i->InTan.w);
						DataStream.WriteFloat(i->OutTan.x);
						DataStream.WriteFloat(i->OutTan.y);
						DataStream.WriteFloat(i->OutTan.z);
						DataStream.WriteFloat(i->OutTan.w);
						break;
					}
				}

				break;
			}
		}

		i++;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads an interpolator
//+-----------------------------------------------------------------------------
BOOL INTERPOLATOR::Load(DATA_IN_STREAM& DataStream, BOOL ReverseXZ)
{
	INT i;
	INT NodeListSize;
	INTERPOLATOR_NODE Node;

	NodeList.clear();

	NodeListSize = DataStream.ReadDWord();

	switch(DataStream.ReadDWord())
	{
		case 0:
		{
			InterpolationType = INTERPOLATION_TYPE_NONE;
			break;
		}

		case 1:
		{
			InterpolationType = INTERPOLATION_TYPE_LINEAR;
			break;
		}

		case 2:
		{
			InterpolationType = INTERPOLATION_TYPE_HERMITE;
			break;
		}

		case 3:
		{
			InterpolationType = INTERPOLATION_TYPE_BEZIER;
			break;
		}

		default:
		{
			Error.SetMessage("Unknown interpolation type!");
			return FALSE;
		}
	}

	GlobalSequenceId = DataStream.ReadDWord();

	for(i = 0; i < NodeListSize; i++)
	{
		Node.Time = DataStream.ReadDWord();

		switch(Type)
		{
			case INTERPOLATOR_TYPE_SCALAR:
			{
				Node.Vector.x = DataStream.ReadFloat();
				break;
			}

			case INTERPOLATOR_TYPE_SCALAR_INT:
			{
				Node.Vector.x = static_cast<FLOAT>(DataStream.ReadDWord());
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR2:
			{
				Node.Vector.x = DataStream.ReadFloat();
				Node.Vector.y = DataStream.ReadFloat();
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR3:
			{
				if(ReverseXZ)
				{
					Node.Vector.z = DataStream.ReadFloat();
					Node.Vector.y = DataStream.ReadFloat();
					Node.Vector.x = DataStream.ReadFloat();
				}
				else
				{
					Node.Vector.x = DataStream.ReadFloat();
					Node.Vector.y = DataStream.ReadFloat();
					Node.Vector.z = DataStream.ReadFloat();
				}
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR4:
			{
				Node.Vector.x = DataStream.ReadFloat();
				Node.Vector.y = DataStream.ReadFloat();
				Node.Vector.z = DataStream.ReadFloat();
				Node.Vector.w = DataStream.ReadFloat();
				break;
			}
		}

		switch(InterpolationType)
		{
			case INTERPOLATION_TYPE_HERMITE:
			case INTERPOLATION_TYPE_BEZIER:
			{
				switch(Type)
				{
					case INTERPOLATOR_TYPE_SCALAR:
					{
						Node.InTan.x = DataStream.ReadFloat();
						Node.OutTan.x = DataStream.ReadFloat();
						break;
					}

					case INTERPOLATOR_TYPE_SCALAR_INT:
					{
						Node.InTan.x = static_cast<FLOAT>(DataStream.ReadDWord());
						Node.OutTan.x = static_cast<FLOAT>(DataStream.ReadDWord());
						break;
					}

					case INTERPOLATOR_TYPE_VECTOR2:
					{
						Node.InTan.x = DataStream.ReadFloat();
						Node.InTan.y = DataStream.ReadFloat();
						Node.OutTan.x = DataStream.ReadFloat();
						Node.OutTan.y = DataStream.ReadFloat();
						break;
					}

					case INTERPOLATOR_TYPE_VECTOR3:
					{
						if(ReverseXZ)
						{
							Node.InTan.z = DataStream.ReadFloat();
							Node.InTan.y = DataStream.ReadFloat();
							Node.InTan.x = DataStream.ReadFloat();
							Node.OutTan.z = DataStream.ReadFloat();
							Node.OutTan.y = DataStream.ReadFloat();
							Node.OutTan.x = DataStream.ReadFloat();
						}
						else
						{
							Node.InTan.x = DataStream.ReadFloat();
							Node.InTan.y = DataStream.ReadFloat();
							Node.InTan.z = DataStream.ReadFloat();
							Node.OutTan.x = DataStream.ReadFloat();
							Node.OutTan.y = DataStream.ReadFloat();
							Node.OutTan.z = DataStream.ReadFloat();
						}
						break;
					}

					case INTERPOLATOR_TYPE_VECTOR4:
					{
						Node.InTan.x = DataStream.ReadFloat();
						Node.InTan.y = DataStream.ReadFloat();
						Node.InTan.z = DataStream.ReadFloat();
						Node.InTan.w = DataStream.ReadFloat();
						Node.OutTan.x = DataStream.ReadFloat();
						Node.OutTan.y = DataStream.ReadFloat();
						Node.OutTan.z = DataStream.ReadFloat();
						Node.OutTan.w = DataStream.ReadFloat();
						break;
					}
				}

				break;
			}
		}

		NodeList.push_back(Node);
	}

	Static = FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the interpolator
//+-----------------------------------------------------------------------------
BOOL INTERPOLATOR::Save(TOKEN_OUT_STREAM& TokenStream, INT BaseTab)
{
	INT NodeListSize;
	std::list<INTERPOLATOR_NODE>::iterator i;

	if(Static)
	{
		TokenStream.WriteTab(BaseTab);
		TokenStream.WriteWord("static ");
		TokenStream.WriteWord(Name);
		TokenStream.WriteWord(" ");

		switch(Type)
		{
			case INTERPOLATOR_TYPE_SCALAR:
			{
				TokenStream.WriteFloat(StaticVector.x);
				break;
			}

			case INTERPOLATOR_TYPE_SCALAR_INT:
			{
				TokenStream.WriteInt(static_cast<INT>(StaticVector.x));
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR2:
			{
				TokenStream.WriteVector2(StaticVector);
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR3:
			{
				TokenStream.WriteVector3(StaticVector);
				break;
			}

			case INTERPOLATOR_TYPE_VECTOR4:
			{
				TokenStream.WriteVector4(StaticVector);
				break;
			}
		}

		TokenStream.WriteLine(",");

		return TRUE;
	}

	NodeListSize = static_cast<INT>(NodeList.size());
	if(NodeListSize <= 0) return TRUE;

	TokenStream.WriteTab(BaseTab);
	TokenStream.WriteWord(Name);
	TokenStream.WriteWord(" ");
	TokenStream.WriteInt(NodeListSize);
	TokenStream.WriteLine(" {");

	TokenStream.WriteTab(BaseTab + 1);
	switch(InterpolationType)
	{
		case INTERPOLATION_TYPE_NONE:
		{
			TokenStream.WriteLine("DontInterp,");
			break;
		}

		case INTERPOLATION_TYPE_LINEAR:
		{
			TokenStream.WriteLine("Linear,");
			break;
		}

		case INTERPOLATION_TYPE_HERMITE:
		{
			TokenStream.WriteLine("Hermite,");
			break;
		}

		case INTERPOLATION_TYPE_BEZIER:
		{
			TokenStream.WriteLine("Bezier,");
			break;
		}
	}

	if(GlobalSequenceId != INVALID_INDEX)
	{
		TokenStream.WriteTab(BaseTab + 1);
		TokenStream.WriteWord("GlobalSeqId ");
		TokenStream.WriteInt(GlobalSequenceId);
		TokenStream.WriteLine(",");
	}

	i = NodeList.begin();
	while(i != NodeList.end())
	{
		TokenStream.WriteTab(BaseTab + 1);
		TokenStream.WriteInt(i->Time);
		TokenStream.WriteWord(": ");

		switch(Type)
		{
			case INTERPOLATOR_TYPE_SCALAR:
			{
				TokenStream.WriteFloat(i->Vector.x);
				TokenStream.WriteLine(",");

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("InTan ");
				TokenStream.WriteFloat(i->InTan.x);
				TokenStream.WriteLine(",");

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("OutTan ");
				TokenStream.WriteFloat(i->OutTan.x);
				TokenStream.WriteLine(",");

				break;
			}

			case INTERPOLATOR_TYPE_SCALAR_INT:
			{
				TokenStream.WriteInt(static_cast<INT>(i->Vector.x));
				TokenStream.WriteLine(",");

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("InTan ");
				TokenStream.WriteInt(static_cast<INT>(i->InTan.x));
				TokenStream.WriteLine(",");

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("OutTan ");
				TokenStream.WriteInt(static_cast<INT>(i->OutTan.x));
				TokenStream.WriteLine(",");

				break;
			}

			case INTERPOLATOR_TYPE_VECTOR2:
			{
				TokenStream.WriteVector2(i->Vector);
				TokenStream.WriteLine(",");

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("InTan ");
				TokenStream.WriteVector2(i->InTan);
				TokenStream.WriteLine(",");

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("OutTan ");
				TokenStream.WriteVector2(i->OutTan);
				TokenStream.WriteLine(",");

				break;
			}

			case INTERPOLATOR_TYPE_VECTOR3:
			{
				TokenStream.WriteVector3(i->Vector);
				TokenStream.WriteLine(",");

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("InTan ");
				TokenStream.WriteVector3(i->InTan);
				TokenStream.WriteLine(",");

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("OutTan ");
				TokenStream.WriteVector3(i->OutTan);
				TokenStream.WriteLine(",");

				break;
			}

			case INTERPOLATOR_TYPE_VECTOR4:
			{
				TokenStream.WriteVector4(i->Vector);
				TokenStream.WriteLine(",");

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("InTan ");
				TokenStream.WriteVector4(i->InTan);
				TokenStream.WriteLine(",");

				TokenStream.WriteTab(BaseTab + 2);
				TokenStream.WriteWord("OutTan ");
				TokenStream.WriteVector4(i->OutTan);
				TokenStream.WriteLine(",");

				break;
			}
		}

		i++;
	}

	TokenStream.WriteTab(BaseTab);
	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads an interpolator
//+-----------------------------------------------------------------------------
BOOL INTERPOLATOR::Load(TOKEN_IN_STREAM& TokenStream)
{
	INT i;
	INT NrOfNodes;
	INTERPOLATOR_NODE Node;
	std::string Token;
	std::stringstream Stream;

	NodeList.clear();

	NrOfNodes = TokenStream.ReadInt();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	Token = TokenStream.ReadWord();
	if(Token == "DontInterp,")
	{
		InterpolationType = INTERPOLATION_TYPE_NONE;
	}
	else if(Token == "Linear,")
	{
		InterpolationType = INTERPOLATION_TYPE_LINEAR;
	}
	else if(Token == "Hermite,")
	{
		InterpolationType = INTERPOLATION_TYPE_HERMITE;
	}
	else if(Token == "Bezier,")
	{
		InterpolationType = INTERPOLATION_TYPE_BEZIER;
	}
	else
	{
		Error.SetMessage("Unable to load \"" + TokenStream.GetFileName() + "\", unknown interpolation mode \"" + Token + "\"!");
		return FALSE;
	}

	for(i = 0; i < NrOfNodes; i++)
	{
		Token = TokenStream.ReadWord();
		if(Token == "GlobalSeqId")
		{
			GlobalSequenceId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;

			NrOfNodes++;
			continue;
		}

		Stream.clear();
		Stream.str(Token);

		Node.Time = 0;
		Stream >> Node.Time;

		switch(Type)
		{
			case INTERPOLATOR_TYPE_SCALAR:
			case INTERPOLATOR_TYPE_SCALAR_INT:
			{
				Node.Vector.x = TokenStream.ReadFloat();
				Node.Vector.y = 0.0f;
				Node.Vector.z = 0.0f;
				Node.Vector.w = 0.0f;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				if(!TokenStream.ExpectWord("InTan")) return FALSE;
				Node.InTan.x = TokenStream.ReadFloat();
				Node.InTan.y = 0.0f;
				Node.InTan.z = 0.0f;
				Node.InTan.w = 0.0f;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(!TokenStream.ExpectWord("OutTan")) return FALSE;
				Node.OutTan.x = TokenStream.ReadFloat();
				Node.OutTan.y = 0.0f;
				Node.OutTan.z = 0.0f;
				Node.OutTan.w = 0.0f;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				break;
			}

			case INTERPOLATOR_TYPE_VECTOR2:
			{
				if(!TokenStream.ReadVector2(Node.Vector)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				if(!TokenStream.ExpectWord("InTan")) return FALSE;
				if(!TokenStream.ReadVector2(Node.InTan)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(!TokenStream.ExpectWord("OutTan")) return FALSE;
				if(!TokenStream.ReadVector2(Node.OutTan)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				break;
			}

			case INTERPOLATOR_TYPE_VECTOR3:
			{
				if(!TokenStream.ReadVector3(Node.Vector)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				if(!TokenStream.ExpectWord("InTan")) return FALSE;
				if(!TokenStream.ReadVector3(Node.InTan)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(!TokenStream.ExpectWord("OutTan")) return FALSE;
				if(!TokenStream.ReadVector3(Node.OutTan)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				break;
			}

			case INTERPOLATOR_TYPE_VECTOR4:
			{
				if(!TokenStream.ReadVector4(Node.Vector)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(InterpolationType == INTERPOLATION_TYPE_NONE) break;
				if(InterpolationType == INTERPOLATION_TYPE_LINEAR) break;

				if(!TokenStream.ExpectWord("InTan")) return FALSE;
				if(!TokenStream.ReadVector4(Node.InTan)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				if(!TokenStream.ExpectWord("OutTan")) return FALSE;
				if(!TokenStream.ReadVector4(Node.OutTan)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				break;
			}
		}

		NodeList.push_back(Node);
	}

	if(!TokenStream.ExpectChar('}')) return FALSE;

	Static = FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Retrieves the scalar at a certain timepoint
//+-----------------------------------------------------------------------------
FLOAT INTERPOLATOR::GetScalar(CONST SEQUENCE_TIME& Time)
{
	D3DXVECTOR4 TempVector;

	if(Static) return StaticVector.x;
	if(NodeList.size() <= 0) return 0.0f;

	GetInterpolatedValue(TempVector, Time);

	return TempVector.x;
}


//+-----------------------------------------------------------------------------
//| Retrieves the vector at a certain timepoint
//+-----------------------------------------------------------------------------
D3DXVECTOR2 INTERPOLATOR::GetVector2(CONST SEQUENCE_TIME& Time)
{
	D3DXVECTOR2 Vector2;
	D3DXVECTOR4 TempVector;

	if(Static) return D3DXVECTOR2(StaticVector.x, StaticVector.y);
	if(NodeList.size() <= 0) return D3DXVECTOR2(0.0f, 0.0f);

	GetInterpolatedValue(TempVector, Time);

	Vector2.x = TempVector.x;
	Vector2.y = TempVector.y;

	return Vector2;
}


//+-----------------------------------------------------------------------------
//| Retrieves the vector at a certain timepoint
//+-----------------------------------------------------------------------------
D3DXVECTOR3 INTERPOLATOR::GetVector3(CONST SEQUENCE_TIME& Time)
{
	D3DXVECTOR3 Vector3;
	D3DXVECTOR4 TempVector;

	if(Static) return D3DXVECTOR3(StaticVector.x, StaticVector.y, StaticVector.z);
	if(NodeList.size() <= 0) return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	GetInterpolatedValue(TempVector, Time);

	Vector3.x = TempVector.x;
	Vector3.y = TempVector.y;
	Vector3.z = TempVector.z;

	return Vector3;
}


//+-----------------------------------------------------------------------------
//| Retrieves the vector at a certain timepoint
//+-----------------------------------------------------------------------------
D3DXVECTOR4 INTERPOLATOR::GetVector4(CONST SEQUENCE_TIME& Time)
{
	D3DXVECTOR4 Vector4;

	if(Static) return StaticVector;
	if(NodeList.size() <= 0) return D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	GetQuaternionValue(Vector4, Time);

	return Vector4;
}


//+-----------------------------------------------------------------------------
//| Sets a static scalar
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticScalar(FLOAT NewScalar, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_SCALAR;
	StaticVector = D3DXVECTOR4(NewScalar, 0.0f, 0.0f, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static integer scalar
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticScalarInt(INT NewScalar, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_SCALAR_INT;
	StaticVector = D3DXVECTOR4(static_cast<FLOAT>(NewScalar), 0.0f, 0.0f, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static vector
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticVector2(CONST D3DXVECTOR2& NewVector2, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_VECTOR2;
	StaticVector = D3DXVECTOR4(NewVector2.x, NewVector2.y, 0.0f, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static vector
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticVector3(CONST D3DXVECTOR3& NewVector3, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_VECTOR3;
	StaticVector = D3DXVECTOR4(NewVector3.x, NewVector3.y, NewVector3.z, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static vector
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticVector4(CONST D3DXVECTOR4& NewVector4, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_VECTOR4;
	StaticVector = NewVector4;
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Retrieves the interpolated value
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::GetInterpolatedValue(D3DXVECTOR4& Vector, CONST SEQUENCE_TIME& Time)
{
	FLOAT Factor;
	FLOAT InverseFactor;
	SEQUENCE_TIME TempTime;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;
	std::list<INTERPOLATOR_NODE>::iterator Node1;
	std::list<INTERPOLATOR_NODE>::iterator Node2;

	if(GlobalSequenceNode.IsAttached())
	{
		GlobalSequence = GlobalSequenceNode.GetObjectData();
		TempTime.IntervalStart = 0;
		TempTime.IntervalEnd = GlobalSequence->Data().Duration;
		TempTime.Time = GlobalSequence->Data().Time;
	}
	else
	{
		TempTime = Time;
	}

	GetSurroundingNodes(TempTime, Node1, Node2);

	if(Node1 == NodeList.end())
	{
		Vector = DefaultVector;
		return;
	}

	if(Node2 == NodeList.end())
	{
		Vector = Node1->Vector;
		return;
	}

	if(Node1->Time >= Node2->Time)
	{
		Vector = Node1->Vector;
		return;
	}

	Factor = static_cast<FLOAT>(TempTime.Time - Node1->Time) / static_cast<FLOAT>(Node2->Time - Node1->Time);
	InverseFactor = (1.0f - Factor);

	switch(InterpolationType)
	{
		case INTERPOLATION_TYPE_NONE:
		{
			Vector = Node1->Vector;
			break;
		}

		case INTERPOLATION_TYPE_LINEAR:
		{
			Vector = (InverseFactor * Node1->Vector) + (Factor * Node2->Vector);
			break;
		}

		case INTERPOLATION_TYPE_HERMITE:
		{
			FLOAT Factor1;
			FLOAT Factor2;
			FLOAT Factor3;
			FLOAT Factor4;
			FLOAT FactorTimesTwo;

			FactorTimesTwo = Factor * Factor;

			Factor1 = FactorTimesTwo * (2.0f * Factor - 3.0f) + 1;
			Factor2 = FactorTimesTwo * (Factor - 2.0f) + Factor;
			Factor3 = FactorTimesTwo * (Factor - 1.0f);
			Factor4 = FactorTimesTwo * (3.0f - 2.0f * Factor);

			Vector = (Factor1 * Node1->Vector) + (Factor2 * Node1->OutTan) + (Factor3 * Node2->InTan) + (Factor4 * Node2->Vector);

			break;
		}

		case INTERPOLATION_TYPE_BEZIER:
		{
			FLOAT Factor1;
			FLOAT Factor2;
			FLOAT Factor3;
			FLOAT Factor4;
			FLOAT FactorTimesTwo;
			FLOAT InverseFactorTimesTwo;

			FactorTimesTwo = Factor * Factor;
			InverseFactorTimesTwo = InverseFactor * InverseFactor;

			Factor1 = InverseFactorTimesTwo * InverseFactor;
			Factor2 = 3.0f * Factor * InverseFactorTimesTwo;
			Factor3 = 3.0f * FactorTimesTwo * InverseFactor;
			Factor4 = FactorTimesTwo * Factor;

			Vector = (Factor1 * Node1->Vector) + (Factor2 * Node1->OutTan) + (Factor3 * Node2->InTan) + (Factor4 * Node2->Vector);

			break;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Retrieves the interpolated quaternion value
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::GetQuaternionValue(D3DXVECTOR4& Vector, CONST SEQUENCE_TIME& Time)
{
	FLOAT Factor;
	SEQUENCE_TIME TempTime;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;
	std::list<INTERPOLATOR_NODE>::iterator Node1;
	std::list<INTERPOLATOR_NODE>::iterator Node2;

	if(GlobalSequenceNode.IsAttached())
	{
		GlobalSequence = GlobalSequenceNode.GetObjectData();
		TempTime.IntervalStart = 0;
		TempTime.IntervalEnd = GlobalSequence->Data().Duration;
		TempTime.Time = GlobalSequence->Data().Time;
	}
	else
	{
		TempTime = Time;
	}

	GetSurroundingNodes(TempTime, Node1, Node2);

	if(Node1 == NodeList.end())
	{
		if(Node2 == NodeList.end())
		{
			Vector = DefaultVector;
		}
		else
		{
			Vector = Node2->Vector;
		}
		return;
	}

	if(Node2 == NodeList.end())
	{
		Vector = Node1->Vector;
		return;
	}

	if(Node1->Time >= Node2->Time)
	{
		Vector = Node1->Vector;
		return;
	}

	Factor = static_cast<FLOAT>(TempTime.Time - Node1->Time) / static_cast<FLOAT>(Node2->Time - Node1->Time);

	switch(InterpolationType)
	{
		case INTERPOLATION_TYPE_NONE:
		{
			Vector = Node1->Vector;
			break;
		}

		case INTERPOLATION_TYPE_LINEAR:
		{
			D3DXQUATERNION* Quaternion;
			D3DXQUATERNION* Quaternion1;
			D3DXQUATERNION* Quaternion2;

			Quaternion = reinterpret_cast<D3DXQUATERNION*>(&Vector);
			Quaternion1 = reinterpret_cast<D3DXQUATERNION*>(&Node1->Vector);
			Quaternion2 = reinterpret_cast<D3DXQUATERNION*>(&Node2->Vector);

			D3DXQuaternionSlerp(Quaternion, Quaternion1, Quaternion2, Factor);

			break;
		}

		case INTERPOLATION_TYPE_HERMITE:
		case INTERPOLATION_TYPE_BEZIER:
		{
			D3DXQUATERNION* Quaternion;
			D3DXQUATERNION* Quaternion1;
			D3DXQUATERNION* Quaternion2;
			D3DXQUATERNION* Quaternion3;
			D3DXQUATERNION* Quaternion4;
			D3DXQUATERNION QuaternionA;
			D3DXQUATERNION QuaternionB;
			D3DXQUATERNION QuaternionC;

			Quaternion = reinterpret_cast<D3DXQUATERNION*>(&Vector);
			Quaternion1 = reinterpret_cast<D3DXQUATERNION*>(&Node1->Vector);
			Quaternion2 = reinterpret_cast<D3DXQUATERNION*>(&Node1->InTan);
			Quaternion3 = reinterpret_cast<D3DXQUATERNION*>(&Node2->Vector);
			Quaternion4 = reinterpret_cast<D3DXQUATERNION*>(&Node2->OutTan);

			D3DXQuaternionSquadSetup(&QuaternionA, &QuaternionB, &QuaternionC, Quaternion1, Quaternion2, Quaternion3, Quaternion4);
			D3DXQuaternionSquad(Quaternion, Quaternion1, &QuaternionA, &QuaternionB, &QuaternionC, Factor);

			break;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Retrieves the surrounding nodes
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::GetSurroundingNodes(CONST SEQUENCE_TIME& Time, std::list<INTERPOLATOR_NODE>::iterator& Node1, std::list<INTERPOLATOR_NODE>::iterator& Node2)
{
	Node1 = NodeList.end();
	Node2 = NodeList.begin();

	while(TRUE)
	{
		if(Node2 == NodeList.end()) break;
		if(Time.Time < Node2->Time) break;

		Node1 = Node2;
		Node2++;
	}

	if((Node1 != NodeList.end()) && (Node1->Time < Time.IntervalStart)) Node1 = NodeList.end();
	if((Node2 != NodeList.end()) && (Node2->Time > Time.IntervalEnd)) Node2 = NodeList.end();
}

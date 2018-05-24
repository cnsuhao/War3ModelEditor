//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_CONSTANTS_H
#define MAGOS_CONSTANTS_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Definitions.h"
#include "Resource.h"


//+-----------------------------------------------------------------------------
//| Common constants
//+-----------------------------------------------------------------------------
CONST INT INVALID_INDEX       = -1;
CONST INT BUFFER_SIZE         = 2048;
CONST INT MENU_HISTORY_BASE   = 15000;
CONST INT MENU_FILTER_BASE    = 20000;
CONST INT DEFAULT_BLP_QUALITY = 90;
CONST WORD CURRENT_LANGUAGE   = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);


//+-----------------------------------------------------------------------------
//| Dimension constants
//+-----------------------------------------------------------------------------
CONST FLOAT DEFAULT_GROUND_TEXTURE_SCALE     = 250.0f;
CONST INT DEFAULT_ANIMATION_WINDOW_WIDTH     = 250;
CONST INT DEFAULT_ANIMATION_WINDOW_HEIGHT    = 400;
CONST INT DEFAULT_TEXTURE_WINDOW_WIDTH       = 256;
CONST INT DEFAULT_TEXTURE_WINDOW_HEIGHT      = 256;
CONST INT DEFAULT_MANAGER_WINDOW_WIDTH       = 250;
CONST INT DEFAULT_MANAGER_WINDOW_HEIGHT      = 400;
CONST INT DEFAULT_NODE_MANAGER_WINDOW_WIDTH  = 350;
CONST INT DEFAULT_NODE_MANAGER_WINDOW_HEIGHT = 550;
CONST INT DEFAULT_MAIN_WINDOW_WIDTH          = 640;
CONST INT DEFAULT_MAIN_WINDOW_HEIGHT         = 480;
CONST INT DEFAULT_MODEL_WINDOW_WIDTH         = 800;
CONST INT DEFAULT_MODEL_WINDOW_HEIGHT        = 600;
CONST INT DEFAULT_MPQ_WINDOW_WIDTH           = 450;
CONST INT DEFAULT_MPQ_WINDOW_HEIGHT          = 600;
CONST INT MIN_TEXTURE_WINDOW_WIDTH           = 64;
CONST INT MIN_TEXTURE_WINDOW_HEIGHT          = 64;
CONST INT MAX_TEXTURE_WINDOW_WIDTH           = 512;
CONST INT MAX_TEXTURE_WINDOW_HEIGHT          = 512;


//+-----------------------------------------------------------------------------
//| Timer constants
//+-----------------------------------------------------------------------------
CONST INT EDITOR_TIMER_FPS              = 25;
CONST INT EDITOR_TIMER_PERIOD           = 1000 / EDITOR_TIMER_FPS;
CONST DWORD EDITOR_TIMER_RESOLUTION     = 1;
CONST DWORD EDITOR_TIMER_MIN_SLEEP_TIME = 5;
CONST DWORD EDITOR_TIMER_MAX_SLEEP_TIME = 1000;


//+-----------------------------------------------------------------------------
//| Graphics constants
//+-----------------------------------------------------------------------------
CONST std::string GRAPHICS_WINDOW_CLASS_NAME = "MAGOS_GRAPHICS_WINDOW_CLASS";
CONST FLOAT GRAPHICS_DEFAULT_FIELD_OF_VIEW   = D3DX_PI / 4.0f;
CONST FLOAT GRAPHICS_DEFAULT_NEAR_DISTANCE   = 1.0f;
CONST FLOAT GRAPHICS_DEFAULT_FAR_DISTANCE    = 10000.0f;
CONST std::string PIXEL_SHADER_PROFILE       = "ps_2_0";
CONST WORD PIXEL_SHADER_MAJOR_VERSION        = 2;
CONST WORD PIXEL_SHADER_MINOR_VERSION        = 0;
CONST std::string VERTEX_SHADER_PROFILE      = "vs_1_1";
CONST WORD VERTEX_SHADER_MAJOR_VERSION       = 1;
CONST WORD VERTEX_SHADER_MINOR_VERSION       = 1;
CONST D3DCOLOR GRAPHICS_DEBUG_COLOR          = D3DCOLOR_ARGB(255, 255, 255, 255);


//+-----------------------------------------------------------------------------
//| Properties constants
//+-----------------------------------------------------------------------------
CONST D3DCOLOR PROPERTIES_NO_BACKGROUND       = D3DCOLOR_ARGB(0, 224, 223, 227);
CONST D3DCOLOR PROPERTIES_DEFAULT_BACKGROUND  = D3DCOLOR_ARGB(0, 90, 90, 90);
CONST D3DCOLOR PROPERTIES_DEFAULT_AMBIENT     = D3DCOLOR_ARGB(0, 128, 128, 128);
CONST D3DCOLOR PROPERTIES_DEFAULT_DIFFUSE     = D3DCOLOR_ARGB(0, 192, 192, 192);
CONST D3DCOLOR PROPERTIES_DEFAULT_SPECULAR    = D3DCOLOR_ARGB(0, 0, 0, 0);
CONST FLOAT PROPERTIES_DEFAULT_SPECULAR_POWER = 8.0f;


//+-----------------------------------------------------------------------------
//| Camera constants
//+-----------------------------------------------------------------------------
CONST FLOAT CAMERA_DEFAULT_PITCH            = D3DX_PI / 4.0f;
CONST FLOAT CAMERA_DEFAULT_YAW              = 0.0f;
CONST FLOAT CAMERA_DEFAULT_DISTANCE         = 200.0f;
CONST FLOAT CAMERA_DEFAULT_MIN_DISTANCE     = 200.0f;
CONST FLOAT CAMERA_MIN_PITCH                = -(D3DX_PI / 2.0f) + 0.01f;
CONST FLOAT CAMERA_MIN_DISTANCE             = 15.0f;
CONST FLOAT CAMERA_MAX_PITCH                = (D3DX_PI / 2.0f) - 0.01f;
CONST FLOAT CAMERA_MAX_DISTANCE             = 8000.0f;
CONST FLOAT CAMERA_FACTOR_PITCH             = 0.02f;
CONST FLOAT CAMERA_FACTOR_YAW               = 0.02f;
CONST FLOAT CAMERA_FACTOR_DISTANCE          = 0.002f;
CONST FLOAT CAMERA_FACTOR_MOVE              = 0.004f;
CONST FLOAT CAMERA_FACTOR_RELATIVE_DISTANCE = 500.0f;
CONST FLOAT CAMERA_FACTOR_RELATIVE_MOVE     = 500.0f;
CONST FLOAT CAMERA_FACTOR_MODEL             = 2.0f;


//+-----------------------------------------------------------------------------
//| Shader constants
//+-----------------------------------------------------------------------------
CONST std::string SHADER_WORLD_MATRIX                 = "WorldMatrix";
CONST std::string SHADER_VIEW_MATRIX                  = "ViewMatrix";
CONST std::string SHADER_PROJECTION_MATRIX            = "ProjectionMatrix";
CONST std::string SHADER_WORLD_VIEW_PROJECTION_MATRIX = "WorldViewProjectionMatrix";
CONST std::string SHADER_CAMERA_POSITION              = "CameraPosition";
CONST std::string SHADER_CAMERA_DIRECTION             = "CameraDirection";
CONST std::string SHADER_AMBIENT_COLOR                = "AmbientColor";
CONST std::string SHADER_DIFFUSE_COLOR                = "DiffuseColor";
CONST std::string SHADER_SPECULAR_COLOR               = "SpecularColor";
CONST std::string SHADER_SPECULAR_POWER               = "SpecularPower";


//+-----------------------------------------------------------------------------
//| Model constants
//+-----------------------------------------------------------------------------
CONST DWORD MODEL_DEFAULT_VERSION          = 800;
CONST INT MODEL_NAME_SIZE                  = 336;
CONST INT MODEL_NAME_NODE_SIZE             = 80;
CONST INT MODEL_NAME_CAMERA_SIZE           = 80;
CONST INT MODEL_NAME_TEXTURE_SIZE          = 256;
CONST INT MODEL_NAME_SEQUENCE_SIZE         = 80;
CONST INT MODEL_NAME_ATTACHMENT_SIZE       = 256;
CONST INT MODEL_NAME_PARTICLE_EMITTER_SIZE = 256;


//+-----------------------------------------------------------------------------
//| Color constants
//+-----------------------------------------------------------------------------
CONST D3DCOLOR COLOR_FACE                  = 0xFFDFDFDF;
CONST D3DCOLOR COLOR_VERTEX_SELECTED       = 0xFF00FF00;
CONST D3DCOLOR COLOR_VERTEX_DESELECTED     = 0xFFFF0000;
CONST D3DCOLOR COLOR_TEXT                  = 0xFFFFFFFF;
CONST D3DCOLOR COLOR_FRAME_SELECT          = 0xFF00FF00;
CONST D3DCOLOR COLOR_FRAME_DESELECT        = 0xFFFF0000;
CONST D3DCOLOR COLOR_PIVOT_POINT           = 0xFFFF00FF;
CONST D3DCOLOR COLOR_AXIS_X                = 0xFFFF0000;
CONST D3DCOLOR COLOR_AXIS_Y                = 0xFF00FF00;
CONST D3DCOLOR COLOR_AXIS_Z                = 0xFF0000FF;
CONST D3DCOLOR COLOR_BONE                  = 0xFF00FF00;
CONST D3DCOLOR COLOR_LIGHT                 = 0xFFFFFF00;
CONST D3DCOLOR COLOR_CAMERA                = 0xFFFF0000;
CONST D3DCOLOR COLOR_ATTACHMENT            = 0xFFFF00FF;
CONST D3DCOLOR COLOR_COLLISION_SHAPE       = 0xFF0000FF;
CONST D3DCOLOR COLOR_BACKGROUND_SELECTED   = 0xFF6D6D6D;
CONST D3DCOLOR COLOR_BACKGROUND_UNSELECTED = 0xFF5A5A5A;


//+-----------------------------------------------------------------------------
//| String constants
//+-----------------------------------------------------------------------------
CONST std::string STRING_DATE_FORMAT   = "dddd\',\' MMMM dd yyyy";
CONST std::string STRING_TIME_FORMAT   = "HH\':\'mm\':\'ss";
CONST std::string STRING_DEFAULT_EVENT = "SNDxAITM";


//+-----------------------------------------------------------------------------
//| Name constants
//+-----------------------------------------------------------------------------
CONST std::string DEFAULT_MODEL_NAME = "Untitled.mdl";
CONST std::string NAME_NO_ANIMATION  = "(Unanimated)";
CONST std::string NAME_NO_ID         = "(None)";
CONST std::string NAME_MULTIPLE_ID   = "(Multiple)";


//+-----------------------------------------------------------------------------
//| Path constants
//+-----------------------------------------------------------------------------
CONST std::string PATH_ICONS                  = "Icons\\";
CONST std::string PATH_LIST_FILES             = "ListFiles\\";
CONST std::string PATH_IMPORTERS              = "Importers\\";
CONST std::string PATH_TEMP                   = "Temp";
CONST std::string PATH_FILTERS                = "Data\\Filters.txt";
CONST std::string PATH_LISTFILE               = "(listfile)";
CONST std::string PATH_MPQ_WAR3               = "War3.mpq";
CONST std::string PATH_MPQ_WAR3X              = "War3x.mpq";
CONST std::string PATH_MPQ_WAR3X_LOCAL        = "War3xLocal.mpq";
CONST std::string PATH_MPQ_WAR3_PATCH         = "War3Patch.mpq";
CONST std::string PATH_ICON_FILES             = PATH_ICONS + "*.ico";
CONST std::string PATH_IMPORTER_FILES         = PATH_IMPORTERS + "*.dll";
CONST std::string PATH_VERTEX_SHADER          = "Data\\VertexShader.vsh";
CONST std::string PATH_PIXEL_SHADER_SHADED    = "Data\\PixelShaderShaded.psh";
CONST std::string PATH_PIXEL_SHADER_UNSHADED  = "Data\\PixelShaderUnshaded.psh";
CONST std::string PATH_PROPERTIES             = "Data\\Properties.txt";
CONST std::string PATH_MODEL_HISTORY          = "Data\\ModelHistory.txt";
CONST std::string PATH_REPLACEABLE_TEXTURES   = "ReplaceableTextures\\";
CONST std::string PATH_TEAM_COLOR             = "ReplaceableTextures\\TeamColor\\";
CONST std::string PATH_TEAM_GLOW              = "ReplaceableTextures\\TeamGlow\\";
CONST std::string PATH_GROUND_TEXTURE         = "TerrainArt\\Ashenvale\\Ashen_DirtGrass.blp";
CONST std::string PATH_REPLACEABLE_TEXTURE_11 = "ReplaceableTextures\\Cliff\\Cliff0.blp";
CONST std::string PATH_REPLACEABLE_TEXTURE_31 = "ReplaceableTextures\\LordaeronTree\\LordaeronSummerTree.blp";
CONST std::string PATH_REPLACEABLE_TEXTURE_32 = "ReplaceableTextures\\AshenvaleTree\\AshenTree.blp";
CONST std::string PATH_REPLACEABLE_TEXTURE_33 = "ReplaceableTextures\\BarrensTree\\BarrensTree.blp";
CONST std::string PATH_REPLACEABLE_TEXTURE_34 = "ReplaceableTextures\\NorthrendTree\\NorthTree.blp";
CONST std::string PATH_REPLACEABLE_TEXTURE_35 = "ReplaceableTextures\\Mushroom\\MushroomTree.blp";
CONST std::string PATH_DATA_SPLAT             = "Data\\SplatData.txt";
CONST std::string PATH_DATA_UBERSPLAT         = "Data\\UberSplatData.txt";
CONST std::string PATH_DATA_SOUND             = "Data\\SoundData.txt";
CONST std::string PATH_DATA_SPAWNOBJECT       = "Data\\SpawnObjectData.txt";


//+-----------------------------------------------------------------------------
//| Filter constants
//+-----------------------------------------------------------------------------
CONST CHAR* CONST FILTER_ALL     = "All Files (*.*)\0*.*\0\0";
CONST CHAR* CONST FILTER_GEOSET  = "Geoset Files (*.geo)\0*.geo;*.w3n\0" \
								   "All Files (*.*)\0*.*\0\0";
CONST CHAR* CONST FILTER_MPQ     = "MPQ Archives (*.mpq;*.w3m;*.w3x;*.w3n)\0*.mpq;*.w3m;*.w3x;*.w3n\0" \
								   "All Files (*.*)\0*.*\0\0";
CONST CHAR* CONST FILTER_MODEL   = "Warcraft Models (*.mdl;*.mdx)\0*.mdl;*.mdx\0" \
								   "All Files (*.*)\0*.*\0\0";
CONST CHAR* CONST FILTER_TEXTURE = "Textures (*.bmp;*.tga;*.png;*.jpg;*.jpeg;*.pcx;*.blp)\0*.bmp;*.tga;*.png;*.jpg;*.jpeg;*.pcx;*.blp\0" \
								   "All Files (*.*)\0*.*\0\0";
CONST CHAR* CONST FILTER_BOTH    = "Warcraft Models or Textures (*.mdl;*.mdx;*.bmp;*.tga;*.png;*.jpg;*.jpeg;*.pcx;*.blp)\0*.mdl;*.mdx;*.bmp;*.tga;*.png;*.jpg;*.jpeg;*.pcx;*.blp\0" \
								   "Warcraft Models (*.mdl;*.mdx)\0*.mdl;*.mdx\0" \
								   "Textures (*.bmp;*.tga;*.png;*.jpg;*.jpeg;*.pcx;*.blp)\0*.bmp;*.tga;*.png;*.jpg;*.jpeg;*.pcx;*.blp\0" \
								   "All Files (*.*)\0*.*\0\0";


//+-----------------------------------------------------------------------------
//| Editor text constants
//+-----------------------------------------------------------------------------
CONST std::string EDITOR_VERSION      = "1.05";
CONST std::string EDITOR_NAME         = "War3 Model Editor v" + EDITOR_VERSION;
CONST std::string EDITOR_MPQ_NAME     = "MPQ Browser";
CONST std::string EDITOR_HELP_TITLE   = "About";
CONST std::string EDITOR_HELP_MESSAGE = EDITOR_NAME + "\n" \
										"Made by Magnus Ostberg (aka Magos)\n" \
										"MagosX@GMail.com\n" \
										"(C) Copyright 2005";


//+-----------------------------------------------------------------------------
//| Filter mode enumeration
//+-----------------------------------------------------------------------------
enum FILTER_MODE
{
	FILTER_MODE_NONE,
	FILTER_MODE_TRANSPARENT,
	FILTER_MODE_BLEND,
	FILTER_MODE_ADDITIVE,
	FILTER_MODE_ADD_ALPHA,
	FILTER_MODE_MODULATE,
};


//+-----------------------------------------------------------------------------
//| Node type enumeration
//+-----------------------------------------------------------------------------
enum NODE_TYPE
{
	NODE_TYPE_NONE,
	NODE_TYPE_ATTACHMENT,
	NODE_TYPE_BONE,
	NODE_TYPE_EVENT_OBJECT,
	NODE_TYPE_COLLISION_SHAPE,
	NODE_TYPE_HELPER,
	NODE_TYPE_LIGHT,
	NODE_TYPE_PARTICLE_EMITTER,
	NODE_TYPE_PARTICLE_EMITTER_2,
	NODE_TYPE_RIBBON_EMITTER,
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

# Microsoft Developer Studio Project File - Name="MyEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MyEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MyEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MyEditor.mak" CFG="MyEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MyEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MyEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MyEditor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx9.lib d3d9.lib dinput8.lib dxguid.lib winmm.lib comctl32.lib StormLibStatic.lib libjpeg.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MyEditor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MyEditor___Win32_Debug"
# PROP BASE Intermediate_Dir "MyEditor___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx9.lib d3d9.lib dinput8.lib dxguid.lib winmm.lib comctl32.lib StormLibStatic.lib libjpeg.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Bin/MyEditor.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MyEditor - Win32 Release"
# Name "MyEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnimationWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\ConstantTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DataInStream.cpp
# End Source File
# Begin Source File

SOURCE=.\DataOutStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Error.cpp
# End Source File
# Begin Source File

SOURCE=.\EventObjectManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\FileLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashingGeoset.cpp
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\GeosetAnimationManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\GeosetManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSequenceManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicsWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Icon.cpp
# End Source File
# Begin Source File

SOURCE=.\Interpolator.cpp
# End Source File
# Begin Source File

SOURCE=.\Jpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MaterialManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Model.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelAttachment.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelBone.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelCollisionShape.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelEventObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGeoset.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetFace.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetVertex.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGlobalSequence.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelImporter.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelLight.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelLoaderMdl.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelLoaderMdx.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelMaterialLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelModel.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelParticleEmitter.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelParticleEmitter2.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelRibbonEmitter.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelSequence.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelTextureAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Mpq.cpp
# End Source File
# Begin Source File

SOURCE=.\MpqWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEditor.rc
# End Source File
# Begin Source File

SOURCE=.\NodeIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectIdManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Particle.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticleManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PixelShader.cpp
# End Source File
# Begin Source File

SOURCE=.\Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Random.cpp
# End Source File
# Begin Source File

SOURCE=.\Register.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Ribbon.cpp
# End Source File
# Begin Source File

SOURCE=.\RibbonManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SequenceManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TeamColor.cpp
# End Source File
# Begin Source File

SOURCE=.\TemporaryFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureAnimationManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureLoaderBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureLoaderBlp.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureLoaderPcx.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureManagerWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\TokenInStream.cpp
# End Source File
# Begin Source File

SOURCE=.\TokenOutStream.cpp
# End Source File
# Begin Source File

SOURCE=.\VertexShader.cpp
# End Source File
# Begin Source File

SOURCE=.\Window.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowAttachmentDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowBoneDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowButton.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowCameraDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowCollisionShapeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowColoredTextDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowCombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowEventObjectDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowExtentDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowExtentsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowFaceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowFacesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowGeosetAnimationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowGeosetDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowGlobalSequenceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowGroupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowGroupsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowHelperDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowImportGeosetDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowInterpolatorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowLightDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowListbox.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowLoadingScreenDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowMaterialDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowMaterialLayerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowModelInfoDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowModelPropertiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowNodeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowParticleEmitter2Dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowParticleEmitterDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowPropertiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowQualityDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowRegisterDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowRibbonEmitterDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowRotateDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowScaleDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowSequenceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowTextureAnimationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowTextureDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowTranslateDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowTreeview.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowVertexDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowVerticesDialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AnimationWindow.h
# End Source File
# Begin Source File

SOURCE=.\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\CameraManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\Constants.h
# End Source File
# Begin Source File

SOURCE=.\ConstantTable.h
# End Source File
# Begin Source File

SOURCE=.\Container.h
# End Source File
# Begin Source File

SOURCE=.\DataInStream.h
# End Source File
# Begin Source File

SOURCE=.\DataOutStream.h
# End Source File
# Begin Source File

SOURCE=.\Definitions.h
# End Source File
# Begin Source File

SOURCE=.\Error.h
# End Source File
# Begin Source File

SOURCE=.\EventObjectManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\Extent.h
# End Source File
# Begin Source File

SOURCE=.\FileLoader.h
# End Source File
# Begin Source File

SOURCE=.\Filter.h
# End Source File
# Begin Source File

SOURCE=.\FlashingGeoset.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\GeosetAnimationManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\GeosetManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\GlobalSequenceManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\Graphics.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsInfo.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsWindow.h
# End Source File
# Begin Source File

SOURCE=.\Icon.h
# End Source File
# Begin Source File

SOURCE=.\Interpolator.h
# End Source File
# Begin Source File

SOURCE=.\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\Keyboard.h
# End Source File
# Begin Source File

SOURCE=.\MainWindow.h
# End Source File
# Begin Source File

SOURCE=.\ManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\MaterialManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Misc.h
# End Source File
# Begin Source File

SOURCE=.\Model.h
# End Source File
# Begin Source File

SOURCE=.\ModelAttachment.h
# End Source File
# Begin Source File

SOURCE=.\ModelBase.h
# End Source File
# Begin Source File

SOURCE=.\ModelBone.h
# End Source File
# Begin Source File

SOURCE=.\ModelBuilder.h
# End Source File
# Begin Source File

SOURCE=.\ModelCamera.h
# End Source File
# Begin Source File

SOURCE=.\ModelCollisionShape.h
# End Source File
# Begin Source File

SOURCE=.\ModelEventObject.h
# End Source File
# Begin Source File

SOURCE=.\ModelGeoset.h
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetAnimation.h
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetFace.h
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetGroup.h
# End Source File
# Begin Source File

SOURCE=.\ModelGeosetVertex.h
# End Source File
# Begin Source File

SOURCE=.\ModelGlobalSequence.h
# End Source File
# Begin Source File

SOURCE=.\ModelHelper.h
# End Source File
# Begin Source File

SOURCE=.\ModelHistory.h
# End Source File
# Begin Source File

SOURCE=.\ModelImporter.h
# End Source File
# Begin Source File

SOURCE=.\ModelLight.h
# End Source File
# Begin Source File

SOURCE=.\ModelLoader.h
# End Source File
# Begin Source File

SOURCE=.\ModelLoaderMdl.h
# End Source File
# Begin Source File

SOURCE=.\ModelLoaderMdx.h
# End Source File
# Begin Source File

SOURCE=.\ModelMaterial.h
# End Source File
# Begin Source File

SOURCE=.\ModelMaterialLayer.h
# End Source File
# Begin Source File

SOURCE=.\ModelModel.h
# End Source File
# Begin Source File

SOURCE=.\ModelParticleEmitter.h
# End Source File
# Begin Source File

SOURCE=.\ModelParticleEmitter2.h
# End Source File
# Begin Source File

SOURCE=.\ModelRibbonEmitter.h
# End Source File
# Begin Source File

SOURCE=.\ModelSequence.h
# End Source File
# Begin Source File

SOURCE=.\ModelTexture.h
# End Source File
# Begin Source File

SOURCE=.\ModelTextureAnimation.h
# End Source File
# Begin Source File

SOURCE=.\ModelWindow.h
# End Source File
# Begin Source File

SOURCE=.\Mouse.h
# End Source File
# Begin Source File

SOURCE=.\Mpq.h
# End Source File
# Begin Source File

SOURCE=.\MpqWindow.h
# End Source File
# Begin Source File

SOURCE=.\NodeIcon.h
# End Source File
# Begin Source File

SOURCE=.\NodeManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\ObjectIdManager.h
# End Source File
# Begin Source File

SOURCE=.\Particle.h
# End Source File
# Begin Source File

SOURCE=.\ParticleManager.h
# End Source File
# Begin Source File

SOURCE=.\PixelShader.h
# End Source File
# Begin Source File

SOURCE=.\PreIncludes.h
# End Source File
# Begin Source File

SOURCE=.\Properties.h
# End Source File
# Begin Source File

SOURCE=.\Random.h
# End Source File
# Begin Source File

SOURCE=.\Reference.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceObject.h
# End Source File
# Begin Source File

SOURCE=.\Register.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResourceLoader.h
# End Source File
# Begin Source File

SOURCE=.\Ribbon.h
# End Source File
# Begin Source File

SOURCE=.\RibbonManager.h
# End Source File
# Begin Source File

SOURCE=.\SequenceManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\SequenceTime.h
# End Source File
# Begin Source File

SOURCE=.\SimpleContainer.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StormLib.h
# End Source File
# Begin Source File

SOURCE=.\StormLibStatic.h
# End Source File
# Begin Source File

SOURCE=.\TeamColor.h
# End Source File
# Begin Source File

SOURCE=.\TemporaryFile.h
# End Source File
# Begin Source File

SOURCE=.\Texture.h
# End Source File
# Begin Source File

SOURCE=.\TextureAnimationManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\TextureLoader.h
# End Source File
# Begin Source File

SOURCE=.\TextureLoaderBasic.h
# End Source File
# Begin Source File

SOURCE=.\TextureLoaderBlp.h
# End Source File
# Begin Source File

SOURCE=.\TextureLoaderPcx.h
# End Source File
# Begin Source File

SOURCE=.\TextureManager.h
# End Source File
# Begin Source File

SOURCE=.\TextureManagerWindow.h
# End Source File
# Begin Source File

SOURCE=.\TextureWindow.h
# End Source File
# Begin Source File

SOURCE=.\Timer.h
# End Source File
# Begin Source File

SOURCE=.\TokenInStream.h
# End Source File
# Begin Source File

SOURCE=.\TokenOutStream.h
# End Source File
# Begin Source File

SOURCE=.\Vertex.h
# End Source File
# Begin Source File

SOURCE=.\VertexShader.h
# End Source File
# Begin Source File

SOURCE=.\War3ModelImporter\War3ModelImporter.h
# End Source File
# Begin Source File

SOURCE=.\Window.h
# End Source File
# Begin Source File

SOURCE=.\WindowAttachmentDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowBoneDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowButton.h
# End Source File
# Begin Source File

SOURCE=.\WindowCameraDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowCollisionShapeDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowColoredTextDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowCombobox.h
# End Source File
# Begin Source File

SOURCE=.\WindowDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowEdit.h
# End Source File
# Begin Source File

SOURCE=.\WindowEventObjectDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowExtentDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowExtentsDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowFaceDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowFacesDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowFrame.h
# End Source File
# Begin Source File

SOURCE=.\WindowGeosetAnimationDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowGeosetDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowGlobalSequenceDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowGroupDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowGroupsDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowHelperDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowImportGeosetDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowInterpolatorDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowLightDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowListbox.h
# End Source File
# Begin Source File

SOURCE=.\WindowLoadingScreenDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowMaterialDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowMaterialLayerDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowModelInfoDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowModelPropertiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowNodeDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowParticleEmitter2Dialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowParticleEmitterDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowPropertiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowQualityDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowRegisterDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowRibbonEmitterDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowRotateDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowScaleDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowSequenceDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowTextureAnimationDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowTextureDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowTranslateDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowTreeview.h
# End Source File
# Begin Source File

SOURCE=.\WindowVertexDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowVerticesDialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ProgramIcons\_War3ModelEditor.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\archive.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\attachment.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\bone.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\collisionshape.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\eventobject.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\folder.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\helper.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\image.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\light.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\model.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\particleemitter.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\particleemitter2.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\ribbonemitter.ico
# End Source File
# Begin Source File

SOURCE=.\ProgramIcons\unknown.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

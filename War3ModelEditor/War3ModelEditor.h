//+-----------------------------------------------------------------------------
//|
//| File:    War3ModelEditor
//| Version: 1.05
//| Date:    2005-10-21
//| Author:  Magnus Ostberg (aka Magos)
//| Mail:    MagosX@Gmail.com
//| Url:     http://Magos.TheJeffFiles.com/War3ModelEditor/
//|
//| Description:
//|          A model viewer and editor for Warcraft 3 models.
//|          Supports loading and saving of the mdl and mdx model formats.
//|          Also suppports loading and saving of the bmp, tga, png, jpg,
//|          jpeg, pcx and blp texture formats. Check the help file for
//|          more information.
//|
//| Version history:
//|          0.00 (2005-02-24)
//|          Ideas and early test versions!
//|
//|          0.00 (2005-08-13)
//|          Project started for real! Basic structure
//|          modified from earlier test versions.
//|
//|          0.01 BETA (2005-09-27)
//|          First BETA version released.
//|
//|          0.02 BETA (2005-10-01)
//|          Fixed buggy MDL/MDX saving and loading. Added Model History
//|          and Loading Screen Creator. General bug fixing.
//|
//|          0.03 BETA (2005-10-02)
//|          Added geoset importing/exporting, added collision shape display,
//|          option to automatically calculate all extents.
//|
//|          0.04 BETA (2005-10-06)
//|          Added model importers, fixed two very serious animation bugs.
//|          Added a feature that flashes the geoset you CTRL-click on.
//|          Also added a PCX loader.
//|
//|          0.05 BETA (2005-10-10)
//|          Fixed BLP and Model-far-away-vertex bugs.
//|
//|          1.00 (2005-10-11)
//|          War 3 Model Editor has gone into a public release!
//|          Added a "Create Camera from Current View" option.
//|          Fixed a collision shape save bug. Added a link to
//|          the help file from the main menu.
//|
//|          1.01 (2005-10-13)
//|          Changed how the initial camera distance works (to avoid close
//|          zooms on particle-emitter-only models). Fixed RGB-BGR bug.
//|          Added a MS3D (Milkshape) importer. Geosets are slightly more
//|          editable now.
//|
//|          1.02 (2005-10-14)
//|          Fixed a follow-up error on the last RGB-BGR bug fix. Added
//|          a coordinate system correction for the MS3D importer.
//|
//|          1.03 (2005-10-19)
//|          Bug-fixes, rotation changes in the MS3D importer. Can zoom
//|          by holding ALT while dragging the mouse.
//|
//|          1.04 (2005-10-21)
//|          Added a first early version of the Model-Geoset Editor
//|          (cannot add/remove stuff yet, just manipulate).
//|
//|          1.05 (2005-10-21)
//|          Changed the blend mode so it doesn't write to the Z-buffer.
//|          Implemented material layer alpha so it's correctly displayed.
//|          Particle emitters generally improved with visuals and
//|          appearance.
//|
//| Known bugs:
//|          None
//|
//| Notes:
//|          The source code is NOT fully compilable. It is meant as an
//|          educational source, not something to be recompiled and
//|          released under another author.
//|
//| Linked files:
//|          d3dx9.lib
//|          d3d9.lib
//|          dinput8.lib
//|          dxguid.lib
//|          winmm.lib
//|          comctl32.lib
//|          libjpeg.lib
//|          StormLibStatic.lib
//|
//| Disclaimer:
//|          Source code is available for educational purposes!
//|          The code may NOT be used for commercial applications
//|          nor in any way be used for profit.
//|
//|          The source code is compiled and/or executed at your own risk!
//|          I take no responsibilities whatsoever for any damaged hardware
//|          and/or software caused by use of this program.
//|
//| Credits:
//|          CProgramming.com       - http://www.cprogramming.com/cboard
//|          Wc3Campaigns.com       - http://www.wc3campaigns.com
//|          Gamedev.net            - http://www.gamedev.net
//|          Independent JPEG Group - http://www.ijg.org
//|          Clan MoMM              - http://www.seiken.co.uk/momm/forum
//|          ShadowFlare (MPQ lib)  - http://shadowflare.gameproc.com
//|          TheJeffFiles.com       - http://TheJeffFiles.com
//|
//| Special thanks:
//|          Jeff Verkoeyen         - Web Hosting (TheJeffFiles.com)
//|          Alfred                 - MDX Format
//|          Jimmy "Nub" Campbell   - MDX Format
//|          ManZero                - Testing and Suggestions
//|          EdwardSwolenToe        - Testing and Suggestions
//|          ProtoZergling          - Testing and Suggestions
//|
//+-----------------------------------------------------------------------------


//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WAR3_MODEL_EDITOR_H
#define MAGOS_WAR3_MODEL_EDITOR_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Function prototypes
//+-----------------------------------------------------------------------------
BOOL Setup();
VOID Shutdown();
BOOL HandleWindowsMessages();


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif

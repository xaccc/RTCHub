#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for WebrtcEngine
#
#\**********************************************************/

set(PLUGIN_NAME "WebrtcEngine")
set(PLUGIN_PREFIX "WEN")
set(COMPANY_NAME "USKEE")

# ActiveX constants:
set(FBTYPELIB_NAME WebrtcEngineLib)
set(FBTYPELIB_DESC "WebrtcEngine 1.0 Type Library")
set(IFBControl_DESC "WebrtcEngine Control Interface")
set(FBControl_DESC "WebrtcEngine Control Class")
set(IFBComJavascriptObject_DESC "WebrtcEngine IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "WebrtcEngine ComJavascriptObject Class")
set(IFBComEventSource_DESC "WebrtcEngine IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID e179657e-6f92-54bd-b1b5-9c61942699bd)
set(IFBControl_GUID 4079ec12-89b5-5ab6-9a60-c7eee6eda514)
set(FBControl_GUID 
    d98a4ba2-60e7-5b87-bb35-b83b3cff8420
    d66bfd5e-80d4-5e3d-b22d-3ea6648fb14a
    )
set(IFBComJavascriptObject_GUID 1f20c7c5-88e5-578c-a564-e33ed2ef4d88)
set(FBComJavascriptObject_GUID 91a6d8e2-dee8-56ed-aa32-ee48c8047107)
set(IFBComEventSource_GUID 9339ba15-174c-5dd1-9c8b-126b782ffbfd)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID eb4719fa-cefe-5ad1-a06b-b6736349a0da)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 12af29e0-ab44-5d69-a933-5d914eb083d0)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID 
    "USKEE.WebrtcEngine"
    "USKEE.WebrtcRenderer"
    )
set(MOZILLA_PLUGINID "uskee.org/WebrtcEngine")

# strings
set(FBSTRING_CompanyName "USKEE")
set(FBSTRING_PluginDescription 
    "Webrtc Engine for stream/track/ICE"
    "Webrtc Renderer for Video"
    )
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2013 USKEE")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "WebrtcEngine")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "WebrtcEngine")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "WebrtcEngine_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType 
    "application/x-webrtcengine"
    "application/x-webrtcrenderer"
    )

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

# set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)

# Add logging library
add_firebreath_library(log4cplus)

# Add boost's filesystem library
add_boost_library(filesystem)
#/**********************************************************\ 
# Auto-generated Windows project definition file for the
# WebrtcEngine project
#\**********************************************************/

# Windows template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in Win/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Win/[^.]*.cpp
    Win/[^.]*.h
    Win/[^.]*.cmake
    )

# use this to add preprocessor definitions
add_definitions(
    /D "_ATL_STATIC_REGISTRY"
    /D "WIN32"
    /D "IRENDER_WIN"
)

SOURCE_GROUP(Win FILES ${PLATFORM})

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

add_windows_plugin(${PROJECT_NAME} SOURCES)

# This is an example of how to add a build step to sign the plugin DLL before
# the WiX installer builds.  The first filename (certificate.pfx) should be
# the path to your pfx file.  If it requires a passphrase, the passphrase
# should be located inside the second file. If you don't need a passphrase
# then set the second filename to "".  If you don't want signtool to timestamp
# your DLL then make the last parameter "".
#
# Note that this will not attempt to sign if the certificate isn't there --
# that's so that you can have development machines without the cert and it'll
# still work. Your cert should only be on the build machine and shouldn't be in
# source control!
# -- uncomment lines below this to enable signing --
#firebreath_sign_plugin(${PROJECT_NAME}
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/certificate.pfx"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/passphrase.txt"
#    "http://timestamp.verisign.com/scripts/timestamp.dll")

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!

find_library(LIBWEBRTC_01 jsoncpp.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_02 libjingle_peerconnection.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_03 libjingle.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_04 expat.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_05 crnss.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_06 nss_static.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_07 crnspr.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_08 sqlite3.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_09 icui18n.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_10 icuuc.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_11 libjingle_media.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_12 libyuv.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_13 libjpeg.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_14 video_capture_module.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_15 webrtc_utility.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_16 audio_coding_module.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_17 CNG.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_18 signal_processing.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_19 system_wrappers.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_20 G711.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_21 G722.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_22 iLBC.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_23 iSAC.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_24 iSACFix.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_25 PCM16B.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_26 NetEq.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_27 resampler.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_28 vad.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_29 webrtc_opus.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_30 opus.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_31 webrtc_video_coding.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_32 webrtc_i420.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_33 common_video.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_34 video_coding_utility.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_35 webrtc_vp8.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_36 libvpx.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_37 libvpx_asm_offsets.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_38 libvpx_intrinsics.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_39 libvpx_asm_offsets_vp9.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_40 directshow_baseclasses.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_41 video_render_module.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_42 video_engine_core.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_43 media_file.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_44 rtp_rtcp.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_45 remote_bitrate_estimator.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_46 paced_sender.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_47 udp_transport.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_48 bitrate_controller.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_49 video_processing.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_50 video_processing_sse2.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_51 voice_engine_core.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_52 audio_conference_mixer.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_53 audio_processing.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_54 audioproc_debug_proto.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_55 protobuf_lite.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_56 audio_processing_sse2.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_57 audio_device.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_58 libjingle_sound.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_59 libjingle_p2p.lib ../../../../third_party/libjingle/trunk/build/Release/lib)
find_library(LIBWEBRTC_60 libsrtp.lib ../../../../third_party/libjingle/trunk/build/Release/lib)

target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    secur32.lib
    msdmo.lib
    dmoguids.lib
    wmcodecdspuuid.lib
    ${LIBWEBRTC_01}
    ${LIBWEBRTC_02}
    ${LIBWEBRTC_03}
    ${LIBWEBRTC_04}
    ${LIBWEBRTC_05}
    ${LIBWEBRTC_06}
    ${LIBWEBRTC_07}
    ${LIBWEBRTC_08}
    ${LIBWEBRTC_09}
    ${LIBWEBRTC_10}
    ${LIBWEBRTC_11}
    ${LIBWEBRTC_12}
    ${LIBWEBRTC_13}
    ${LIBWEBRTC_14}
    ${LIBWEBRTC_15}
    ${LIBWEBRTC_16}
    ${LIBWEBRTC_17}
    ${LIBWEBRTC_18}
    ${LIBWEBRTC_19}
    ${LIBWEBRTC_20}
    ${LIBWEBRTC_21}
    ${LIBWEBRTC_22}
    ${LIBWEBRTC_23}
    ${LIBWEBRTC_24}
    ${LIBWEBRTC_25}
    ${LIBWEBRTC_26}
    ${LIBWEBRTC_27}
    ${LIBWEBRTC_28}
    ${LIBWEBRTC_29}
    ${LIBWEBRTC_30}
    ${LIBWEBRTC_31}
    ${LIBWEBRTC_32}
    ${LIBWEBRTC_33}
    ${LIBWEBRTC_34}
    ${LIBWEBRTC_35}
    ${LIBWEBRTC_36}
    ${LIBWEBRTC_37}
    ${LIBWEBRTC_38}
    ${LIBWEBRTC_39}
    ${LIBWEBRTC_40}
    ${LIBWEBRTC_41}
    ${LIBWEBRTC_42}
    ${LIBWEBRTC_43}
    ${LIBWEBRTC_44}
    ${LIBWEBRTC_45}
    ${LIBWEBRTC_46}
    ${LIBWEBRTC_47}
    ${LIBWEBRTC_48}
    ${LIBWEBRTC_49}
    ${LIBWEBRTC_50}
    ${LIBWEBRTC_51}
    ${LIBWEBRTC_52}
    ${LIBWEBRTC_53}
    ${LIBWEBRTC_54}
    ${LIBWEBRTC_55}
    ${LIBWEBRTC_56}
    ${LIBWEBRTC_57}
    ${LIBWEBRTC_58}
    ${LIBWEBRTC_59}
    ${LIBWEBRTC_60}	
    )

set(WIX_HEAT_FLAGS
    -gg                 # Generate GUIDs
    -srd                # Suppress Root Dir
    -cg PluginDLLGroup  # Set the Component group name
    -dr INSTALLDIR      # Set the directory ID to put the files in
    )

add_wix_installer( ${PLUGIN_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/Win/WiX/WebrtcEngineInstaller.wxs
    PluginDLLGroup
    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/
    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${FBSTRING_PluginFileName}.dll
    ${PROJECT_NAME}
    )

# This is an example of how to add a build step to sign the WiX installer
# -- uncomment lines below this to enable signing --
#firebreath_sign_file("${PLUGIN_NAME}_WiXInstall"
#    "${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}.msi"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/certificate.pfx"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/passphrase.txt"
#    "http://timestamp.verisign.com/scripts/timestamp.dll")

# This is an example of how to create a cab
# -- uncomment lines below this to enable signing --
#create_cab(${PLUGIN_NAME}
#    ${CMAKE_CURRENT_SOURCE_DIR}/Win/Wix/WebrtcEngine.ddf
#    ${CMAKE_CURRENT_SOURCE_DIR}/Win/Wix/WebrtcEngine.inf
#    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/
#    ${PROJECT_NAME}_WiXInstallExe
#    )

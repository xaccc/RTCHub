#!/bin/bash
# uskee.org

# project source path
ROOT=`pwd`/..

PLATFORM=`uname`

echox() 
{
    if [ $# -ne 2 ]; then
        echo "usage: echox color string"
        exit 1
    else
        color=$1
        shift
        echo -e "\033[${color}m$*\033[00m"; 
    fi
}

echor() { echox 31 "$*"; }
echog() { echox 32 "$*"; }
echoy() { echox 33 "$*"; }
echob() { echox 34 "$*"; }


check_error()
{
    if [ $? != 0 ]; then
        echor "[*] Error and exit!!!, reason="$1
        exit 1
    fi
}

check_depended()
{
    echoy "Check depended tools ..."
    cd $ROOT/scripts
    ./checkdeps.sh
    check_error "fail to checkdeps.sh"
}

config_firebreath()
{
    echoy "Getting FireBreath from its repo into third_party/FireBreath .."
    mkdir -p $ROOT/third_party/
    cd $ROOT/third_party/
    if [ ! -e FireBreath ]; then
        git clone http://github.com/PeterXu/FireBreath.git
        git submodule init
        git submodule update
        check_error "fail to git submodule for FireBreath"
    fi
}

config_jingle()
{
    obj_path=$ROOT/third_party/libjingle
    echoy "Getting libjingle from its repo into $obj_path ..."
    mkdir -p $obj_path
    cd $obj_path
    if [ ! -e trunk ]; then
        gclient config --name=trunk http://libjingle.googlecode.com/svn/trunk
        gclient sync -r 303 --force >/tmp/svn_co.log 2>&1
        check_error "fail to gclient sync for libjingle!"
    fi
    #python ../../scripts/patch.py -p1 --verbose ../../third_party_mods/common/libjingle_build.diff
}

build_jingle()
{
    obj_path=$ROOT/third_party/libjingle

    echoy "Do gyp_chromium on libjingle_all.gyp from $obj_path ..."
    cd $obj_path
    python trunk/build/gyp_chromium --depth=trunk trunk/talk/libjingle_all.gyp
    #python trunk/build/gyp_chromium --depth=trunk trunk/third_party/webrtc/build/merge_libs.gyp
    check_error "fail to gyp_chromium for libjingle_all.gyp"

    # for jingle
    targets="libjingle libjingle_p2p libjingle_media libjingle_peerconnection libjingle_sound libjingle_xmpphelp peerconnection_server "

    # for webrtc misc
    targets=$targets"bitrate_controller remote_bitrate_estimator rtp_rtcp udp_transport webrtc_utility system_wrappers "
    targets=$targets"libjpeg libsrtp protobuf_full_do_not_use protobuf_lite protoc libyuv "

    # for webrtc audio
    targets=$targets"resampler signal_processing vad CNG "
    targets=$targets"G711 G722 NetEq NetEq4 PCM16B iLBC iSAC iSACFix opus "
    targets=$targets"audio_coding_module audio_conference_mixer audio_device audio_processing "
    targets=$targets"voice_engine_core media_file paced_sender audio_processing_sse2 webrtc_opus "

    # for webrtc video
    targets=$targets"common_video video_capture_module webrtc_vp8 video_coding_utility video_processing video_processing_sse2 "
    targets=$targets"webrtc_video_coding video_engine_core video_render_module webrtc_i420 "
    targets=$targets"gen_asm_offsets_vp8 gen_asm_offsets_vp9 gen_asm_offsets_vpx_scale "
    targets=$targets"libvpx libvpx_asm_offsets libvpx_asm_offsets_vp9 libvpx_asm_offsets_vpx_scale "

    cd $obj_path/trunk
    for target in $targets
    do
        if [ $PLATFORM = 'Darwin' ]; then
            xcodebuild -arch i386 -sdk macosx10.7 -configuration $bld_type -project talk/libjingle_all.xcodeproj -target=$target GCC_ENABLE_CPP_RTTI=YES
        else
            make BUILDTYPE=$bld_type $target 2>>/tmp/jingle_build.log 1>&2
        fi
        check_error "fail to build [$target]"
    done
}

# for preparing building env
build_prepare()
{
    check_depended
    config_firebreath
    config_jingle
}

# for webrtc lib
#thelibs="1 2 "
makearchive ()
{
    if [ $PLATFORM = "Linux" ]; then
        for k in $thelibs
        do
            if [ ! -e $k ]; then
                echor "Can not find $k!"
                continue
            fi
            #echo "Processing $k ..."
            ar t $k | xargs ar qc libwebrtc_$1.a
        done
        echo "Adding symbol table to archive."
        ar sv libwebrtc_$1.a
    else
        libtool -static -arch_only i386 -o libwebrtc_$1.a ${thelibs[@]:0}
    fi
}

build_webrtc()
{
    if [ $PLATFORM = "Darwin" ]; then
        LOCROOT=$ROOT/third_party/libjingle/trunk/xcodebuild/
    else
        LOCROOT=$ROOT/third_party/libjingle/trunk/out/
    fi

    mkdir -p $LOCROOT/$bld_type
    cd $LOCROOT/$bld_type
    rm -f libwebrtc_$bld_type.a
    if [ $PLATFORM = "Darwin" ]; then
        thelibs=`find . -name "lib*.a" -print`
    else
        thelibs=`find obj.target/ -name lib*.a`
    fi
    makearchive $bld_type
}

build_plugin()
{
    if [ $# -ne 1 ]; then
        echor "usage: $0 plugin"
        exit 1
    fi

    cd $ROOT
    rm -f src/js_api/build/projects/$1/CMakeFiles/$1.dir/*.o
    if [ $PLATFORM = 'Darwin' ]; then
        third_party/FireBreath/prepmac.sh src/js_api/projects src/js_api/build -D CMAKE_OSX_ARCHITECTURES="i386" -D CMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk" -D CMAKE_OSX_DEPLOYMENT_TARGET=10.7
    else
        third_party/FireBreath/prepmake.sh src/js_api/projects src/js_api/build
    fi

    cd $ROOT/src/js_api/build
    make $1 2>/tmp/$1.log
}

build_package()
{
    pkg_dir=/tmp/webrtc_plugin_pkg
    rm -rf $pkg_dir
    mkdir -p $pkg_dir

    # tests and docs
    cp -f ./tests/* $pkg_dir
    chmod +x $pkg_dir/go_*.sh

    # plugin so
    if [ $PLATFORM = 'Darwin' ]; then
        echo
    else
        cp -f $ROOT/src/js_api/build/bin/WebrtcEngine/npWebrtcEngine.so $pkg_dir
    fi

    # peerconnection_server
    if [ $PLATFORM = 'Darwin' ]; then
        test_dir=$ROOT/third_party/libjingle/trunk/xcodebuild/$bld_type
    else
        test_dir=$ROOT/third_party/libjingle/trunk/out/$bld_type
    fi
    if [ -f $test_dir/peerconnection_server ]; then
        cp -f $test_dir/peerconnection_server $pkg_dir
    fi

    obj_dir=`dirname $pkg_dir`
    obj_name=`basename $pkg_dir`
    cd $obj_dir
    tar cvfz $pkg_dir.tar.gz $obj_name
}

build_engine()
{
    build_plugin "WebrtcEngine"
}

build_all()
{
    build_prepare
    build_jingle 
    build_webrtc
    build_engine
    build_package
}

usage()
{
    echob "usage: $0 all|jingle|webrtc|engine|package"
}


if [ $# != 1 ]; then
    usage
    exit 1
fi

echog "[*] Building $1"
bld_type=Release
build_prepare
build_$1
check_error "fail to build_$1"

exit 0

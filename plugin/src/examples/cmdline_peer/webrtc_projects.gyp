# Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

{
  'includes': [
    '../../../third_party/libjingle/trunk/third_party/webrtc/build/common.gypi',
  ],
  'targets': [
    {
      'target_name': 'peerconnection_server',
      'type': 'executable',
      'sources': [
        '../../../third_party/libjingle/trunk/talk/examples/peerconnection/server/data_socket.cc',
        '../../../third_party/libjingle/trunk/talk/examples/peerconnection/server/data_socket.h',
        '../../../third_party/libjingle/trunk/talk/examples/peerconnection/server/main.cc',
        '../../../third_party/libjingle/trunk/talk/examples/peerconnection/server/peer_channel.cc',
        '../../../third_party/libjingle/trunk/talk/examples/peerconnection/server/peer_channel.h',
        '../../../third_party/libjingle/trunk/talk/examples/peerconnection/server/utils.cc',
        '../../../third_party/libjingle/trunk/talk/examples/peerconnection/server/utils.h',
      ],
      'dependencies': [
          '../../../third_party/libjingle/trunk/talk/libjingle.gyp:libjingle',
      ],
      'include_dirs': [
          '../../../third_party/libjingle/trunk',
      ],
      'link_settings': {
          'libraries': [
              '-lrt',
          ]
      }
    },
  ],
  'conditions': [
      ['OS=="linux"', {
          'targets': [
          {
            'target_name': 'cmdline_audio_peer',
            'type': 'executable',
            'sources': [
                'main.cpp',
                'parsecmd.cpp',
                'shell.cpp',
                'shell.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.cpp',
                'TestDefaults.h',
                'TestDefaults.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.h',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.cpp',
                'TestClientShell.h',
                '../../include/ThreadSingle.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLCall.h',
                '../../js_api/projects/WebrtcPlugin/WPLCall.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.h',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.cpp',
                '../../include/rtc_common.h',
                '../../include/anyoption/anyoption.h',
                '../../include/anyoption/anyoption.cpp',
            ],
           'dependencies': [
                '../../../third_party/libjingle/trunk/talk/libjingle.gyp:libjingle_peerconnection',
            ],
            'defines': [
                'GOCAST_LINUX',
                'POSIX',
                'JSONCPP_RELATIVE_PATH',
                'LIBPEERCONNECTION_LIB',
            ],
            'include_dirs': [
                '../../../third_party/libjingle/trunk/third_party/jsoncpp/source/include',
                '../../../third_party/libjingle/trunk',
                '../../include',
                '../../js_api/projects/WebrtcPlugin'
            ],
            'link_settings': {
                'libraries': [
                    '-lX11',
                    '-lXext',
                    '-lXrender',
                    '-lXcomposite',
                ],
            },
          },        
         {
            'target_name': 'cmdline_video_peer',
            'type': 'executable',
            'sources': [
                'main.cpp',
                'parsecmd.cpp',
                'shell.cpp',
                'shell.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.cpp',
                'TestDefaults.h',
                'TestDefaults.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.h',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.cpp',
                'TestClientShell.h',
                '../../include/ThreadSingle.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLCall.h',
                '../../js_api/projects/WebrtcPlugin/WPLCall.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.h',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.cpp',
                '../../include/rtc_common.h',
                '../../include/anyoption/anyoption.h',
                '../../include/anyoption/anyoption.cpp',
                '../../js_api/projects/WebrtcPlugin/X11/WPLVideoRenderer.h',
                '../../js_api/projects/WebrtcPlugin/X11/WPLVideoRenderer.cpp',
            ],
           'dependencies': [
                '../../../third_party/libjingle/trunk/talk/libjingle.gyp:libjingle_peerconnection',
            ],
            'defines': [
                'GOCAST_ENABLE_VIDEO',
                'GOCAST_LINUX',
                'POSIX',
                'JSONCPP_RELATIVE_PATH',
                'LIBPEERCONNECTION_LIB',
            ],
            'include_dirs': [
                '../../../third_party/libjingle/trunk/third_party/jsoncpp/source/include',
                '../../../third_party/libjingle/trunk',
                '../../include',
                '../../js_api/projects/WebrtcPlugin'
            ],
            'cflags': [
                '<!@(pkg-config --cflags gtk+-2.0)',
            ],
            'link_settings': {
                'ldflags': [
                    '<!@(pkg-config --libs-only-L --libs-only-other gtk+-2.0 gthread-2.0)',
                ],
                'libraries': [
                    '-lX11',
                    '-lXext',
                    '-lXrender',
                    '-lXcomposite',
                    '<!@(pkg-config --libs-only-l gtk+-2.0 gthread-2.0)',
                ],
            },
         },
          {
	# Now for the version which doesn't use gyp's monster deps, but instead links the monolithic libwebrtc.a
            'target_name': 'cmdline_audio_peer_monolithic',
            'type': 'executable',
            'sources': [
                'main.cpp',
                'parsecmd.cpp',
                'shell.cpp',
                'shell.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.cpp',
                'TestDefaults.h',
                'TestDefaults.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.h',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.cpp',
                'TestClientShell.h',
                '../../include/ThreadSingle.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLCall.h',
                '../../js_api/projects/WebrtcPlugin/WPLCall.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.h',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.cpp',
                '../../include/rtc_common.h',
                '../../include/anyoption/anyoption.h',
                '../../include/anyoption/anyoption.cpp',
            ],
			'defines': [
				'POSIX',
				'WEBRTC_LINUX',
				'GOCAST_LINUX',
			],
			'include_dirs': [
                '../../../third_party/libjingle/trunk',
				'../../include',
				'../../js_api/projects/WebrtcPlugin',
            ],
			'link_settings': {
				'libraries': [
# TODO - use _Release here if release build.
					'../../../third_party/libjingle/trunk/out/Release/libwebrtc_Release.a',
					'-lexpat',
					'-lX11',
					'-lXext',
					'-lXrender',
					'-lXcomposite',
					'-ldl',
					'-lrt',
					'-lpthread',
					'-lGL',
				],
			},
          }, 
	], #targets
      }, ], #OS=="linux"
      ['OS=="mac"', {
        'targets': [
        {
            'target_name': 'cmdline_audio_peer',
            'type': 'executable',
            'sources': [
                'main.cpp',
				'parsecmd.cpp',
				'shell.cpp',
				'shell.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.cpp',
                'TestDefaults.h',
                'TestDefaults.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.h',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.cpp',
                'TestClientShell.h',
                '../../include/ThreadSingle.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLCall.h',
                '../../js_api/projects/WebrtcPlugin/WPLCall.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.h',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.cpp',
                '../../include/rtc_common.h',
                '../../include/anyoption/anyoption.h',
				'../../include/anyoption/anyoption.cpp',
            ],
            'dependencies': [
                '../../../third_party/libjingle/trunk/talk/libjingle.gyp:libjingle_peerconnection',
            ],
			'defines': [
				'POSIX',
				'WEBRTC_MAC',
				'GOCAST_MAC',
			],
            'include_dirs': [
                '../../../third_party/libjingle/trunk',
				'../../include',
				'../../js_api/projects/WebrtcPlugin'
            ],
            'link_settings': {
                'xcode_settings': {
                  'OTHER_LDFLAGS': [
                    '-L../../../third_party/libjingle/trunk/third_party/libsrtp',
                    '-lsrtp',
                    '-framework CoreServices',
                    '-framework CoreAudio',
                    '-framework CoreVideo',
                    '-framework QtKit',
                    '-framework OpenGL',
                    '-framework AudioToolbox',
                    '-framework ApplicationServices',
                    '-framework Foundation',
                    '-framework AppKit',
                  ],
                },
            },
        }, 
# Now for the monolithic version - links to the libwebrtc_Debug.a big library.
        {
            'target_name': 'cmdline_audio_peer_monolithic',
            'type': 'executable',
            'sources': [
                'main.cpp',
				'parsecmd.cpp',
				'shell.cpp',
				'shell.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionClient.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.h',
                '../../js_api/projects/WebrtcPlugin/WPLPeerConnectionObserver.cpp',
                'TestDefaults.h',
                'TestDefaults.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.h',
                '../../js_api/projects/WebrtcPlugin/WPLSocketServer.cpp',
                'TestClientShell.h',
                '../../include/ThreadSingle.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.h',
                '../../js_api/projects/WebrtcPlugin/WPLThreadSafeMessageQueue.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLCall.h',
                '../../js_api/projects/WebrtcPlugin/WPLCall.cpp',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.h',
                '../../js_api/projects/WebrtcPlugin/WPLConfig.cpp',
                '../../include/rtc_common.h',
                '../../include/anyoption/anyoption.h',
				'../../include/anyoption/anyoption.cpp',
            ],
			'defines': [
				'POSIX',
				'WEBRTC_MAC',
				'GOCAST_MAC',
			],
            'include_dirs': [
                '../../../third_party/libjingle/trunk',
				'../../include',
				'../../js_api/projects/WebrtcPlugin'
            ],
            'link_settings': {
                'xcode_settings': {
                  'OTHER_LDFLAGS': [
                    '-L../../../third_party/libjingle/trunk/third_party/libsrtp',
					'-L../../../third_party/libjingle/trunk/xcodebuild/Debug',
# TODO - use _Release here if release build.
					'-lwebrtc_Debug',
					'-ldl',
					'-lpthread',
                    '-lsrtp',
                    '-framework CoreServices',
                    '-framework CoreAudio',
                    '-framework CoreVideo',
                    '-framework QtKit',
                    '-framework OpenGL',
                    '-framework AudioToolbox',
                    '-framework ApplicationServices',
                    '-framework Foundation',
                    '-framework AppKit',
                  ],
                },
            },
        }, 
	], #targets
      }, ] #OS=="mac"
  ], #conditions
}

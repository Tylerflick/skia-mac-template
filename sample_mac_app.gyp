{
  'targets': [
    {
      'configurations': {
        'Debug': { },
        'Release': { }
      },
      'target_name': 'sample_mac_app',
      'product_name': 'SampleMacApp',
      'type': 'executable',
      'mac_bundle': 1,
      'dependencies': [
        'third_party/skia/gyp/skia_lib.gyp:skia_lib',
        'third_party/skia/gyp/views.gyp:views',
        'third_party/skia/gyp/xml.gyp:xml',
        'third_party/skia/gyp/codec.gyp:codec',
        'third_party/skia/gyp/libpng.gyp:libpng_static'
      ],
      'include_dirs': [
        'third_party/skia/include/config',
        'third_party/skia/include/core',
        'third_party/skia/include/views',
        'third_party/skia/include/private',
        'third_party/skia/src/views/mac',
        'third_party/skia/src/core',
        'third_party/skia/src/codec',
        'SampleMacApp',
      ],
      'sources': [
        'SampleMacApp/AppDelegate.mm',
        'SampleMacApp/AppDelegate.h',
        'SampleMacApp/main.mm',
        'SampleMacApp/SampleSkNSView.h',
        'SampleMacApp/SampleSkNSView.mm',
        'SampleMacApp/SampleWindow.h',
        'SampleMacApp/SampleWindow.cpp',
        'SampleMacApp/Test.swift',
        'SampleMacApp/<(_product_name)-Bridging-Header.h'
      ],
      'xcode_settings': {
        'INFOPLIST_FILE': 'SampleMacApp/Info.plist',
        'MACOSX_DEPLOYMENT_TARGET': '10.11',
        'CLANG_ENABLE_MODULES': 'YES',
        'CLANG_ENABLE_OBJC_ARC': 'YES',
        'SWIFT_OBJC_BRIDGING_HEADER': 'SampleMacApp/<(_product_name)-Bridging-Header.h',
        'LD_RUNPATH_SEARCH_PATHS': ['@executable_path/../Frameworks', '@loader_path/../Frameworks'],
      },
      'mac_bundle_resources': [
        'SampleMacApp/Base.lproj/MainMenu.xib',
        'SampleMacApp/Assets.xcassets',
      ],
      'link_settings': {
        'libraries': [
          '/System/Library/Frameworks/Cocoa.framework',
        ],
      },
      'ldflags': [
        '-lskia', '-stdlib=libc++', '-std=c++1y'
      ],
      'cflags': [
        '-Werror', '-W', '-Wall', '-Wextra', '-Wno-unused-parameter', '-g', '-O0', '-stdlib=libc++'
      ]
    }
  ]
}

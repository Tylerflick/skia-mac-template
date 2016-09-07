# skia-mac-template
A template project for getting started with the [Skia graphics library](https://skia.org) on macOS/OS X

**Warning**

This repo is meant to be consumed with gclient, not cloned directly with git.

*For a deeper understanding of the build process checkout [Skia's official sample guide](https://skia.org/user/sample/building).*

## Dependencies
- [git] (http://git-scm.com/)
- [gclient] (https://code.google.com/p/gclient/)
- [gyp] (https://code.google.com/p/gyp/)
- [ninja] (http://martine.github.io/ninja/)

## Getting Started
1. Create a directory for your project
2. Within the directory create a .gclient file with the following contents:
		
		solutions = [
		  { "name"        : "src",
		    "url"         : "https://github.com/Tylerflick/skia-mac-template.git",
		    "deps_file"   : "DEPS",
		    "managed"     : True,
		    "custom_deps" : {
		    },
		    "safesync_url": "",
		  },
		  { "name"        : "src/third_party/skia",
		    "url"         : "https://skia.googlesource.com/skia.git@deb8136c9ce4c65b707a1625a6b7d94d3cad9f4f",
		    "deps_file"   : "DEPS",
		    "managed"     : True,
		    "custom_deps" : {
		    },
		    "safesync_url": "",
		  },
		]
		cache_dir = None


 *Note: This repo references a particular commit from the Skia repo. This commit hash can be changed, but in doing so the commit hash references in `DEPS` must be changed to match.*
 
3. Run `gclient sync`
4. Launch the Xcode project, `src/your_app.xcodeproj`
5. Build and run
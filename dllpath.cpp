#include "pch.h"
#include "dllpath.h"
#include <libloaderapi.h>
#include <string>

std::string path;

namespace plugInSettings {
		void findDllPath() {
			char DllPathFile[_MAX_PATH];
			GetModuleFileNameA(HINSTANCE(&__ImageBase), DllPathFile, sizeof(DllPathFile));
			path = DllPathFile;
			path.resize(path.size() - strlen("StripMaker.dll"));
		}
		std::string getDllPath() {
			return path;
		}
}
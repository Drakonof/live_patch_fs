#pragma once

#define FUSE_USE_VERSION 31

#include <fuse3/fuse.h>
#include <string>
#include <map>

class LivePatchFs {
public:
	LivePatchFs();
	
	int getattr(const char* path, struct stat* stbuf);
	int readdir(const char* path, void* buf, fuse_fill_dir_t filler);
	int read(const char* path, char* buf, size_t size, off_t offset);
private:
	std::map<std::string, std::string> patches_;
	
	void scanPatches();
	std::string readStatus();
};

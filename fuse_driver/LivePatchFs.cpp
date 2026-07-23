#include "LivePatchFs.hpp"
#include <cstring>
#include <fstream>
#include <filesystem>

LivePatchFs::LivePatchFs()
{
	scanPatches();
}

int LivePatchFs::getattr(const char* path, struct stat* stbuf)
{
	std::string p(path);
	std::memset(stbuf, 0, sizeof(*stbuf));

	if (p == "/") {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
	}

	if (p == "/status") {
		std::string content = readStatus();
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = content.size();
		return 0;
	}

	std::string name = p.substr(1);
	auto it = patches_.find(name);
	if (it != patches_.end()) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = it->second.size();
		return 0;
	}

	return -ENOENT;
}
	
int LivePatchFs::readdir(const char* path, void* buf, fuse_fill_dir_t filler)
{
	std::string p(path);
	
	if (p != "/")
		return -ENOENT;

	filler(buf, ".", nullptr, 0, static_cast<fuse_fill_dir_flags>(0));
	filler(buf, "..", nullptr, 0, static_cast<fuse_fill_dir_flags>(0));
	filler(buf, "status", nullptr, 0, static_cast<fuse_fill_dir_flags>(0));

	for (const auto& [name, state] : patches_)
		filler(buf, name.c_str(), nullptr, 0, static_cast<fuse_fill_dir_flags>(0));

	return 0;
}

int LivePatchFs::read(const char* path, char* buf, size_t size, off_t offset) 
{
	std::string p(path);
	std::string content;
	
	if (p == "/status") {
		content = readStatus();
	} else  {
		std::string name = p.substr(1);
		auto it = patches_.find(name);
		if (it != patches_.end())
			content = it->second;
		else
			return -ENOENT;
	}

	if (offset >= static_cast<off_t>(content.size()))
		return 0;
	size_t len = content.size() - offset;
	if (size < len)
		len = size;

	std::memcpy(buf, content.data() + offset, len);
	return static_cast<int>(len);
}

void LivePatchFs::scanPatches()
{
	const std::string base = "/sys/kernel/livepatch";

	if (!std::filesystem::exists(base))
		return;

	for (const auto& entry : std::filesystem::directory_iterator(base)) {
		if (entry.is_directory()) {
			std::string name = entry.path().filename().string();

			std::ifstream enabled_file(entry.path().string() + "/enabled");
			std::string enabled_str;
			if (enabled_file.is_open())
				std::getline(enabled_file, enabled_str);

			patches_[name] = (enabled_str == "1" ? "enabled\n" : "disabled\n");
		}
	}
}

std::string LivePatchFs::readStatus()
{
	std::string result;
	if (result.empty())
		return "No patches applied\n";

	for (const auto& [name, state] : patches_)
		result += name + ": " + state;

	return result;
}

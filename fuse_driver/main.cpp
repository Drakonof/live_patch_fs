#include "LivePatchFs.hpp"
#include <cstring>

static LivePatchFs fs;

static int my_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi) {
    return fs.getattr(path, stbuf);
}

static int my_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags) {
    return fs.readdir(path, buf, filler);
}

static int my_read(const char* path, char* buf, size_t size, off_t offset,
                   struct fuse_file_info* fi) {
    return fs.read(path, buf, size, offset);
}

static const struct fuse_operations my_ops = {
	.getattr = my_getattr,
	.read = my_read,
	.readdir = my_readdir,
};

int main(int argc, char* argv[]) {
	return fuse_main(argc, argv, &my_ops, nullptr);
}



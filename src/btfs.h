/*
Copyright 2015 Johan Gunnarsson <johan.gunnarsson@gmail.com>

This file is part of BTFS.

BTFS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

BTFS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BTFS.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BTFS_H
#define BTFS_H

#include <vector>
#include <list>
#include <fstream>

#include "libtorrent/config.hpp"
#include <libtorrent/peer_request.hpp>

#include "btfsstat.h"

#if LIBTORRENT_VERSION_NUM > 12000
typedef libtorrent::piece_index_t piece_index_t;
typedef libtorrent::file_index_t file_index_t;
#else
typedef int piece_index_t;
typedef int file_index_t;
#endif

namespace btfs
{

class Part;
class Read;

typedef std::vector<Part>::iterator parts_iter;
typedef std::list<Read*>::iterator reads_iter;

class Part
{
	friend class Read;

public:
	Part(libtorrent::peer_request p, char *b) : part(p), buf(b),
			filled(false) {
	}

private:
	libtorrent::peer_request part;

	char *buf;

	bool filled;
};

class Read
{
public:
	Read(char *buf, file_index_t index, off_t offset, size_t size);

	void fail(piece_index_t piece);

	void copy(piece_index_t piece, char *buffer, int size);

	void trigger();

	bool finished();

	int size();

	int read();

private:
	bool failed = false;

	std::vector<Part> parts;
};

class Array
{
public:
	Array() : buf(0), size(0) {
	}

	~Array() {
		free(buf);
	}

	bool expand(size_t n) {
		return (buf = (char *) realloc((void *) buf, size += n)) != NULL;
	}

	char *buf;

	size_t size;
};

class Log : public std::ofstream
{
public:
	Log(std::string p) : std::ofstream(p.c_str()), path(p) {
		if (!is_open())
			// If open log file fails, write to a dummy file
			open("/dev/null");
	}

	~Log() {
		close();

		if (remove(path.c_str()))
			perror("Failed to remove log");
	}

private:
	std::string path;
};

struct btfs_params {
	int version;
	int help;
	int help_fuse;
	int browse_only;
	int keep;
	int utp_only;
	int no_fetch;
	char *data_directory;
	int min_port;
	int max_port;
	int max_download_rate;
	int max_upload_rate;
	const char *metadata;
};

}

#endif

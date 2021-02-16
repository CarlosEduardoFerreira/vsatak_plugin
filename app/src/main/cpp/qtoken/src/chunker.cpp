#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "chunker.hpp"

using namespace Qtoken;

/**
 * Creates a chunker object and fills it up.
 * @param fs the stream of file to be chunked
 * @param chunk_size size of chunk in bytes
 */
Chunker::Chunker(std::ifstream& fs, int chunk_size) {
    fs.seekg(0, fs.end);
    std::streampos stream_size = fs.tellg();
    this->file_size = static_cast<int>(stream_size);
    if (chunk_size <= 0) {
        std::cerr << "Chunk size " << chunk_size << " is invalid" << std::endl;
        return;
    }
    int total_chunks = 1 + ((file_size - 1) / chunk_size);
    int tail_size = file_size - ((total_chunks - 1) * chunk_size);

    for (int i = 0; i < total_chunks; i++) {
        char* memblock;
        fs.seekg(chunk_size * i);

        // automatically pad last chunk
        if (i == total_chunks - 1) {
            memblock = new char[chunk_size]();
            fs.read(memblock, tail_size);
        } else {
            memblock = new char[chunk_size];
            fs.read(memblock, chunk_size);
        }

        std::vector<char> memvec(memblock, memblock + chunk_size);
        chunks.push_back(memvec);
        delete[] memblock;
    }
}

/**
 * Write chunks to files.
 * @param dir directory to write chunks too
 */
void Chunker::write_chunks(const std::string& dir) {
    std::filesystem::create_directory(dir);
    if (chunks.size() == 0)
        return;

    for (int i = 0; i < chunks.size(); i++) {
        // assume '/' at the end of dir
        std::string shard_file_name = dir + "shard_" + std::to_string(i);
        std::ofstream shard_file(shard_file_name);

        if (shard_file.is_open()) {
            if (i == chunks.size() - 1)
                shard_file.write(chunks[i].data(),
                                 file_size % get_chunk_size());
            else
                shard_file.write(chunks[i].data(), get_chunk_size());
            shard_file.close();
        } else {
            std::cerr << "Unable to open file" << std::endl;
            return;
        }
    }
    std::cout << "Shard files written to " << dir << std::endl;
}

/**
 * Given a chunk size resize each chunk to match the new size
 * smaller chunker sizes will truncate to achieve the new size
 * larger chunk sizes will pad with zeroes
 * @param new_size new chunk size
 */
void Chunker::resize_chunks(int new_size) {
    for (auto& chunk : chunks) {
        chunk.resize(new_size, 0);
    }
}

/**
 * Rebuilds file from chunks and saves to specified directory
 * Note: tail chunk padding is truncated on rebuild
 * @param output_file save directory
 */
void Chunker::rebuild(std::string output_file) {
    int chunk_size = chunks[0].size();
    int total_chunks = 1 + ((file_size - 1) / chunk_size);
    int tail_size = file_size - ((total_chunks - 1) * chunk_size);

    std::ofstream fs(output_file, std::ios::app | std::ios::binary);
    for (int i = 0; i < chunks.size(); i++) {
        // Truncate tail padding
        if (i == chunks.size() - 1) {
            fs.write(chunks[i].data(), tail_size);
        } else {
            fs.write(chunks[i].data(), chunks[i].size());
        }
    }
    fs.close();
    std::cout << "File rebuilt at " << output_file << std::endl;
}

/**
 * Chunk wise comparison of two chunkers
 * @param rhs chunker to be compared to this
 * Note: Because it is chunk wise comparison equal files
 * with differing chunk sizes can return != as chunkers
 */
bool Chunker::operator==(Chunker& rhs) {
    if (file_size != rhs.get_file_size()) {
        return false;
    }
    if (get_chunk_size() != rhs.get_chunk_size()) {
        return false;
    }
    if (this->chunks != rhs.get_chunks())
        return false;

    return true;
}

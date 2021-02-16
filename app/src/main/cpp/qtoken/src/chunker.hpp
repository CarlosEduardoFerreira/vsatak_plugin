#ifndef CHUNKER_H
#define CHUNKER_H

#include <fstream>
#include <string>
#include <vector>

namespace Qtoken {

/**
 * Chunker handles file chunking operations. Its purpose is to store a byte
 * vector which can undergo several arbitrary transforms.
 *
 * Chunks are byte vectors that can be written to files. They support
 * encoding operations.
 */
class Chunker {
private:
    std::vector<std::vector<char>> chunks;
    int file_size;

public:
    Chunker() = default;
    Chunker(std::ifstream& file_stream, int chunk_size);
    Chunker(std::vector<std::vector<char>> chunks, int file_size)
        : chunks(chunks)
        , file_size(file_size){};
    ~Chunker(){};

    inline int size() { return chunks.size(); };
    inline std::vector<char>& get(int i) { return chunks[i]; };
    inline int get_chunk_size() { return chunks[0].size(); };
    int get_file_size() { return file_size; };
    void rebuild(std::string output_file);
    std::vector<std::vector<char>>& get_chunks() { return chunks; }
    void write_chunks(const std::string& dir);
    void resize_chunks(int new_size);
    bool operator==(Chunker& rhs);
};

}  // namespace Qtoken

#endif

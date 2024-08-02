#ifndef ODD_CODING_LIB_HPP
#define ODD_CODING_LIB_HPP

#include <istream>
#include <ostream>

namespace oddcod
{

    using word_t = unsigned char;

    enum class Result
    {
        OK = 0,
        DECODE_ERR = 1,
        ENCODE_ERR = 2
    };

    // template <typename T>
    // class Input
    //{
    // public:
    // explicit Input(T &source) : m_source(source)
    //{
    // init();
    //}

    // virtual word_t read() = 0;
    // virtual bool canRead() = 0;

    // private:
    // virtual void init() = 0;
    // T &m_source;
    //};

    // template <>
    // class Input<const std::vector<word_t> &>
    //{
    // public:
    // word_t read() {

    //};
    // bool canRead() {

    //};

    // private:
    // void init()
    //{
    // }
    //};

    namespace huffman
    {
        // template <typename T>
        // Result decode(Input<T> input, Output<T> output);
        // template <typename T>
        // Result encode(Input<T> input, Output<T> output);
        ////
        // Result decode(const std::vector<word_t> &inBuffer, std::vector<word_t> &outBuffer);
        // Result encode(const std::vector<word_t> &inBuffer, std::vector<word_t> &outBuffer);
        ////
        Result decode(std::istream &inStream, std::ostream &outStream);
        Result encode(std::istream &inStream, std::ostream &outStream);
        ////
        // Result decode(std::istream &inStream, std::vector<word_t> &outBuffer);
        // Result encode(std::istream &inStream, std::vector<word_t> &outBuffer);
        ////
        // Result decode(const std::vector<word_t> &inBuffer, std::ostream &outStream);
        // Result encode(const std::vector<word_t> &inBuffer, std::ostream &outStream);
    };
};

#endif

#include "pipeline.hpp"

using namespace Qtoken;

/**
 * Calls the base chunk operator on writer w
 * @param w writer to call base on
 */
template <typename Coder>
void Pipeline<Coder>::run(Writer& w) {
    this->base(w);
}

/**
 * Adds a single coder to the pipeline
 * by composing the given coder's chunk operator
 * with the base pipeline chunk operator
 * @param c coder to be added
 */
template <typename Coder>
void Pipeline<Coder>::add_coder(Coder& c) {
    this->base = this->compose(this->base, c.get_coder());
}

/**
 * Variadic function with count args
 * Args are expected to be of type Coder&
 * Each given coder is added to the pipeline
 * @param count The number of arguments passed
 * @param ... Coder to add to the pipeline
 */
template <typename Coder>
void Pipeline<Coder>::add(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        add_coder(*va_arg(args, Coder*));
    }

    va_end(args);
}

/**
 * Composer method. Allows to compose 2 functions whose return values
 * feed into each other, providing additional support for a Writer,
 * which is used to store metadata.
 * Taken from:
 * https://bartoszmilewski.com/2020/08/05/categories-for-the-working-c-programmer/
 * @param f f in resulting composition g(f(x))
 * @param g g in resulting composition g(f(x))
 * @return chunk operator g(f(x))
 */
template <typename Coder>
ChunkOperator Pipeline<Coder>::compose(ChunkOperator f, ChunkOperator g) {
    return [f, g, this](Writer& w) {
        f(w);
        g(w);
    };
}

/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <fmi4cpp/fmi2/xml/ModelStructure.hpp>

using fmi4cpp::fmi2::Unknown;
using fmi4cpp::fmi2::ModelStructure;

Unknown::Unknown(const unsigned int index, const std::optional<std::string> &dependenciesKind,
        const std::optional<std::vector<unsigned int>> &dependencies)
        : index_(index), dependenciesKind_(dependenciesKind), dependencies_(dependencies) {}

const size_t fmi4cpp::fmi2::Unknown::index() {
    return index_;
}

const std::optional<std::string> fmi4cpp::fmi2::Unknown::dependenciesKind() {
    return dependenciesKind_;
}

const std::optional<std::vector<unsigned int>> &Unknown::dependencies() const {
    return dependencies_;
}

ModelStructure::ModelStructure(const std::vector<Unknown> &outputs_, const std::vector<Unknown> &derivatives_,
                               const std::vector<Unknown> &initialUnknowns_)
        : outputs_(outputs_), derivatives_(derivatives_), initialUnknowns_(initialUnknowns_) {}

const std::vector<Unknown> &ModelStructure::outputs() const {
    return outputs_;
}

const std::vector<Unknown> &ModelStructure::derivatives() const {
    return derivatives_;
}

const std::vector<Unknown> &ModelStructure::initialUnknowns() const {
    return initialUnknowns_;
}

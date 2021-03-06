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

#include <fstream>
#include <vector>
#include <iostream>

#include <fmi4cpp/driver/FmuDriver.hpp>

using namespace std;
using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

using namespace fmi4cpp::driver;

namespace {

    const char *CSV_SEPARATOR = ",";

    void addHeader(vector<ScalarVariable> &variables, std::string &data) {

        data += "\"Time\", ";

        for (unsigned long i = 0; i < variables.size(); i++) {
            data += "\"" + variables[i].name() + "\"";
            if (i != variables.size() - 1) {
                data += CSV_SEPARATOR;
            }
        }

    }

    void addRow(fmi4cpp::fmi2::FmuSlave &slave, vector<ScalarVariable> &variables, string &data) {

        data += "\n" + to_string(slave.getSimulationTime()) + CSV_SEPARATOR;
        for (unsigned int i = 0; i < variables.size(); i++) {
            auto var = variables[i];

            if (var.isInteger()) {
                int ref = 0;
                slave.readInteger(var.valueReference(), ref);
                data += to_string(ref);
            } else if (var.isReal()) {
                double ref = 0;
                slave.readReal(var.valueReference(), ref);
                data += to_string(ref);
            } else if (var.isString()) {
                const char *ref;
                slave.readString(var.valueReference(), ref);
                data += ref;
            } else if (var.isBoolean()) {
                int ref = 0;
                slave.readBoolean(var.valueReference(), ref);
                data += to_string(ref);
            }

            if (i != variables.size() - 1) {
                data += CSV_SEPARATOR;
            }
        }

    }

}

void FmuDriver::run(DriverOptions options) {

    if (options.modelExchange) {
#if FMI4CPP_WITH_ODEINT
        auto solver = make_solver<EulerSolver>(1E-3);
        simulate(fmu_->asModelExchangeFmu()->newInstance(solver), options);
#else
        const char *msg = "Model Exchange mode selected, but driver has been built without odeint support!";
        cerr << msg << endl;
        throw Failure(msg);
#endif
    } else {
        simulate(fmu_->asCoSimulationFmu()->newInstance(), options);
    }

}

void FmuDriver::dumpOutput(const string &data, const string &outputFolder) {

    const auto fmuName = fs::path(fmu_->fmuFile_).stem().string();
    const auto outputFile = fs::path(outputFolder + "/" + fmuName + "_out.csv");
    fs::create_directories(outputFile.parent_path());

    ofstream out(outputFile.string(), ofstream::out);
    out << data;
    out.flush();
    out.close();

}

void FmuDriver::simulate(std::unique_ptr<FmuSlave> slave, DriverOptions options) {

    auto startTime = options.startTime;
    auto stopTime = options.stopTime;
    auto stepSize = options.stepSize;

    slave->setupExperiment(startTime);
    slave->enterInitializationMode();
    slave->exitInitializationMode();

    string data = "";
    addHeader(options.variables, data);

    addRow(*slave, options.variables, data);
    while (slave->getSimulationTime() <= stopTime) {

        if (!slave->doStep(stepSize)) {
            slave->terminate();
            const string msg = string("Simulation terminated prematurely.");
            throw Failure(msg);
        }

        addRow(*slave, options.variables, data);

    }

    slave->terminate();

    if (options.failOnLargeFileSize) {
        const size_t size = data.size();
        if (size > 1e6) {
            double mbSize = ((double) size) / 1e6;
            throw Rejection(string("Generated csv was larger than 1MB. Was: ") + std::to_string((mbSize)) + "MB");
        }
    }

    dumpOutput(data, options.outputFolder.string());

}

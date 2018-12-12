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

#ifndef FMI4CPP_FMI2FMU_HPP
#define FMI4CPP_FMI2FMU_HPP

#include <memory>
#include <vector>
#include <string>
#include <type_traits>

#include "fmi4cpp/Fmu.hpp"
#include "fmi4cpp/solver/ModelExchangeSolver.hpp"

#include "fmi2Slave.hpp"
#include "fmi2CoSimulationLibrary.hpp"
#include "fmi2ModelExchangeLibrary.hpp"
#include "fmi2ModelExchangeInstance.hpp"
#include "fmi2ModelExchangeSlave.hpp"

#include "fmi4cpp/fmi2/xml/ModelDescription.hpp"

namespace fmi4cpp::fmi2 {

class fmi2Fmu : public virtual FmuProvider<ModelDescription> {

        friend class fmi2CoSimulationFmu;

        friend class fmi2ModelExchangeFmu;

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<ModelDescription> modelDescription_;

    public:
        explicit fmi2Fmu(const std::string &fmuFile);

        const std::string fmuFile_;
        
        const std::string getFmuFileName() const;

        const std::string getModelDescriptionXml() const;

        std::shared_ptr<ModelDescription> getModelDescription() const override;

        bool supportsModelExchange() const override;

        bool supportsCoSimulation() const override;

        std::unique_ptr<CoSimulationFmu> asCoSimulationFmu() const override;

        std::unique_ptr<ModelExchangeFmu> asModelExchangeFmu() const override;

    };

    class fmi2CoSimulationFmu : public FmuBase<CoSimulationModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<fmi2CoSimulationLibrary> lib_;
        std::shared_ptr<CoSimulationModelDescription> modelDescription_;

    public:

        fmi2CoSimulationFmu(const std::shared_ptr<FmuResource> &resource,
                        const std::shared_ptr<CoSimulationModelDescription> &md);

        std::shared_ptr<CoSimulationModelDescription> getModelDescription() const override;

        std::unique_ptr<fmi2Slave> newInstance(bool visible = false, bool loggingOn = false);

    };

    class fmi2ModelExchangeFmu : public virtual FmuBase<ModelExchangeModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<fmi2ModelExchangeLibrary> lib_;
        std::shared_ptr<ModelExchangeModelDescription> modelDescription_;

    public:

        fmi2ModelExchangeFmu(const std::shared_ptr<FmuResource> &resource,
                         const std::shared_ptr<ModelExchangeModelDescription> &md);

        std::shared_ptr<ModelExchangeModelDescription> getModelDescription() const override;

        std::unique_ptr<fmi2ModelExchangeInstance> newInstance(bool visible = false, bool loggingOn = false);

        std::unique_ptr<fmi2ModelExchangeSlave>
        newInstance(std::unique_ptr<fmi4cpp::solver::ModelExchangeSolver> &solver, bool visible = false, bool loggingOn = false);

    };

}


#endif //FMI4CPP_FMI2FMU_HPP
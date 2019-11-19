/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 *
 * @section LICENSE
 * Copyright (c) 20(0,4), SeisSol Group
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 **/

#ifndef MODEL_ANISOTROPIC_DATASTRUCTURES_H_
#define MODEL_ANISOTROPIC_DATASTRUCTURES_H_

#include <Model/common_datastructures.hpp>
#include <Equations/elastic/Model/datastructures.hpp>
#include <Eigen/Eigen>
#include <Eigen/Eigenvalues>
#include <generated_code/init.h>
#include <generated_code/tensor.h>
#include <generated_code/kernel.h>

namespace seissol {
  namespace model {
    struct AnisotropicMaterial : Material {
      real c_store[21];
      real c11;
      real c12;
      real c13;
      real c14;
      real c15;
      real c16;
      real c22;
      real c23;
      real c24;
      real c25;
      real c26;
      real c33;
      real c34;
      real c35;
      real c36;
      real c44;
      real c45;
      real c46;
      real c55;
      real c56;
      real c66;

      AnisotropicMaterial() {};

      AnisotropicMaterial(ElasticMaterial m) {
        rho = m.rho;
        c11 = m.lambda + 2*m.mu;
        c12 = m.lambda;
        c13 = m.lambda;
        c14 = 0;
        c15 = 0;
        c16 = 0; 
        c22 = m.lambda + 2*m.mu;
        c23 = m.lambda;
        c24 = 0;
        c25 = 0;
        c26 = 0;
        c33 = m.lambda + 2*m.mu;
        c34 = 0;
        c35 = 0;
        c36 = 0;
        c44 = m.mu; 
        c45 = 0;
        c46 = 0;
        c55 = m.mu; 
        c56 = 0; 
        c66 = m.mu; 
      }

      virtual ~AnisotropicMaterial() {};

      Material getRotatedMaterialCoefficients(real i_N[36]) {
        AnisotropicMaterial material;
        material.rho = rho;
        using Matrix66 = Eigen::Matrix<real, 6, 6>;
        Matrix66 N = Matrix66(i_N);
        Matrix66 C = Matrix66();
        C(0,0) = c11;
        C(0,1) = c12;
        C(0,2) = c13;
        C(0,3) = c14;
        C(0,4) = c15;
        C(0,5) = c16;
        C(1,0) = c12;
        C(1,1) = c22;
        C(1,2) = c23;
        C(1,3) = c24;
        C(1,4) = c25;
        C(1,5) = c26;
        C(2,0) = c13;
        C(2,1) = c23;
        C(2,2) = c33;
        C(2,3) = c34;
        C(2,4) = c35;
        C(2,5) = c36;
        C(3,0) = c14;
        C(3,1) = c24;
        C(3,2) = c34;
        C(3,3) = c44;
        C(3,4) = c45;
        C(3,5) = c46;
        C(4,0) = c15;
        C(4,1) = c25;
        C(4,2) = c35;
        C(4,3) = c45;
        C(4,4) = c55;
        C(4,5) = c56;
        C(5,0) = c16;
        C(5,1) = c26;
        C(5,2) = c36;
        C(5,3) = c46;
        C(5,4) = c56;
        C(5,5) = c66;

        Matrix66 rotatedC = N.transpose()*C*N;

        material.c11 = rotatedC(0,0);
        material.c12 = rotatedC(0,1);
        material.c13 = rotatedC(0,2);
        material.c14 = rotatedC(0,3);
        material.c15 = rotatedC(0,4);
        material.c16 = rotatedC(0,5);
        material.c22 = rotatedC(1,1);
        material.c23 = rotatedC(1,2);
        material.c24 = rotatedC(1,3);
        material.c25 = rotatedC(1,4);
        material.c26 = rotatedC(1,5);
        material.c33 = rotatedC(2,2);
        material.c34 = rotatedC(2,3);
        material.c35 = rotatedC(2,4);
        material.c36 = rotatedC(2,5);
        material.c44 = rotatedC(3,3);
        material.c45 = rotatedC(3,4);
        material.c46 = rotatedC(3,5);
        material.c55 = rotatedC(4,4);
        material.c56 = rotatedC(4,5);
        material.c66 = rotatedC(5,5);
        return material;
      }

      void getFullElasticTensor(real fullTensor[81]) {
        fullTensor[0]  = c11;
        fullTensor[1]  = c16;
        fullTensor[2]  = c15;
        fullTensor[3]  = c16;
        fullTensor[4]  = c12;
        fullTensor[5]  = c14;
        fullTensor[6]  = c15;
        fullTensor[7]  = c14;
        fullTensor[8]  = c13;
        fullTensor[9]  = c16;
        fullTensor[10] = c66;
        fullTensor[11] = c56;
        fullTensor[12] = c66;
        fullTensor[13] = c26;
        fullTensor[14] = c46;
        fullTensor[15] = c56;
        fullTensor[16] = c46;
        fullTensor[17] = c36;
        fullTensor[18] = c15;
        fullTensor[19] = c56;
        fullTensor[20] = c55;
        fullTensor[21] = c56;
        fullTensor[22] = c25;
        fullTensor[23] = c35;
        fullTensor[24] = c55;
        fullTensor[25] = c45;
        fullTensor[26] = c35;
        fullTensor[27] = c16;
        fullTensor[28] = c66;
        fullTensor[29] = c56;
        fullTensor[30] = c66;
        fullTensor[31] = c26;
        fullTensor[32] = c46;
        fullTensor[33] = c56;
        fullTensor[34] = c46;
        fullTensor[35] = c36;
        fullTensor[36] = c12;
        fullTensor[37] = c26;
        fullTensor[38] = c25;
        fullTensor[39] = c26;
        fullTensor[40] = c22;
        fullTensor[41] = c24;
        fullTensor[42] = c25;
        fullTensor[43] = c24;
        fullTensor[44] = c23;
        fullTensor[45] = c14;
        fullTensor[46] = c46;
        fullTensor[47] = c45;
        fullTensor[48] = c46;
        fullTensor[49] = c24;
        fullTensor[50] = c44;
        fullTensor[51] = c45;
        fullTensor[52] = c44;
        fullTensor[53] = c34;
        fullTensor[54] = c15;
        fullTensor[55] = c56;
        fullTensor[56] = c55;
        fullTensor[57] = c56;
        fullTensor[58] = c25;
        fullTensor[59] = c45;
        fullTensor[60] = c55;
        fullTensor[61] = c45;
        fullTensor[62] = c35;
        fullTensor[63] = c14;
        fullTensor[64] = c46;
        fullTensor[65] = c45;
        fullTensor[66] = c46;
        fullTensor[67] = c24;
        fullTensor[68] = c44;
        fullTensor[69] = c45;
        fullTensor[70] = c44;
        fullTensor[71] = c34;
        fullTensor[72] = c14;
        fullTensor[73] = c36;
        fullTensor[74] = c35;
        fullTensor[75] = c36;
        fullTensor[76] = c23;
        fullTensor[77] = c34;
        fullTensor[78] = c35;
        fullTensor[79] = c34;
        fullTensor[80] = c33;
      }

      real getMaxWaveSpeed() {
#ifdef USE_ANISOTROPIC
        double samplingDirectionsData[seissol::tensor::samplingDirections::Size];
        std::copy_n(init::samplingDirections::Values,
            seissol::tensor::samplingDirections::Size,
            samplingDirectionsData);
        auto samplingDirections = init::samplingDirections::view::create(samplingDirectionsData);

        Eigen::SelfAdjointEigenSolver<Eigen::Matrix<real, 3, 3>> saes;

        real maxEv = 0;

        real fullTensor[81];
        getFullElasticTensor(fullTensor);
        kernel::computeChristoffel computeChristoffel;
        computeChristoffel.C = fullTensor;

        for(unsigned j = 0; j < 200; ++j)
        {
          real n[3] = { samplingDirections(j, 0),
                        samplingDirections(j, 1),
                        samplingDirections(j, 2)
          };
          real M[9];
          computeChristoffel.n = n;
          computeChristoffel.christoffel = M;
          computeChristoffel.execute();

          saes.compute(Eigen::Matrix<real, 3, 3>(M));
          auto eigenvalues = saes.eigenvalues();
          for(unsigned i = 0; i < 3; ++i) {
            maxEv = eigenvalues(i) > maxEv ? eigenvalues(i) : maxEv;
          }
        }
        return sqrt(maxEv / rho);
#else
        return 0;
#endif
      }

      real getPWaveSpeed() {
        real muBar = (c44 + c55 + c66) / 3.0;
        real lambdaBar = (c11 + c22 + c33) / 3.0 - 2.0*muBar;
        return std::sqrt((lambdaBar + 2*muBar) / rho);
      }

      real getSWaveSpeed() {
        real muBar = (c44 + c55 + c66) / 3.0;
        return std::sqrt(muBar / rho);
      }

      materialType getMaterialType() const {
        return anisotropic;
      }
    };
  }
}

#endif

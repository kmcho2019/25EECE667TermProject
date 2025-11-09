///////////////////////////////////////////////////////////////////////////////
// Creator: Minjae Kim of CSDL, POSTECH
// Email:   kmj0824@postech.ac.kr
// GitHub:  ApeachM
//
// BSD 3-Clause License
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////
#include "Evaluator.h"
namespace Placer {
bool Evaluator::placeLegalityCheck(Evaluator *compared_circuit) {
  compared_circuit_ = compared_circuit;

  // Variable numbers check
  vector<int> variable_numbers = compared_circuit_->getVariableNumbers();
  if (!cellNumCheck(variable_numbers.at(0))) {
    cout << "Cell number is different with inputted file." << endl;
    return false;
  } else if (!netNumCheck(variable_numbers.at(1))) {
    cout << "Net number is different with inputted file." << endl;
    return false;
  } else if (!pinNumCheck(variable_numbers.at(2))) {
    cout << "Pin number is different with inputted file." << endl;
    return false;
  } else if (!padNumCheck(variable_numbers.at(3))) {
    cout << "Pad number is different with inputted file." << endl;
    return false;
  }

  // Placed check
  if (!placeCheck()) {
    cout << "Some cell is not placed." << endl;
    return false;
  }
  if (!isAllCellInDie()) {
    cout << "Some cell is out of the die." << endl;
    return false;
  }
  return true;
}
bool Evaluator::evaluate(Evaluator *compared_circuit) {
  if (!placeLegalityCheck(compared_circuit))
    return false;

  // Density check
  if (!densityCheck()) {
    cout << "This circuit doesn't pass density threshold." << endl;
    return false;
  }

  return true;
}
vector<int> Evaluator::getVariableNumbers() {
  vector<int> varNums;
  varNums.push_back(getCellNumber());
  varNums.push_back(getNetNumber());
  varNums.push_back(getPinNumber());
  varNums.push_back(getPadNumber());
  return varNums;
}
int Evaluator::getCellNumber() {
  if ((int) instance_pointers_.size() < 0)
    assert(0);
  return (int) instance_pointers_.size();
}
int Evaluator::getNetNumber() {
  if ((int) net_pointers_.size() < 0)
    assert(0);
  return (int) net_pointers_.size();
}
int Evaluator::getPinNumber() {
  if ((int) pin_pointers_.size() < 0)
    assert(0);
  return (int) pin_pointers_.size();
}
int Evaluator::getPadNumber() {
  if ((int) pad_pointers_.size() < 0)
    assert(0);
  return (int) pad_pointers_.size();
}
bool Evaluator::cellNumCheck(int ref_cell_num) {
  if (ref_cell_num == instance_pointers_.size())
    return true;
  else
    return false;
}
bool Evaluator::netNumCheck(int ref_net_num) {
  if (ref_net_num == net_pointers_.size())
    return true;
  else
    return false;
}
bool Evaluator::pinNumCheck(int ref_pin_num) {
  if (ref_pin_num == pin_pointers_.size())
    return true;
  else
    return false;
}
bool Evaluator::padNumCheck(int ref_pad_num) {
  if (ref_pad_num == pad_pointers_.size())
    return true;
  else
    return false;
}
bool Evaluator::placeCheck() {
  bool all_placed = true;
  for (Instance *instance : instance_pointers_)
    if (!instance->isPlaced())
      all_placed = false;
  return all_placed;
}
bool Evaluator::isAllCellInDie() {
  const long die_w = static_cast<long>(die_->getWidth());
  const long die_h = static_cast<long>(die_->getHeight());
  for (Instance* inst : instance_pointers_) {
    auto ll = inst->getCoordinate();
    const long urx = static_cast<long>(ll.first)  + static_cast<long>(inst->getWidth());
    const long ury = static_cast<long>(ll.second) + static_cast<long>(inst->getHeight());
    if (ll.first < 0 || ll.second < 0 || urx > die_w || ury > die_h) {
      return false;
    }
  }
  return true;
}
pair<int,int> Evaluator::getBinNumbers() {
  long aw = 0, ah = 0;
  for (Instance* inst : instance_pointers_) {
    aw += static_cast<long>(inst->getWidth());
    ah += static_cast<long>(inst->getHeight());
  }
  if (instance_pointers_.empty()) return {1,1};
  aw = std::max<long>(1, aw / static_cast<long>(instance_pointers_.size()));
  ah = std::max<long>(1, ah / static_cast<long>(instance_pointers_.size()));

  const double fx = static_cast<double>(die_->getWidth())  / static_cast<double>(aw) * 0.2;
  const double fy = static_cast<double>(die_->getHeight()) / static_cast<double>(ah) * 0.2;
  int nx = static_cast<int>(std::floor(fx));
  int ny = static_cast<int>(std::floor(fy));
  if (nx < 1) nx = 1; if (ny < 1) ny = 1;
  if (nx > 40) nx = 40; if (ny > 40) ny = 40;
  return {nx, ny};
}
bool Evaluator::densityCheck() {
  const auto num = getBinNumbers();
  const int Nx = num.first;
  const int Ny = num.second;

  const uint64 die_w = die_->getWidth();
  const uint64 die_h = die_->getHeight();
  if (die_w == 0 || die_h == 0) return true;

  auto ceil_div = [](uint64 a, uint64 b)->uint64 { return (a + b - 1) / b; };
  const uint64 bin_w = std::max<uint64>(1, ceil_div(die_w, static_cast<uint64>(Nx)));
  const uint64 bin_h = std::max<uint64>(1, ceil_div(die_h, static_cast<uint64>(Ny)));

  struct Bin {
    pair<int,int> ll, ur;
    uint64 cell_area{0};
    uint64 area() const {
      return static_cast<uint64>(std::max(0, ur.first  - ll.first)) *
             static_cast<uint64>(std::max(0, ur.second - ll.second));
    }
    void accumulateOverlap(Instance* inst) {
      auto p = inst->getCoordinate();
      const int inst_llx = p.first;
      const int inst_lly = p.second;
      const int inst_urx = inst_llx + static_cast<int>(inst->getWidth());
      const int inst_ury = inst_lly + static_cast<int>(inst->getHeight());

      const int Lx = std::max(ll.first,  inst_llx);
      const int Ly = std::max(ll.second, inst_lly);
      const int Ux = std::min(ur.first,  inst_urx);
      const int Uy = std::min(ur.second, inst_ury);
      const int ow = Ux - Lx;
      const int oh = Uy - Ly;
      if (ow > 0 && oh > 0) cell_area += static_cast<uint64>(ow) * static_cast<uint64>(oh);
    }
  };

  // Build exactly Nx × Ny bins
  std::vector<std::vector<Bin>> bins(Nx, std::vector<Bin>(Ny));
  for (int ix = 0; ix < Nx; ++ix) {
    for (int iy = 0; iy < Ny; ++iy) {
      const int llx = static_cast<int>(ix * bin_w);
      const int lly = static_cast<int>(iy * bin_h);
      // Clip last bin to die boundary
      const int urx = static_cast<int>(std::min<uint64>(die_w, (ix + 1) * bin_w));
      const int ury = static_cast<int>(std::min<uint64>(die_h, (iy + 1) * bin_h));
      bins[ix][iy] = Bin{ {llx, lly}, {urx, ury}, 0 };
    }
  }

  auto clamp = [](int v, int lo, int hi){ return v < lo ? lo : (v > hi ? hi : v); };

  // Accumulate area into intersecting bins
  for (Instance* inst : instance_pointers_) {
    auto p = inst->getCoordinate();
    const int llx = p.first;
    const int lly = p.second;
    const int urx = llx + static_cast<int>(inst->getWidth());
    const int ury = lly + static_cast<int>(inst->getHeight());

    // Map coordinates to bin indices. Use -1 on upper bound so a boundary lands in the last valid bin.
    const int li = clamp(llx / static_cast<int>(bin_w), 0, Nx - 1);
    const int lj = clamp(lly / static_cast<int>(bin_h), 0, Ny - 1);
    const int ri = clamp((std::max(0, urx - 1)) / static_cast<int>(bin_w), 0, Nx - 1);
    const int rj = clamp((std::max(0, ury - 1)) / static_cast<int>(bin_h), 0, Ny - 1);

    for (int ix = li; ix <= ri; ++ix)
      for (int iy = lj; iy <= rj; ++iy)
        bins[ix][iy].accumulateOverlap(inst);
  }

  // Find max density over true bin areas
  uint64 max_area = 0;
  Bin max_bin;
  for (int iy = 0; iy < Ny; ++iy) {
    for (int ix = 0; ix < Nx; ++ix) {
      if (bins[ix][iy].cell_area > max_area) {
        max_area = bins[ix][iy].cell_area;
        max_bin = bins[ix][iy];
      }
    }
  }

  const double den = (max_bin.area() == 0) ? 0.0
                   : static_cast<double>(max_area) / static_cast<double>(max_bin.area());
  if (den > 1.2) {
    std::cout << "The overflow grid is \n"
              << "(" << max_bin.ll.first << ", " << max_bin.ll.second << "), "
              << "(" << max_bin.ur.first << ", " << max_bin.ur.second << ")\n"
              << "(" << max_bin.ll.first / this->getUnitOfMicro() << ", "
              << max_bin.ll.second / this->getUnitOfMicro() << "), "
              << "(" << max_bin.ur.first / this->getUnitOfMicro() << ", "
              << max_bin.ur.second / this->getUnitOfMicro() << ")  in micro units\n"
              << "Max density: " << den << std::endl;
    return false;
  }
  return true;
}

}

/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010-2014 Joel Andersson, Joris Gillis, Moritz Diehl,
 *                            K.U. Leuven. All rights reserved.
 *    Copyright (C) 2011-2014 Greg Horn
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#ifndef CASADI_KNITRO_INTERFACE_HPP
#define CASADI_KNITRO_INTERFACE_HPP

#include <casadi/interfaces/knitro/casadi_nlpsol_knitro_export.h>
#include <knitro.h>
#include "casadi/core/function/nlpsol_impl.hpp"

/** \defgroup plugin_Nlpsol_knitro
  KNITRO interface
*/

/** \pluginsection{Nlpsol,knitro} */

/// \cond INTERNAL
namespace casadi {
  // Forward declaration
  class KnitroInterface;

  struct CASADI_NLPSOL_KNITRO_EXPORT KnitroMemory : public NlpsolMemory {
    /// Function object
    const KnitroInterface& self;

    // KNITRO context pointer
    KTR_context_ptr kc;

    // Inputs
    double *wx, *wlbx, *wubx, *wlbg, *wubg;

    // Stats
    const char* return_status;

    /// Constructor
    KnitroMemory(const KnitroInterface& self);

    /// Destructor
    ~KnitroMemory();
  };

  /** \brief \pluginbrief{Nlpsol,knitro}
     @copydoc Nlpsol_doc
     @copydoc plugin_Nlpsol_knitro
  */
  class CASADI_NLPSOL_KNITRO_EXPORT KnitroInterface : public Nlpsol {
  public:
    // NLP functions
    Function fg_fcn_;
    Function gf_jg_fcn_;
    Function hess_l_fcn_;
    Sparsity jacg_sp_;
    Sparsity hesslag_sp_;

    explicit KnitroInterface(const std::string& name, const Function& nlp);
    virtual ~KnitroInterface();

    // Get name of the plugin
    virtual const char* plugin_name() const { return "knitro";}

    /** \brief  Create a new NLP Solver */
    static Nlpsol* creator(const std::string& name, const Function& nlp) {
      return new KnitroInterface(name, nlp);
    }

    ///@{
    /** \brief Options */
    static Options options_;
    virtual const Options& get_options() const { return options_;}
    ///@}

    // Initialize the solver
    virtual void init(const Dict& opts);

    /** \brief Create memory block */
    virtual void* alloc_memory() const { return new KnitroMemory(*this);}

    /** \brief Free memory block */
    virtual void free_memory(void *mem) const { delete static_cast<KnitroMemory*>(mem);}

    /** \brief Initalize memory block */
    virtual void init_memory(void* mem) const;

    /** \brief Set the (persistent) work vectors */
    virtual void set_work(void* mem, const double**& arg, double**& res,
                          int*& iw, double*& w) const;

    // Solve the NLP
    virtual void solve(void* mem) const;

    /// Can discrete variables be treated
    virtual bool integer_support() const { return true;}

    // KNITRO callback wrapper
    static int callback(const int evalRequestCode, const int n, const int m, const int nnzJ,
                        const int nnzH, const double * const x, const double * const lambda,
                        double * const obj, double * const c, double * const objGrad,
                        double * const jac, double * const hessian,
                        double * const hessVector, void *userParams);

    // KNITRO return codes
    static const char* return_codes(int flag);

    // KNITRO options
    Dict opts_;

    // Type of constraints
    std::vector<int> contype_;

    /// A documentation string
    static const std::string meta_doc;
  };

} // namespace casadi

/// \endcond
#endif // CASADI_KNITRO_INTERFACE_HPP
